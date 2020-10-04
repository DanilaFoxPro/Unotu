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
        
        const std::string&	Text,
        const int&			FontSize,
        const rgba&			TextColor,
        
        const rgba& BackgroundColor,
        const rgba& OutlineColor,
        const int&	OutlineThickness
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
        this->TextBox->SetText( Text );
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
}

void w_editabletextbox::OnRefresh( ValidityState_t Reason )
{
        // Otherwise parent would be invisible.
        w_textscrollbox::OnRefresh( Reason );
        // It glitches out sometimes.
        this->FixupCaretPosition();
        
        this->gText.Clear();

        //:: Caret.
        const fpoint Pixel = point( pixel(1) );
        
        const std::pair< fpoint, fpoint > PaddedArea =
                OutlineAdjustedArea(
                        Position,
                        SecondPosition( Position, Size ),
                        this->OutlineThickness
                );
        
        const std::pair< std::size_t, std::size_t > CaretCoordinates = this->TextCaretPosition;
        const float OffsetLines = (float)CaretCoordinates.first - this->GetScrollOffsetLines();
        // FIXME: Doesn't account for inserted newlines.
        const fpoint CaretOriginLocal =
                        fpoint
                        (
                                (float)(CaretCoordinates.second*this->TextBox->FontSize)/2.0f*Pixel.x,
                                -OffsetLines*this->TextBox->FontSize*Pixel.y
                        );
        const fpoint CaretOrigin = PaddedArea.first + CaretOriginLocal;
        
        this->gText.AddText(
                std::string( 1, fsym::caret ),
                this->TextBox->FontSize,
                CaretOrigin,
                this->TextBox->FontColor
        );
        
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
        const std::size_t CurrentLine = this->TextCaretPosition.first;
        this->ScrollIntoView( CurrentLine );
}

void w_editabletextbox::OnCharacterInput( const std::string& Input )
{
        if( this->ProcessCharacterInput( Input ) )
        {
                this->Invalidate( ValidityState::ParametersUpdated );
                const std::size_t CurrentLine = this->TextCaretPosition.first;
                this->ScrollIntoView( CurrentLine );
        }
}

void w_editabletextbox::OnKeyInput( const int& Key, const int& Modifiers )
{
        if( this->ProcessKeyInput( Key, Modifiers ) )
        {
                this->Invalidate( ValidityState::ParametersUpdated );	
                const std::size_t CurrentLine = this->TextCaretPosition.first;
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
                this->TextCaretPosition = TextCoord;
                this->BumpCaret();
                
                this->FixupCaretPosition();
                this->StartSelection();
                
                this->bBegunClickSelection = true;
                
                this->Invalidate();
                
        }
}

void w_editabletextbox::OnMouseReleased( const int Button, const bool )
{
        if( Button == GLFW_MOUSE_BUTTON_1 && this->bBegunClickSelection ) {
                
                const text_coord TextCoord = this->TextBox->PositionToTextCoord( MousePosition() );
                this->TextCaretPosition = TextCoord;
                
                this->FixupCaretPosition();
                this->BumpCaret();
                
                this->bBegunClickSelection = false;
                
                this->Invalidate();
        }
}


