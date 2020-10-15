#include <entities\widgets\w_textscrollbox.h>

#include <workers\widget_worker.h>
#include <workers\window_worker.h>
#include <utility\widget.h>

#include <math.h> // floor

w_textscrollbox::w_textscrollbox
(
	const point& position_,
	const point& size_,
	
	const std::string&	text,
	const int&			font_size,
	const rgba&			text_color,
	
	const rgba& background_color_,
	const rgba& outline_color_,
	const int&	outline_thickness_
)
{
	//:: Them pointers.
	this->TextBox = std::make_shared<w_textbox>();
	this->ScrollBar = std::make_shared<w_scrollbar>();
	
	//:: Widget base.
	Position = position_;
	Size = size_;
	
	//:: Textbox.
	this->TextBox->SetText( text );
	this->TextBox->FontSize = font_size;
	this->TextBox->FontColor = text_color;
	this->TextBox->BackgroundColor = rgba( color::black, 0.0f );
	this->TextBox->OutlineThickness = 0;
	
	//:: Text scrollbox.
	   BackgroundColor = background_color_;
	   OutlineColor = outline_color_;
	   OutlineThickness = outline_thickness_;
	
}

void w_textscrollbox::PostConstruct()
{
        this->bAcceptExternalScroll = true;
	this->AddChild( this->TextBox );
	this->AddChild( this->ScrollBar );
}

void w_textscrollbox::OnRefresh( ValidityState_t Reason )
{
	
        //:: Positions and sizes.
        
        const point Position2 = SecondPosition( Position, Size );
        
        std::pair< point, point > PaddedArea = OutlineAdjustedArea( Position, Position2, this->OutlineThickness );
        
        const fpoint Pixel = point( pixel(1) );

        const float min_scrollbox_width = Pixel.x*32;

        this->TextBox->Position = PaddedArea.first;
        this->TextBox->SetSecondPosition(
                point(
                        PaddedArea.second.x,
                        PaddedArea.second.y
                )
        );

        this->ScrollBar->Position = 
                point(
                        PaddedArea.second.x-ratio( min_scrollbox_width ),
                        PaddedArea.first.y
                );

        this->ScrollBar->SetSecondPosition( PaddedArea.second );

        //:: Update.

        // HACK: Makes sure 'VisibleRatio' is updated.
        this->TextBox->OnRefresh( Reason );

        //:: Offset.

        this->ScrollBar->ScrollViewzone = this->TextBox->VisibleRatio;
        this->TextBox->SetOffset( this->ScrollBar->ScrollOffset );
        
        //:: Geomery.
        
        this->gColor.Clear();
        
        if( BackgroundColor.alpha != 0.0f ){
                this->gColor.AddRectangle(
                        colored_rectangle( PaddedArea.first, PaddedArea.second, BackgroundColor )
                );
        }
        
        if( OutlineColor.alpha != 0.0f ){
                this->gColor.AddOutline(
                        colored_rectangle( Position, Position2, OutlineColor ), OutlineThickness
                );
        }
        
        this->gColor.Update();
        
        //:: Invalidate.

        // ScrollBar's and TextBox's geometries will be updated since
        // widget validation is recursive.
        this->ScrollBar->Invalidate();
        this->TextBox->Invalidate();
    
}

void w_textscrollbox::OnEvent( std::shared_ptr<widget_event> Event )
{
        widget_event* const EventPtr = Event.get();
        
        we_scrollsetratio* ScrollEvent = dynamic_cast<we_scrollsetratio*>( EventPtr );
        if( ScrollEvent ) {
                this->Invalidate();
                return ScrollEvent->Handle();
        }
        
        auto ScrollLines = dynamic_cast<we_scrolllines*>( EventPtr );
        if( ScrollLines ) {
                const double LineRatio = 1.0/this->TextBox->LineCount();
                this->ScrollBar->OffsetByRatio( -ScrollLines->Lines*LineRatio );
                this->Invalidate( ValidityState::ParametersUpdated );
        }
        
        auto ScrollPages = dynamic_cast<we_scrollpages*>( EventPtr );
        if( ScrollPages ) {
                this->ScrollBar->OffsetByViewzone( -ScrollPages->Pages );
                this->Invalidate( ValidityState::ParametersUpdated );
        }
        
}

void w_textscrollbox::OnDraw()
{
	this->gColor.Draw();
}

/*
 * @brief Set scroll offset as ratio.
 */
void w_textscrollbox::SetScrollOffset( float Ratio )
{
	this->ScrollBar->ScrollOffset = clamp( Ratio, 0.0f, 1.0f );
	this->Invalidate();
}

/*
 * @brief Set line index to be scrolled to the top of the scrollbox.
 */
void w_textscrollbox::SetScrollOffset( std::size_t Line )
{
	const w_textbox& TextBox = *this->TextBox;
	const std::size_t ViewHeight = TextBox.TextAreaSize.y.ypixels() / TextBox.FontSize;
	const float Offset = (float)Line/(float)(TextBox.LineCount()-ViewHeight);
	this->SetScrollOffset( Offset );
}

/**
 * @brief Scroll line into view.
 *        Makes sure the line falls between beginning and end of the viewzone, with minimal amount of scrolling.
 *        Doesn't scroll if line is already fully visible.
 */
void w_textscrollbox::ScrollIntoView( std::size_t Line )
{
        const w_textbox& TextBox = *this->TextBox;
        
        const double DLine = Line;
        
        const double ViewOffset   = this->GetScrollOffsetLines();
        const double Viewzone     = TextBox.TextViewzoneY();
        const double LastSeenLine = ViewOffset+Viewzone;
        
        const double ScrollLength = pixel(TextBox.FontSize).yratio() * TextBox.LineCount();
        const double LineRatio    = (ScrollLength-TextBox.TextAreaSize.y.yratio())/TextBox.LineCount();
        
        double TargetOffset;
        
        if( ViewOffset >= DLine ) {
                TargetOffset = DLine-ViewOffset;
        } else if( DLine >= floor(LastSeenLine) ) {
                TargetOffset = DLine-LastSeenLine+1.0;
        } else {
                return;
        }
        
        this->ScrollBar->OffsetByRatio( TargetOffset * LineRatio );
        this->Invalidate( ValidityState::ParametersUpdated );
        
}

float w_textscrollbox::GetScrollOffsetLines()
{
        const w_textbox& TextBox = *this->TextBox;
        const float ViewHeight = (float)TextBox.TextAreaSize.y.ypixels() / (float)TextBox.FontSize;
        if( this->ScrollBar->ScrollViewzone < 1.0f ) {
                return
                this->ScrollBar->ScrollOffset
                *
                ( (float)TextBox.LineCount() - (float)ViewHeight );
        } else {
                return 0.0f;
        }

}

void w_textscrollbox::ScrollToTop()
{
        this->SetScrollOffset( 0.0f );
}

void w_textscrollbox::ScrollToBottom()
{
        this->SetScrollOffset( 1.0f );
}

        //:: Text module.

void w_textscrollbox::SetText( const std::string& Text )
{
        this->TextBox->SetText( Text );
        this->Invalidate( ValidityState::ParametersUpdated );
}

void w_textscrollbox::ClearText()
{
        this->TextBox->ClearText();
        this->Invalidate( ValidityState::ParametersUpdated );
}

void w_textscrollbox::TextUpdated()
{
        this->TextBox->TextUpdated();
}


std::string w_textscrollbox::GetText()
{
        return this->TextBox->GetText();
}

std::string* w_textscrollbox::GetTextRef()
{
        return this->TextBox->GetTextRef();
}

std::string w_textscrollbox::GetOriginalText()
{
        return this->TextBox->GetOriginalText();
}

std::string* w_textscrollbox::GetOriginalTextRef()
{
        return this->TextBox->GetOriginalTextRef();
}


