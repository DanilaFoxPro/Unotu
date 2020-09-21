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

void w_scrollbar::OnMouseReleased( const int button )
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
        
        const ent_window& the_window = TheWindowManager.Cur();
        
        const point position2 = SecondPosition( Position, Size );
        const point mpos = MousePosition();
        
        //TODO: Organize this horror.
        
        const float scroll_space = ((Position.y-position2.y)-(Size.x*2)).ratio( the_window.y );
        const float scrollbar_height = scroll_space * ScrollViewzone;
        const float input_space = scroll_space - scrollbar_height;
        
        const point origin = // Starting point of the scrollbar input zone.
        point
        (
                Position.x,
                Position.y-Size.x-ratio( scrollbar_height/2.0f )
        );
        
        point local = Localize( origin, mpos );
        local.y = -local.y;
        
        this->ScrollOffset = clamp( ( local.y.ratio( the_window.y ) / input_space ), 0.0f, 1.0f );
        this->ThrowEvent
        (
                std::shared_ptr<widget_event>
                (
                        new we_scrollsetratio( ScrollOffset )
                )
        );
        this->Invalidate();
        
}

void w_scrollbar::OnRefresh( ValidityState_t )
{

        const ent_window& TheWindow = TheWindowManager.Cur();
        
        const rgb BackgroundColor = TheTheme.Background;
        const rgb ButtonColor = TheTheme.Primary2;

        const point Position2 = SecondPosition( Position, Size );
        const point PSize = point( this->Size.x.xpixels(), this->Size.y.ypixels() );

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
                const float ScrollSpace = ((Position.y-Position2.y)-(Size.x*2)).ratio( TheWindow.y );
                const float ScrollSpace2 = ScrollSpace * (1.0f-ScrollViewzone);
                const float ScrollbarHeight = ScrollSpace * ScrollViewzone;
                const float ScrollbarStart = (Position.y-Size.x).yratio() - ScrollOffset * ScrollSpace2;

                gColor.AddRectangle
                (
                        colored_rectangle(
                                Position.x.ratio( TheWindow.x ),
                                ScrollbarStart,
                                
                                Position2.x.ratio( TheWindow.x ),
                                ScrollbarStart-ScrollbarHeight,
                                
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
                
                const float SizeY  = Size.y.yratio();
                const float LowerSizeY = SizeY-SizeY*ScrollViewzone;
                
                const float ScrollbarStart = BeginY - LowerSizeY*ScrollOffset;
                const float ScrollbarEnd   = ScrollbarStart - SizeY*ScrollViewzone;
                
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
