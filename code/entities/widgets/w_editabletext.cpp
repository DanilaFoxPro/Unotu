#include <entities\widgets\w_editabletext.h>

#include <entities\ent_opengl.h>
#include <workers\widget_worker.h>
#include <workers\window_worker.h>

#include <utility\text.h>
#include <utility\shortcuts.h>
#include <utility\widget.h>

#include <events\widget_event.h>
#include <font_symbols.h>

#include <cmath> // Remainder.

w_editabletext::w_editabletext( const point& Position, const point& Size, const int Layer )
{
	this->Position = Position;
	this->Size = Size;
	this->Layer = Layer;
	
}

void w_editabletext::PostConstruct()
{
        this->bKeyboardFocusable = true;
	this->bInvalidateOnKeyboardFocus = true;
        
        std::shared_ptr<widget> Lock = WidgetByPointer( this ).lock();
        std::shared_ptr<m_text> Cast = std::dynamic_pointer_cast<m_text>(Lock);
        this->SetCaretTarget( std::weak_ptr<m_text>(Cast) );
}


void w_editabletext::OnTick()
{
	const double CurrentTime = glfwGetTime();
	const bool bCaretShouldBeVisible = 
	(
		(
			( fmod(CurrentTime , CaretPeriod ) < CaretPeriod/2.0 )
			or
			( CurrentTime - this->LastCaretBump < 0.5f )
		)
		and
		this->bKeyboardFocused
	);
        
	if( bTextCaretVisible != bCaretShouldBeVisible )
	{
		bTextCaretVisible = bCaretShouldBeVisible;
		this->Invalidate();
	}
	
	if( this->bBegunClickSelection ) {
                
                text_coord TextCoord = this->PositionToTextCoord( MousePosition() );
                line_coord LineCoord = ToStringCoord( TextCoord, this->GetCaretLineMap() );
                
                if( LineCoord == this->CaretPositionGet() )
                        return;
                
                this->CaretPositionSet( TextCoord );
                this->CaretFixupPosition();
                
                this->BumpCaret();
                this->Invalidate();
        }
	
}

void w_editabletext::OnRefresh( ValidityState_t )
{
	
	const point Position2 = SecondPosition( Position, Size );
	const std::pair<point, point> Area = OutlineAdjustedArea( Position, Position2, OutlineThickness );
	
	this->gColor.Clear();
	this->gText.Clear();
	
	// Outline.
	if( this->OutlineColor.alpha > 0.0f ) {
		this->gColor.AddOutline (
			         colored_rectangle( Position, Position2, this->bKeyboardFocused ? TheTheme.Accent : OutlineColor ),
			OutlineThickness
		);
	}
	
	// Background.
	if( this->BackgroundColor.alpha > 0.0f )
	{
		this->gColor.AddRectangle
		(
			         colored_rectangle( Area.first, Area.second, BackgroundColor )
		);
	}
	
	// Text.
	if( this->TextColor.alpha > 0.0f )
	{
		// Main text.
		const int FontSize = this->FontSize();
		
                if( this->Text.size() != 0 ) {
                        const std::size_t Limit = this->Text.size()-1;
                        
                        if( this->HasCaretSelection() ) {
                                
                                text_coord First  = ToTextCoord( this->FirstCaretSelection(), this->GetCaretLineMap() );
                                text_coord Second = ToTextCoord( this->SecondCaretSelection(), this->GetCaretLineMap() );
                                
                                const rgba Inverse = rgba( ~this->TextColor, this->TextColor.alpha );
                                
                                this->gText.AddText(
                                        this->Text,
                                        {{0, Limit}},
                                        FontSize,
                                        Area.first,
                                        {
                                                {{0, 0}, this->TextColor},
                                                {First, Inverse, this->TextColor},
                                                {Second, this->TextColor}
                                        }
                                );
                        } else {
                                this->gText.AddText(
                                        this->Text,
                                        {split_line{0, Limit}},
                                        FontSize,
                                        Area.first,
                                        {{{0, 0}, this->TextColor}}
                                );
                        }
                        
                }
                
		// Caret.
		
		// TODO: Fix this horror.
		
		const point CaretOrigin =
		point
		(
			Area.first.x.xratio() + ( pixel(FontSize).xratio()/2.0f * (float)(ToTextCoord( CaretPositionGet(), this->GetCaretLineMap() )).second ),
			Area.first.y
		);
		// Blinking caret.
		if( this->bTextCaretVisible )
		{
                        this->gText.AddText(
                                std::string( 1, fsym::caret ),
                                FontSize,
                                CaretOrigin,
                                this->TextColor
                        );
		}
		
	}
	
	this->gColor.Update();
	this->gText.Update();
	
}

void w_editabletext::OnDraw()
{
	//:: Draw colored rectangles.
	this->gColor.Draw();
		
	//:: Draw text.
	this->gText.Draw();
	
}

void w_editabletext::OnKeyboardFocused()
{
	//this->MoveCaretToEnd(); // I've seen this done, but is it needed?
	this->BumpCaret();
}

void w_editabletext::OnCharacterInput( const std::string& Input )
{
	if( this->ProcessCharacterInput( Input ) ) {
                this->ThrowEvent( std::make_shared<we_textupdated>() );
		this->Invalidate();
	}
}

void w_editabletext::OnKeyInput( const int& Key, const int& Modifiers )
{
	if ( this->ProcessKeyInput( Key, Modifiers ) ) {
                this->ThrowEvent( std::make_shared<we_textupdated>() );
		this->Invalidate();	
	} else {
		if( Key == GLFW_KEY_ENTER
                        && Modifiers == 0
                                && this->bCommitOnEnter) {
				this->CommitText();
		}
	}
}

void w_editabletext::OnMousePressed( const int Button )
{
        if( Button == GLFW_MOUSE_BUTTON_1 && this->bKeyboardFocused ) {
                const text_coord TextCoord = this->PositionToTextCoord( MousePosition() );
                
                this->VoidCaretSelection();
                this->CaretPositionSet( TextCoord );
                this->BumpCaret();
                
                this->CaretFixupPosition();
                this->StartSelection();
                
                this->Invalidate();
                
                this->bBegunClickSelection = true;
        }
}

void w_editabletext::OnMouseReleased( const int Button, const bool )
{
        if( Button == GLFW_MOUSE_BUTTON_1 && this->bBegunClickSelection ) {
                text_coord TextCoord = this->PositionToTextCoord( MousePosition() );
                
                this->CaretPositionSet( TextCoord );
                this->CaretFixupPosition();
                
                this->BumpCaret();
                this->Invalidate();
                
                this->bBegunClickSelection = false;
        }
}

/**
 * @brief Commit the text. Sends out a commit event.
 */
void w_editabletext::CommitText()
{
        this->ThrowEvent( std::make_shared<we_textcommit>( &this->Text ) );
        if( this->bClearOnCommit ) {
                this->TextClear();
        }
}

int w_editabletext::FontSize()
{
        const point Position2 = SecondPosition( this->Position, this->Size );
        const std::pair<point, point> Area = OutlineAdjustedArea( Position, Position2, OutlineThickness );
	const point LocalAreaSize = AreaSize( Area.first, Area.second );
        return LocalAreaSize.y.ypixels();
}


/** @brief Position in text coordinates based on on-screen position. */
text_coord w_editabletext::PositionToTextCoord( const fpoint Position )
{
        const fpoint Pixel = pixel(1);
        const fpoint LocalPosition = Localize( this->Position, Position );
        
        std::ptrdiff_t XTextCoord = LocalPosition.x / (this->FontSize()*Pixel.x/2.0f);
        
        XTextCoord = clamp(
                XTextCoord,
                (std::ptrdiff_t)(0),
                (std::ptrdiff_t)this->Text.size()
        );
        
        return {0, XTextCoord};
}


void w_editabletext::TextSet( const std::string& Text )
{
        this->Text = Text;
        this->Invalidate( ValidityState::ParametersUpdated );
}


void w_editabletext::TextClear()
{
        this->Text.clear();
        this->ThrowEvent( std::make_shared<we_textupdated>() );
        this->CaretFixupPosition();
        this->Invalidate( ValidityState::ParametersUpdated );
}

std::string w_editabletext::TextGet()
{
        return this->Text;
}

std::string* w_editabletext::TextGetRef()
{
        return &this->Text;
}
