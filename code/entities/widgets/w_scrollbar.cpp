#include <entities\widgets\w_scrollbar.h>

#include <entities\ent_opengl.h>
#include <entities\ent_window.h>

#include <workers\widget_worker.h>
#include <workers\window_worker.h>
#include <utility\general.h>
#include <utility\text.h>
#include <utility\colors.h>
#include <utility\shortcuts.h>
#include <utility\widget.h>

#include <events\widget_event.h>

#include <font_symbols.h>

#include <cmath>
#include <stdio.h> //TODO: DEBUG.

void w_scrollbar::OnMousePressed( const int button )
{
        switch( button )
        {
                case GLFW_MOUSE_BUTTON_LEFT:
                {
                        this->bHasMouse = true;
                        break;
                }
        }
}

void w_scrollbar::OnMouseReleased( const int button, const bool )
{
        switch( button )
        {
                case GLFW_MOUSE_BUTTON_LEFT:
                {
                        this->bHasMouse = false;
                        break;
                }
        }
}

bool w_scrollbar::DoesHintCollide( point Position )
{
        const point HintMargin = pixel(32);
        const point Position2 = SecondPosition( this->Position, this->Size );
        const std::pair< point, point > Area = ApplyPadding( this->Position, Position2, -HintMargin );
        return DoesCollideRectangle( Position, Area.first, Area.second );
}

void w_scrollbar::OnTick()
{
        
        if( !this->bHasMouse ) {
                return;
        }
        
        const point Position2 = SecondPosition( Position, Size );
        const point MousePos = MousePosition();
        
        const fpoint FPosition = this->Position;
        const fpoint FPosition2 = Position2;
        
        const double ButtonSize      = pixel(Size.x.xpixels()).yratio();
        
        const double ScrollSpace     = FPosition.y-FPosition2.y-ButtonSize*2.0;
        const double ScrollbarHeight = ScrollSpace * this->ViewzoneRatio();
        const double InputSpace      = ScrollSpace - ScrollbarHeight;
        
        // Starting point of the scrollbar input zone.
        const double InputBegin  = FPosition.y-ButtonSize;
        const double LocalOffset = InputBegin-MousePos.y.yratio()-ScrollbarHeight/2.0;
        
        // -ScrollbarHeight/2 so mouse drags the middle of the scrollbar. ^
        
        this->ScrollOffsetSet( clamp( LocalOffset / InputSpace, 0.0, 1.0 ) * this->MaximumOffset() );
        this->ThrowEvent (
                std::shared_ptr<widget_event> (
                        new we_scrollsetratio( this->ScrollOffsetGet() )
                )
        );
        
}

void w_scrollbar::OnRefresh( ValidityState_t Reason )
{
        
        if( Reason & ValidityState::Resized ) {
                // Clamps offset to maximum.
                this->ScrollOffsetSet( this->ScrollOffsetGet() );
                // HACK: Hacky.
                if( !this->Parent.expired() ) {
                        this->Parent.lock()->Invalidate( ValidityState::ParametersUpdated );
                }
        }
        
        const rgb BackgroundColor = TheTheme.Background;
        const rgb ButtonColor = TheTheme.Primary2;
        
        const point Position2 = SecondPosition( Position, Size );
        const point PSize = point( this->Size.x.xpixels(), this->Size.y.ypixels() );

        const fpoint FPosition  = this->Position;
        const fpoint FPosition2 = Position2;
        
        gColor.Clear();
        gText.Clear();
        gPreviewColor.Clear();

        //:: Color.

        // Background.
        gColor.AddRectangle( colored_rectangle( Position, Position2, BackgroundColor ) );
        // Top button.
        gColor.AddRectangle(
                colored_rectangle(
                        Position,
                        point
                        (
                                Position2.x,
                                Position.y-PSize.x
                        ),
                        ButtonColor
                )
        );
        // Bottom button.
        gColor.AddRectangle(
                colored_rectangle(
                        point
                        (
                                Position.x,
                                Position2.y+PSize.x
                        ),
                        Position2,
                        ButtonColor
                )
        );
        // Scrollbar.

        //TODO: Do something about this mess. D:
        {
                const double ButtonHeight = pixel( Size.x.xpixels() ).yratio();
                
                const double ScrollSpace     = FPosition.y-FPosition2.y-ButtonHeight*2;
                const double InputSpace      = ScrollSpace * MaximumOffsetRatio();
                
                const double ScrollbarHeight  = ScrollSpace * ViewzoneRatio();
                const double ScrollSpaceBegin = FPosition.y-ButtonHeight;
                
                const double ScrollbarStart  = ScrollSpaceBegin - OffsetRatio() * InputSpace;
                const double ScrollbarEnd    = ScrollbarStart-ScrollbarHeight;

                gColor.AddRectangle
                (
                        colored_rectangle(
                                Position.x.xratio(),
                                ScrollbarStart,
                                
                                Position2.x.xratio(),
                                ScrollbarEnd,
                                
                                ButtonColor
                        )
                );
        }

        //:: Texture.
        // Top button.
        gText.AddText(
                std::string( 1, fsym::arrow_up ),
                PSize.x.xpixels(),
                point (
                        Position.x+(PSize.x/4),
                        Position.y
                ),
                color::white
        );
        // Bottom button.
        gText.AddText(
                std::string( 1, fsym::arrow_down ),
                PSize.x.xpixels(),
                point (
                        Position.x+(PSize.x/4),
                        Position2.y+PSize.x
                ),
                color::white
        );
        
        //:: Preview color.
        {
                
                const float PreviewOpacity = 0.9f;
                
                const float BeginX = Position2.x.xratio() - pixel(this->IdealPreviewWidth).xratio();
                const float EndX   = Position2.x.xratio();
                const float BeginY = Position.y.yratio();
                const float EndY   = Position2.y.yratio();
                
                const float SizeY       = Size.y.yratio();
                const float OffsetSizeY = SizeY*MaximumOffsetRatio();
                
                const float ScrollbarStart = BeginY - OffsetSizeY*OffsetRatio();
                const float ScrollbarEnd   = ScrollbarStart - SizeY*ViewzoneRatio();
                
                // Top background.
                gPreviewColor.AddRectangle(
                        colored_rectangle(
                                BeginX,
                                BeginY,
                                
                                EndX,
                                ScrollbarStart,
                                
                                rgba( BackgroundColor, PreviewOpacity )
                        )
                );
                
                // Viewzone.
                gPreviewColor.AddRectangle(
                        colored_rectangle(
                                BeginX,
                                ScrollbarStart,
                                EndX,
                                ScrollbarEnd,
                                rgba( ButtonColor, PreviewOpacity )
                        )
                );
                
                
                // Bottom background.
                gPreviewColor.AddRectangle(
                        colored_rectangle(
                                BeginX,
                                ScrollbarEnd,
                                EndX,
                                EndY,
                                rgba( BackgroundColor, PreviewOpacity )
                        )
                );
                
        }
        
        gColor.Update();
        gText.Update();
        gPreviewColor.Update();
}

void w_scrollbar::OnDraw()
{
        if( this->bHintFocused || this->bHasMouse ) {
                //:: Draw rectangles.
                this->gColor.Draw();
                
                //:: Draw text.
                this->gText.Draw();
        } else {
                this->gPreviewColor.Draw();
        }
}

/** Offset scrollbar offset by offset specified. */
void w_scrollbar::Offset( const double Offset )
{
        this->ScrollOffsetSet( ScrollOffsetGet()+Offset );
}

/** Offset scrollbar offset by viewzones specified. */
void w_scrollbar::OffsetByViewzone( double Ratio )
{
        this->ScrollOffsetSet( this->ScrollOffsetGet() + Ratio * this->ScrollViewzoneGet() );
        this->Invalidate( ValidityState::ParametersUpdated );
}

/** Offset scrollbar offset by a ratio of total scroll. */
void w_scrollbar::OffsetByRatio( double Ratio )
{
        this->ScrollOffsetSet( this->ScrollOffsetGet() + Ratio / this->MaximumOffset() );
        this->Invalidate( ValidityState::ParametersUpdated );
}

void w_scrollbar::ScrollLengthSet( const double ScrollLength )
{
        this->ScrollLength = ScrollLength < 0.0 ? 0.0 : ScrollLength;
        this->Invalidate( ValidityState::ParametersUpdated );
}

/** @note Offset gets clamped according to scroll length. */
void w_scrollbar::ScrollOffsetSet( const double ScrollOffset )
{
        this->ScrollOffset = clamp( ScrollOffset, 0.0, this->MaximumOffset() );
        this->Invalidate( ValidityState::ParametersUpdated );
}

void w_scrollbar::ScrollViewzoneSet( const double ScrollViewzone )
{
        this->ScrollViewzone = ScrollViewzone < 0.0 ? 0.0 : ScrollViewzone;
        this->Invalidate( ValidityState::ParametersUpdated );
}

double w_scrollbar::ScrollLengthGet() const
{
        return this->ScrollLength;
}

double w_scrollbar::ScrollOffsetGet() const
{
        return this->ScrollOffset;
}

double w_scrollbar::ScrollViewzoneGet() const
{
        if( this->ScrollViewzone > this->ScrollLengthGet() ) {
                return this->ScrollLengthGet();
        } else {
                return this->ScrollViewzone;
        }
}

/** Maximum offset possible before scrolling outside of the scroll range. */
double w_scrollbar::MaximumOffset() const
{
        const double Result = ScrollLength-ScrollViewzoneGet();
        return Result < 0.0 ? 0.0 : Result;
}

/** Maximum offset as a ratio between maximum offset and scroll length. */
double w_scrollbar::MaximumOffsetRatio() const
{
        if( ScrollLengthGet() == 0.0 ) {
                return 0.0;
        } else {
                return (ScrollLengthGet()-ScrollViewzoneGet())/ScrollLengthGet();
        }
}

/** Viewzone as a ratio of scroll length. */
double w_scrollbar::ViewzoneRatio() const
{
        if( ScrollLengthGet() == 0.0 ) {
                return 0.0;
        } else {
                return ScrollViewzoneGet()/this->ScrollLengthGet();
        }
}

double w_scrollbar::OffsetRatio() const
{
        if( this->MaximumOffset() == 0.0 ) {
                return 0.0;
        } else {
                return this->ScrollOffsetGet()/this->MaximumOffset();
        }
}

