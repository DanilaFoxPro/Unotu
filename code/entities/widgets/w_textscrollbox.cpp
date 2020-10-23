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
        this->TextBox->TextSet( text );
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

        const float MinimumScrollboxWidth = Pixel.x*w_scrollbar::IdealWidth;

        this->TextBox->Position = PaddedArea.first;
        this->TextBox->SetSecondPosition( point(
                        PaddedArea.second.x,
                        PaddedArea.second.y
        ) );

        this->ScrollBar->Position = point(
                PaddedArea.second.x-ratio( MinimumScrollboxWidth ),
                PaddedArea.first.y
        );

        this->ScrollBar->SetSecondPosition( PaddedArea.second );

        //:: Update.

        // HACK: Makes sure all cached members are set.
        this->TextBox->TextUpdated();
        
        //:: Offset and size.

        this->ScrollBar->ScrollViewzoneSet( this->TextBox->TextViewzoneY() );
        this->ScrollBar->ScrollLengthSet( this->TextBox->LineCount() );
        this->TextBox->Offset = this->ScrollBar->ScrollOffsetGet();
        
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
                this->ScrollBar->Offset( -ScrollLines->Lines );
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

/**
 * @brief Set scroll offset as ratio.
 */
void w_textscrollbox::SetScrollOffset( float Ratio )
{
        this->ScrollBar->ScrollOffsetSet( Ratio*this->ScrollBar->MaximumOffset() );
	this->Invalidate();
}

/**
 * @brief Set line index to be scrolled to the top of the scrollbox.
 */
void w_textscrollbox::SetScrollOffset( std::size_t Line )
{
	this->ScrollBar->ScrollOffsetSet( (double)Line );
}

/**
 * @brief Scroll line into view.
 *        Makes sure the line falls between beginning and end of the viewzone, with minimal amount of scrolling.
 *        Doesn't scroll if line is already fully visible.
 */
void w_textscrollbox::ScrollIntoView( std::size_t Line )
{
        
        const double FirstVisibleLine = this->ScrollBar->ScrollOffsetGet();
        const double LastVisibleLine  = FirstVisibleLine + this->ScrollBar->ScrollViewzoneGet();
        
        double TargetOffset;
        if( Line < ceil( FirstVisibleLine ) ) {
                TargetOffset = Line;
        } else if( floor(LastVisibleLine) <= Line ) {
                TargetOffset = FirstVisibleLine-(LastVisibleLine-(double)Line)+1.0;
        } else {
                return;
        }
        
        this->ScrollBar->ScrollOffsetSet( TargetOffset );
        this->Invalidate( ValidityState::ParametersUpdated );
        
}

float w_textscrollbox::GetScrollOffsetLines()
{
        return this->ScrollBar->ScrollOffsetGet();

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

void w_textscrollbox::TextSet( const std::string& Text )
{
        this->TextBox->TextSet( Text );
        this->Invalidate( ValidityState::ParametersUpdated );
}

void w_textscrollbox::TextClear()
{
        this->TextBox->TextClear();
        this->Invalidate( ValidityState::ParametersUpdated );
}

void w_textscrollbox::TextUpdated()
{
        this->TextBox->TextUpdated();
}


std::string w_textscrollbox::TextGet()
{
        return this->TextBox->TextGet();
}

std::string* w_textscrollbox::TextGetRef()
{
        return this->TextBox->TextGetRef();
}

std::string w_textscrollbox::OriginalTextGet()
{
        return this->TextBox->OriginalTextGet();
}

std::string* w_textscrollbox::OriginalTextGetRef()
{
        return this->TextBox->OriginalTextGetRef();
}

std::vector<split_line> w_textscrollbox::LineMapGet() const
{
        return this->TextBox->LineMapGet();
}


