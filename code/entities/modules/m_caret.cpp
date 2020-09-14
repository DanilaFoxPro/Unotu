#include <entities\modules\m_caret.h>

#include <utility\deps\opengl_includes.h> // 'glfwGetTime()'.

#include <utility\text.h>
#include <utility\clipboard.h>
#include <utility\shortcuts.h>

/**
 * @brief Set the text over which the caret is shown.
 * Used for getting the text length when normalizing or moving to end.
 * 
 * @param Text Editable text pointer.
 * @param SplitText Pointer to split text. (Optional.)
 */
void m_caret::SetCaretTarget( std::weak_ptr<m_text> Target )
{
        if( !Target.expired() ) {
                this->Target = Target;
        }
}

/**
 * @brief 'Bumps' the caret. Means it should stop blinking and be visible for a while.
 * */
void m_caret::BumpCaret()
{
	this->LastCaretBump = glfwGetTime();
}

/**
 * @brief Sets 'TextCaretPosition', but with position checks and caret bump.
 */
void m_caret::SetCaretPosition( const std::size_t& NewPosition, const bool& BumpCaret )
{
	this->TextCaretPosition = ToLineCoord( NewPosition, this->GetCaretLineMap() );
        this->FixupCaretPosition();
        if( BumpCaret ) {
                this->BumpCaret();
        }
}

split_line m_caret::GetCurCaretLine()
{
        return this->GetCaretLineMap()[this->TextCaretPosition.first];
}

void m_caret::CaretToLineBeginning()
{
        this->TextCaretPosition.second = 0;
        this->BumpCaret();
}

void m_caret::CaretToLineEnd()
{
        const split_line Line = this->GetCurCaretLine();
        this->TextCaretPosition.second = Line.Length();
        this->BumpCaret();
}


/**
 * @brief Sets 'TextCaretPosition' to the very beginning, with caret bump.
 * */
void m_caret::CaretToBeginning()
{
	this->SetCaretPosition( 0, true );
}

/**
 * @brief Sets 'TextCaretPosition' to the very end, with caret bump.
 * */
void m_caret::CaretToEnd()
{
        const std::shared_ptr<m_text> Lock = this->Target.lock();
        if( !Lock ) {
                return;
        }

        const auto Lines = this->GetCaretLineMap();
        const auto Max = GetMaxLineCoord( Lines );
        
        this->TextCaretPosition = Max;
        
        this->BumpCaret();
        
}

/**
 * @brief Offsets vertical caret position by amount specified. Clamped.
 * @note Jump lines offset leads out of current line's bounds.
 * 
 * @param Offset Offset by which to shift. Left is minus.
 * */
bool m_caret::XMoveCaret( const std::ptrdiff_t Offset, const bool bAllowOverflow )
{
        /* TODO: Consider replacing those if statements with convertion to single
         *       coordinate, operating upon it, and then converting back to double.
         */
        
        auto& CaretPos = this->TextCaretPosition;
        
        const auto Lines = this->GetCaretLineMap();
        const split_line Line = Lines[CaretPos.first];
        
        if( Lines.size() == 0 ) {
                this->FixupCaretPosition();
                return true;
        }
        
        const std::ptrdiff_t Offseted = CaretPos.second+Offset;
        if( Offseted < 0 ) {
                if( CaretPos.first == 0 ) {
                        CaretPos.second = 0;
                        //printf( "-Final offset: [%i, %i]\n", (int)CaretPos.first, (int)CaretPos.second );
                        return true;
                }
                
                CaretPos.first--;
                // '+1' since jumping lines.
                const std::ptrdiff_t NewOffset = CaretPos.second+Offset+1;
                const split_line NewLine = Lines[this->TextCaretPosition.first];
                
                CaretPos.second = NewLine.Length();
                this->XMoveCaret( NewOffset );
                
        } else if( Offseted > (std::ptrdiff_t)Line.Length() ) {
                
                if( bAllowOverflow ) {
                        CaretPos.second = Offseted;
                        /*printf(
                                "#Final offset: [%i, %i/%i]\n",
                               (int)CaretPos.first,
                               (int)CaretPos.second,
                               (int)Line.Length()
                        );*/
                        return true;
                }
                
                if( CaretPos.first == Lines.size()-1 ) {
                        CaretPos.second = Line.Length();
                        /*printf(
                                "+Final offset: [%i, %i/%i]\n",
                               (int)CaretPos.first,
                               (int)CaretPos.second,
                               (int)Line.Length()
                        );*/
                        return true;
                }
                
                CaretPos.first++;
                
                // TODO: Fix the offset here. It should change when changing the lines, but I'm puzzled by how much.
                
                const std::size_t DistanceToEnd = Line.Length()-CaretPos.second;
                // '-1' due to jumping the lines.
                const std::ptrdiff_t NewOffset = Offset-DistanceToEnd-1;
                
                CaretPos.second = 0;
                this->XMoveCaret( NewOffset );
                
        } else {
                CaretPos.second = Offseted;
                //printf( "Final offset: [%i, %i]\n", (int)CaretPos.first, (int)CaretPos.second );
        }
        
        return Offset != 0;
}

/**
 * @brief Offsets vertical caret position by amount specified. Clamped.
 * 
 * @param Offset Offset by which to shift. Top is minus.
 * */
bool m_caret::YMoveCaret( const std::ptrdiff_t Offset )
{
        const std::shared_ptr<m_text> Lock = this->Target.lock();
        if( !Lock ) {
                return false;
        }
        this->BumpCaret();
        
        const auto Lines = this->GetCaretLineMap();
        auto& CaretPos = this->TextCaretPosition;
        
        const std::size_t OldLine = CaretPos.first;
        const std::ptrdiff_t NewLine = OldLine+Offset;
        
        if( NewLine < 0 || Lines.size() == 0 ) {
                CaretPos.first = 0;
        } else if( NewLine >= (std::ptrdiff_t)Lines.size() ) {
                CaretPos.first = Lines.size()-1;
        } else {
                CaretPos.first = NewLine;
        }
        
        this->FixupCaretPosition();
        
        return OldLine != CaretPos.first;
        
}


/**
 * @brief Changes text and caret state based on input provided.
 * Returns true if caret or text state were changed.
 */
bool m_caret::ProcessCharacterInput( const std::string& Input )
{
        
        std::shared_ptr<m_text> Lock = this->Target.lock();
        std::string* CaretTextPtr = nullptr;
        
        if( !Lock || !(CaretTextPtr = Lock->GetOriginalTextRef()) ) {
                return false;
        }
        
        this->DeleteCaretSelection();
        
        // Don't assume caret position was valid.
        this->FixupCaretPosition();
        
        auto Line = this->GetCurCaretLine();
        
        CaretTextPtr->insert( Line[this->TextCaretPosition.second], Input );
        if( CaretTextPtr->size() > this->TextMaxLength ) {
                CaretTextPtr->resize( this->TextMaxLength );
        }
        Lock->TextUpdated();
        
        this->XMoveCaret( Input.size() );
        this->BumpCaret();
        
        return true;
}

/**
 * @brief Changes text and caret state based on key.
 * Returns true if caret or text state were changed.
 */
bool m_caret::ProcessKeyInput( const int& Key, const int& Modifiers )
{
        const std::shared_ptr<m_text> Lock = this->Target.lock();
        std::string* CaretTextPtr = nullptr;
        if( !Lock || !(CaretTextPtr = Lock->GetOriginalTextRef()) ) {
                return false;
        }
        
        const int ControlAndShift = (GLFW_MOD_CONTROL | GLFW_MOD_SHIFT);
        
        switch( Modifiers ) {
                case 0://No modifiers.
                {
                        switch( Key )
                        {
                                case GLFW_KEY_LEFT:
                                        this->XMoveCaret( -1 );
                                        this->BumpCaret();
                                        break;
                                case GLFW_KEY_RIGHT:
                                        this->XMoveCaret( +1 );
                                        this->BumpCaret();
                                        break;
                                case GLFW_KEY_UP:
                                        this->VoidCaretSelection();
                                        return this->YMoveCaret( -1 );
                                        break;
                                case GLFW_KEY_DOWN:
                                        this->VoidCaretSelection();
                                        return this->YMoveCaret( +1 );
                                        break;
                                case GLFW_KEY_HOME:
                                        this->CaretToLineBeginning();
                                        break;
                                case GLFW_KEY_END:
                                        this->CaretToLineEnd();
                                        break;
                                case GLFW_KEY_BACKSPACE:
                                {
                                        this->FixupCaretPosition();
                                        this->BumpCaret();
                                        if( this->HasCaretSelection() ) {
                                                this->DeleteCaretSelection();
                                        } else {
                                                const std::size_t CaretAbs = this->GetCaretStrPos();
                                                if( CaretAbs != 0 ) {
                                                        CaretTextPtr->erase( CaretAbs-1, 1 );
                                                        Lock->TextUpdated();
                                                        this->XMoveCaret( -1 );
                                                } else {
                                                        return false;
                                                }
                                        }
                                        break;
                                }
                                case GLFW_KEY_DELETE:
                                {
                                        this->FixupCaretPosition();
                                        this->BumpCaret();
                                        
                                        if( this->HasCaretSelection() ) {
                                                this->DeleteCaretSelection();
                                        } else {
                                                const std::size_t CaretAbs = this->GetCaretStrPos();
                                                if( CaretAbs != CaretTextPtr->size() ) {
                                                        CaretTextPtr->erase( CaretAbs, 1 );
                                                        Lock->TextUpdated();
                                                } else {
                                                        return false;
                                                }
                                        }
                                        
                                        break;
                                }
                                case GLFW_KEY_ENTER:
                                        if( this->bAllowNewlines ) {
                                                return this->ProcessCharacterInput( "\n" );
                                        } else {
                                                return false;
                                        }
                                        break; // Yeah!
                                default:
                                        return false;
                        }// switch( Key )
                        this->VoidCaretSelection();
                        break;
                }// case 0
                case GLFW_MOD_CONTROL:
                {
                        switch( Key )
                        {
                                case GLFW_KEY_HOME:
                                        this->CaretToBeginning();
                                        this->VoidCaretSelection();
                                        break;
                                case GLFW_KEY_END:
                                        this->CaretToEnd();
                                        this->VoidCaretSelection();
                                        break;
                                // Exchange selection. (Swap caret between start and end.)
                                case GLFW_KEY_E:
                                        if( this->HasCaretSelection() ) {
                                                const caret_coord Temp = this->TextSelectPosition;
                                                this->StartSelection(false);
                                                this->TextCaretPosition = Temp;
                                        }
                                        break;
                                // Select all.
                                case GLFW_KEY_A:
                                        this->CaretToBeginning();
                                        this->StartSelection();
                                        this->CaretToEnd();
                                        break;
                                // Copy.
                                case GLFW_KEY_C:
                                        SetClipboard( this->GetSelectedSubstring() );
                                        break;
                                // Paste.
                                case GLFW_KEY_V:{
                                        const std::string Clipboard = GetClipboard();
                                        printf( "Got clipboard: %s\n", Clipboard.c_str() );
                                        this->ProcessCharacterInput( Clipboard );
                                        break;
                                }
                                default:
                                        return false;
                        }// switch( Key )
                        this->BumpCaret();
                        break;
                }// case GLFW_MOD_CONTROL
                case GLFW_MOD_SHIFT:
                {
                        switch( Key )
                        {
                                case GLFW_KEY_LEFT:
                                        this->StartSelection();
                                        this->XMoveCaret( -1 );
                                        this->BumpCaret();
                                        break;
                                case GLFW_KEY_RIGHT:
                                        this->StartSelection();
                                        this->XMoveCaret( +1 );
                                        this->BumpCaret();
                                        break;
                                case GLFW_KEY_UP:
                                        this->StartSelection();
                                        return this->YMoveCaret( -1 );
                                        break;
                                case GLFW_KEY_DOWN:
                                        this->StartSelection();
                                        return this->YMoveCaret( +1 );
                                        break;
                                case GLFW_KEY_HOME:
                                        this->StartSelection();
                                        this->CaretToLineBeginning();
                                        break;
                                case GLFW_KEY_END:
                                        this->StartSelection();
                                        this->CaretToLineEnd();
                                        break;
                                default:
                                        return false;
                        }// switch( Key )
                        break;
                        return true;
                }// case GLFW_MOD_SHIFT
                case ControlAndShift:
                {
                        switch( Key )
                        {
                                case GLFW_KEY_HOME:
                                        this->StartSelection();
                                        this->CaretToBeginning();
                                        break;
                                case GLFW_KEY_END:
                                        this->StartSelection();
                                        this->CaretToEnd();
                                        break;
                                default:
                                        return false;
                        }
                        return true;
                }// case ControlAndShift
                default:
                        return false;
        }// switch( Modifiers )
        
        // Only reaches this point if case not default, thus input was handled.
        this->FixupCaretPosition();
        return true;
}

void m_caret::FixupCaretPosition()
{
        this->TextCaretPosition = this->Normalize( this->TextCaretPosition );
}

caret_coord m_caret::Normalize( caret_coord Coord )
{
        const std::shared_ptr<m_text> Lock = this->Target.lock();
        const std::vector<split_line> Lines = this->GetCaretLineMap();
        
        if( !Lock || Lines.size() == 0 ) {
                return {0, 0};
        }
        
        const size_t Last = Lines.size()-1;
        
        caret_coord OutputCoord = Coord;
        
        if( OutputCoord.first > Last ) {
                OutputCoord.first = Last;
        }
        
        const split_line Line = Lines[OutputCoord.first];
        
        if( OutputCoord.second > Line.Length() ) {
                OutputCoord.second = Line.Length();
        }
        
        return OutputCoord;
        
}

bool m_caret::HasCaretSelection()
{
        const auto Caret = this->TextCaretPosition;
        const auto Select = this->TextSelectPosition;
        
        return Select.first != (std::size_t)(-1) && Caret != Select;
}

void m_caret::VoidCaretSelection()
{
        this->TextSelectPosition.first = -1;
}

void m_caret::StartSelection( const bool bOnlyIfVoid )
{
        if( bOnlyIfVoid && this->HasCaretSelection() ) {
                return;
        }
        
        this->TextSelectPosition = this->TextCaretPosition;
        
}


caret_coord& m_caret::FirstCaretSelection()
{
        if( this->TextCaretPosition.first == this->TextSelectPosition.first ) {
                if( this->TextCaretPosition.second < this->TextSelectPosition.second ) {
                        return this->TextCaretPosition;
                } else {
                        return this->TextSelectPosition;
                }
        } else if( this->TextCaretPosition.first < this->TextSelectPosition.first ) {
                return this->TextCaretPosition;
        } else {
                return this->TextSelectPosition;
        }
}

caret_coord& m_caret::SecondCaretSelection()
{
        if( this->TextCaretPosition.first == this->TextSelectPosition.first ) {
                if( this->TextCaretPosition.second > this->TextSelectPosition.second ) {
                        return this->TextCaretPosition;
                } else {
                        return this->TextSelectPosition;
                }
        } else if( this->TextCaretPosition.first > this->TextSelectPosition.first ) {
                return this->TextCaretPosition;
        } else {
                return this->TextSelectPosition;
        }
}

std::string m_caret::GetSelectedSubstring()
{
        if( !this->HasCaretSelection() ) {
                return "";
        }
        
        std::shared_ptr<m_text> Lock = this->Target.lock();
        std::string* CaretTextPtr = nullptr;
        
        if( !Lock || !(CaretTextPtr = Lock->GetOriginalTextRef()) ) {
                return "";
        }
        
        const std::size_t First = ToStringCoord( this->FirstCaretSelection(), this->GetCaretLineMap() );
        const std::size_t Second = ToStringCoord( this->SecondCaretSelection(), this->GetCaretLineMap() );
        const std::size_t Length = Second-First;
        
        return CaretTextPtr->substr( First, Length );
        
}


void m_caret::DeleteCaretSelection()
{
        if( !this->HasCaretSelection() ) {
                return;
        }
        
        std::shared_ptr<m_text> Lock = this->Target.lock();
        std::string* CaretTextPtr = nullptr;
        
        if( !Lock || !(CaretTextPtr = Lock->GetOriginalTextRef()) ) {
                return;
        }
        
        const std::size_t First = ToStringCoord( this->FirstCaretSelection(), this->GetCaretLineMap() );
        const std::size_t Second = ToStringCoord( this->SecondCaretSelection(), this->GetCaretLineMap() );
        const std::size_t Length = Second-First;
        
        if( this->TextCaretPosition != this->FirstCaretSelection() ) {
                this->TextCaretPosition = this->FirstCaretSelection();
        }
        
        CaretTextPtr->erase( First, Length );
        
        this->VoidCaretSelection();
        
}


size_t m_caret::GetCaretStrPos()
{
        const std::shared_ptr<m_text> Lock = this->Target.lock();
        auto Multi = dynamic_cast<m_textlines*>( Lock.get() );
        if( !Lock || !Multi ) {
                return this->TextCaretPosition.second;
        }
        return ToStringCoord( this->TextCaretPosition, this->GetCaretLineMap() );
}

std::vector<split_line> m_caret::GetCaretLineMap()
{
        const std::shared_ptr<m_text> Lock = this->Target.lock();
        auto Multi = dynamic_cast<m_textlines*>( Lock.get() );
        if( !Lock ) {
                return {split_line(0, 0)};
        }
        if( Multi ) {
                return Multi->GetLineMap();
        } else {
                return {split_line( 0, Lock->GetText().size() )};
        }
}

