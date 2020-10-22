#include <entities\widgets\w_editabletextbox.h>

#include <workers\window_worker.h>
#include <utility\text.h>
#include <utility\widget.h>
#include <font_symbols.h>

#include <cmath>

//:: Constructors.

w_editabletextbox::w_editabletextbox
(
        const point& Position,
        const point& Size,
        
        const std::string& Text,
        const int&         FontSize,
        const rgba&        TextColor,
        
        const rgba& BackgroundColor,
        const rgba& OutlineColor,
        const int&  OutlineThickness
)
{
        //:: Boring.
        
        // Them pointers.
        this->TextBox = std::make_shared<w_textbox>();
        this->ScrollBar = std::make_shared<w_scrollbar>();
        
        // Widget base.
        this->Position = Position;
        this->Size = Size;
        
        // Textbox.
        this->TextBox->TextSet( Text );
        this->TextBox->FontSize = FontSize;
        this->TextBox->FontColor = TextColor;
        this->TextBox->BackgroundColor = rgba( color::black, 0.0f );
        this->TextBox->OutlineThickness = 0;
        
        // Text scrollbox.
        this->BackgroundColor = BackgroundColor;
        this->OutlineColor = OutlineColor;
        this->OutlineThickness = OutlineThickness;
        
        //:: For text editing.
        this->bKeyboardFocusable = true;
        this->bInvalidateOnKeyboardFocus = true;
        this->SetCaretTarget( this->TextBox );
        this->bAllowNewlines = true;
}

//:: Overrides.

void w_editabletextbox::OnTick()
{
        w_textscrollbox::OnTick();
        const double CurrentTime = glfwGetTime();
        this->bTextCaretVisible =
        (
                (
                        ( fmod(CurrentTime , CaretPeriod ) < CaretPeriod/2.0 )
                        or
                        ( CurrentTime - this->LastCaretBump < 0.5f )
                )
                and
                this->bKeyboardFocused
        );
        
        // Dynamic selection visuals.
        if( this->bBegunClickSelection ) {
                
                const text_coord TextCoord = this->TextBox->PositionToTextCoord( MousePosition() );
                
                if( TextCoord == ToTextCoord( this->CaretPositionGet(), this->LineMapGet() ) )
                        return;
                
                this->CaretPositionSet( TextCoord );
                
                this->CaretFixupPosition();
                this->BumpCaret();
                
                this->Invalidate();
        }
}

void w_editabletextbox::OnRefresh( ValidityState_t Reason )
{
        // Otherwise parent would be invisible.
        w_textscrollbox::OnRefresh( Reason );
        // It glitches out sometimes.
        this->CaretFixupPosition();
        
        this->gText.Clear();

        //:: Caret.
        const fpoint Pixel = point( pixel(1) );
        
        const std::pair< fpoint, fpoint > PaddedArea =
        OutlineAdjustedArea(
                Position,
                SecondPosition( Position, Size ),
                this->OutlineThickness
        );
        
        // TODO: Make a cleanup pass on this mess.
        
        const std::size_t CaretLine      = this->CaretCurrentLineIndexGet();
        const std::size_t CaretCharacter = this->CaretOffsetGet();
        
        const float OffsetLines = (float)CaretLine - this->GetScrollOffsetLines();
        
        const double& TextOffset = this->TextBox->Offset;
        
        const std::size_t LineCount = CountCharacter( this->TextGet(), '\n' )+1;
        
        const auto   CutPair   = TextCutsFromArea( LineCount, this->TextBox->TextViewzoneY(), TextOffset );
        const float& TopCut    = CutPair.first;
        const float& BottomCut = CutPair.second;
        
        const std::ptrdiff_t CaretRelativeY = ceil( OffsetLines );
        std::size_t          LastLine       = ceil( this->TextBox->TextViewzoneY() )-1;
        
        if( TopCut + BottomCut > 1.0f ) {
                LastLine += 1;
        }
        
        float FinalTopCut = 0.0f;
        float FinalBottomCut = 0.0f;
        
        if( CaretRelativeY >= 0 && CaretRelativeY <= (ptrdiff_t)LastLine ) {
                if( CaretRelativeY == 0 ) {
                        FinalTopCut = TopCut;
                }
                
                if( CaretRelativeY == (ptrdiff_t)LastLine ) {
                        FinalBottomCut = BottomCut;
                }
                
                const fpoint CaretOriginLocal = fpoint (
                        (float)(CaretCharacter*this->TextBox->FontSize)/2.0f*Pixel.x,
                        (-OffsetLines-FinalTopCut)*this->TextBox->FontSize*Pixel.y
                );
                const fpoint CaretOrigin = PaddedArea.first + CaretOriginLocal;
                
                this->gText.AddText(
                        std::string( 1, fsym::caret ),
                        this->TextBox->FontSize,
                        CaretOrigin,
                        this->TextBox->FontColor,
                        FinalTopCut,
                        FinalBottomCut
                );
                
        }
        
        this->gText.Update();
        
}


void w_editabletextbox::OnDraw()
{
        // Otherwise parent would be invisible, once more.
        w_textscrollbox::OnDraw();
        if( this->bTextCaretVisible ) {
                this->gText.Draw();
        }
}

void w_editabletextbox::OnKeyboardFocused()
{
        const std::size_t CurrentLine = this->CaretCurrentLineIndexGet();
        this->ScrollIntoView( CurrentLine );
}

void w_editabletextbox::OnCharacterInput( const std::string& Input )
{
        if( this->ProcessCharacterInput( Input ) )
        {
                this->Invalidate( ValidityState::ParametersUpdated );
                const std::size_t CurrentLine = this->CaretCurrentLineIndexGet();
                this->ScrollIntoView( CurrentLine );
        }
}

void w_editabletextbox::OnKeyInput( const int& Key, const int& Modifiers )
{
        if( this->ProcessKeyInput( Key, Modifiers ) )
        {
                this->Invalidate( ValidityState::ParametersUpdated );	
                const std::size_t CurrentLine = this->CaretCurrentLineIndexGet();
                this->ScrollIntoView( CurrentLine );
        }
}

void w_editabletextbox::OnMousePressed( const int Button )
{
        
        // HACK: Fixes mouse selection activating while using the scrollbar.
        if( this->ScrollBar->DoesCollide( MousePosition() ) )
                return;
        
        if( Button == GLFW_MOUSE_BUTTON_1 && this->bKeyboardFocused ) {
                text_coord TextCoord = this->TextBox->PositionToTextCoord( MousePosition() );
                
                this->VoidCaretSelection();
                this->CaretPositionSet( TextCoord );
                this->BumpCaret();
                
                this->CaretFixupPosition();
                this->StartSelection();
                
                this->bBegunClickSelection = true;
                
                this->Invalidate();
                
        }
}

void w_editabletextbox::OnMouseReleased( const int Button, const bool )
{
        if( Button == GLFW_MOUSE_BUTTON_1 && this->bBegunClickSelection ) {
                
                const text_coord TextCoord = this->TextBox->PositionToTextCoord( MousePosition() );
                this->CaretPositionSet( TextCoord );
                
                this->CaretFixupPosition();
                this->BumpCaret();
                
                this->bBegunClickSelection = false;
                
                this->Invalidate();
        }
}


