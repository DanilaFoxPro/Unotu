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
void m_caret::CaretPositionSet( const std::size_t& NewPosition, const bool& BumpCaret )
{
	this->TextCaretPosition = NewPosition;
        this->CaretFixupPosition();
        if( BumpCaret ) {
                this->BumpCaret();
        }
}

/**
 * @brief Sets 'TextCaretPosition' as text_coord.
 */
void m_caret::CaretPositionSet( text_coord Position )
{
        this->TextCaretPosition = ToStringCoord( Position, this->GetCaretLineMap() );
        this->CaretFixupPosition();
        this->BumpCaret();
}

/**
 * @brief Get text caret position as caret_coord.
 */
caret_coord m_caret::CaretPositionGet() const
{
        return this->TextCaretPosition;
}

/** @brief Get offset of caret from the line beginning, in characters. */
std::size_t m_caret::CaretOffsetGet() const
{
        return this->CaretPositionGet() - this->CaretCurrentLineGetBeginning();
}

std::size_t m_caret::CaretCurrentLineIndexGet() const
{
        const std::shared_ptr<m_text> Lock = this->Target.lock();
        if( !Lock ) {
                return 0;
        }
        
        return CountCharacterBefore( Lock->TextGet(), '\n', this->TextCaretPosition );
}

split_line m_caret::CaretCurrentLineGet()
{
        return this->GetCaretLineMap()[ this->CaretCurrentLineIndexGet() ];
}

std::size_t m_caret::CaretCurrentLineGetBeginning() const
{
        const std::shared_ptr<m_text> Lock = this->Target.lock();
        if( !Lock ) {
                return 0;
        }
        
        return GetLineBeginning( this->TextCaretPosition, Lock->TextGet() );
        
}

std::size_t m_caret::CaretCurrentLineGetEnd() const
{
        const std::shared_ptr<m_text> Lock = this->Target.lock();
        if( !Lock ) {
                return 0;
        }
        
        return GetLineEnd( this->TextCaretPosition, Lock->TextGet() );
        
}


void m_caret::CaretToLineBeginning()
{
        this->CaretPositionSet( this->CaretCurrentLineGetBeginning() );
        this->BumpCaret();
}

void m_caret::CaretToLineEnd()
{
        this->CaretPositionSet( this->CaretCurrentLineGetEnd() );
        this->BumpCaret();
}


/**
 * @brief Sets 'TextCaretPosition' to the very beginning, with caret bump.
 * */
void m_caret::CaretToBeginning()
{
	this->CaretPositionSet( 0 );
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
        
        this->TextCaretPosition = Lock->TextGetSize();
        
        this->BumpCaret();
        
}

/**
 * @brief Offsets vertical caret position by amount specified. Clamped.
 * @note Jump lines offset leads out of current line's bounds.
 * 
 * @param Offset Offset by which to shift. Left is minus.
 * */
bool m_caret::XMoveCaret( const std::ptrdiff_t Offset, const bool  )
{
        const std::shared_ptr<m_text> Lock = this->Target.lock();
        if( !Lock ) {
                return false;
        }
        
        const std::ptrdiff_t Target = this->CaretPositionGet()+Offset;
        
        this->CaretPositionSet( clamp(
                Target,
                (std::ptrdiff_t)0,
                (std::ptrdiff_t)Lock->TextGetSize()
        ) );
        this->CaretFixupPosition();
        this->BumpCaret();
        
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
        
        const std::size_t OldLine = this->CaretCurrentLineIndexGet();
        this->CaretPositionSet( YMoveLineCoord( this->CaretPositionGet(), Offset, Lock->TextGet() ) );
        
        return OldLine != this->CaretCurrentLineIndexGet();
        
}


/**
 * @brief Changes text and caret state based on input provided.
 * Returns true if caret or text state were changed.
 */
bool m_caret::ProcessCharacterInput( const std::string& Input )
{
        
        std::shared_ptr<m_text> Lock = this->Target.lock();
        std::string* CaretTextPtr = nullptr;
        
        if( !Lock || !(CaretTextPtr = Lock->OriginalTextGetRef()) ) {
                return false;
        }
        
        this->DeleteCaretSelection();
        
        // Don't assume caret position was valid.
        this->CaretFixupPosition();
        
        CaretTextPtr->insert( this->CaretPositionGet(), Input );
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
        if( !Lock || !(CaretTextPtr = Lock->OriginalTextGetRef()) ) {
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
                                        this->CaretFixupPosition();
                                        this->BumpCaret();
                                        if( this->HasCaretSelection() ) {
                                                this->DeleteCaretSelection();
                                        } else {
                                                const std::size_t CaretAbs = this->GetCaretStrPos();
                                                if( CaretAbs != 0 ) {
                                                        CaretTextPtr->erase( CaretAbs-1, 1 );
                                                        Lock->TextUpdated();
                                                        // FIXME: Caret does funky stuff when text update causes re-split.
                                                        this->XMoveCaret( -1 );
                                                } else {
                                                        return false;
                                                }
                                        }
                                        break;
                                }
                                case GLFW_KEY_DELETE:
                                {
                                        this->CaretFixupPosition();
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
                                        this->StartSelection(false);
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
        this->CaretFixupPosition();
        return true;
}

void m_caret::CaretFixupPosition()
{
        this->TextCaretPosition = this->Normalize( this->TextCaretPosition );
}

caret_coord m_caret::Normalize( caret_coord Coord )
{
        const std::shared_ptr<m_text> Lock = this->Target.lock();
        
        if( !Lock || Lock->TextGetSize() == 0 ) {
                return 0;
        }
        
        const caret_coord Last = Lock->TextGetSize();
        const caret_coord OutputCoord = clamp( Coord, caret_coord(0), Last );
        
        return OutputCoord;
        
}

bool m_caret::HasCaretSelection() const
{
        const auto Caret = this->TextCaretPosition;
        const auto Select = this->TextSelectPosition;
        
        return Select != m_caret::InvalidSelectionPosition && Caret != Select;
}

void m_caret::VoidCaretSelection()
{
        this->TextSelectPosition = m_caret::InvalidSelectionPosition;
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
        if( this->TextCaretPosition < this->TextSelectPosition ) {
                return this->TextCaretPosition;
        } else {
                return this->TextSelectPosition;
        }
}

caret_coord& m_caret::SecondCaretSelection()
{
        if( this->TextCaretPosition > this->TextSelectPosition ) {
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
        
        if( !Lock || !(CaretTextPtr = Lock->OriginalTextGetRef()) ) {
                return "";
        }
        
        const std::size_t First  = this->FirstCaretSelection();
        const std::size_t Second = this->SecondCaretSelection();
        const std::size_t Length = Second-First;
        
        return CaretTextPtr->substr( First, Length );
        
}

/** Deletes all selected characters and calls `VoidCaretSelection()`. */
void m_caret::DeleteCaretSelection()
{
        if( !this->HasCaretSelection() ) {
                this->VoidCaretSelection();
                return;
        }
        
        std::shared_ptr<m_text> Lock = this->Target.lock();
        std::string* CaretTextPtr = nullptr;
        
        if( !Lock || !(CaretTextPtr = Lock->OriginalTextGetRef()) ) {
                return;
        }
        
        const std::size_t First  = this->FirstCaretSelection();
        const std::size_t Second = this->SecondCaretSelection();
        const std::size_t Length = Second-First;
        
        if( this->TextCaretPosition != this->FirstCaretSelection() ) {
                this->TextCaretPosition = this->FirstCaretSelection();
        }
        
        CaretTextPtr->erase( First, Length );
        
        Lock->TextUpdated();
        this->VoidCaretSelection();
        
}


size_t m_caret::GetCaretStrPos()
{
        return this->TextCaretPosition;
}

std::vector<split_line> m_caret::GetCaretLineMap()
{
        const std::shared_ptr<m_text> Lock = this->Target.lock();
        auto Multi = dynamic_cast<m_textlines*>( Lock.get() );
        if( !Lock ) {
                return {split_line(0, 0)};
        }
        if( Multi ) {
                return Multi->LineMapGet();
        } else {
                return {split_line( 0, Lock->TextGet().size() )};
        }
}

