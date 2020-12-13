#include <unotu\entities\widgets\w_filtergraph.h>

namespace unotu
{
        
        w_filtergraph::w_filtergraph()
        {
                
                this->bKeyboardFocusable    = true;
                this->bAcceptExternalScroll = true;
                
                // Testing!
                this->bValidateOnRefresh = false;
                
                Graph.NodeAdd(
                        std::make_shared<graph_node<filter_node, void*>>( filter_node(
                                {0.0, 0.0},
                                "I'm the origin!"
                        ) )
                );
                
                std::vector< std::shared_ptr<w_filtergraph::node_type> > Nodez = {
                        std::make_shared<graph_node<filter_node, void*>>( filter_node({0.1, 0.5}) ),
                        std::make_shared<graph_node<filter_node, void*>>( filter_node({0.8, 0.6}) ),
                        std::make_shared<graph_node<filter_node, void*>>( filter_node(
                                {0.5, 0.8},
                                "OUT"
                        ) )
                };
                
                for( auto Node : Nodez ) {
                        Graph.NodeAdd( Node );
                }
                
                // BUG: Those node connections result in crashes on exit.
                Nodez[0]->ConnectTo( Nodez[2] );
                Nodez[1]->ConnectTo( Nodez[2] );
                
        }
        
        void w_filtergraph::OnTick()
        {
                if( this->bDragging ) {
                        this->AdjustViewToDrag();
                }
        }
        
        void w_filtergraph::OnRefresh( unotui::ValidityState_t )
        {
                auto Nodes = Graph.NodeGetAll();
                
                gText.Clear();
                gColor.Clear();
                
                const int TextSize = int( 32 / this->Viewzone );
                
                for( auto Node : Nodes ) {
                        const filter_node Data = Node->DataGet();
                        
                        const point RealPosition = this->ToRealPosition( Data.Position );
                        
                        const std::string& NodeName = Data.Name;
                        const auto NodeNameLines = unotui::SplitText( NodeName, std::size_t(w_filtergraph::NodeMaxWidth) );
                        
                        gText.AddText(
                                Data.Name,
                                NodeNameLines,
                                TextSize,
                                RealPosition,
                                unotui::color::black
                        );
                        
                        const std::size_t NodeNameHeight = NodeNameLines.size();
                        const std::size_t NodeNameWidth  = unotui::LongestLine( NodeNameLines );
                        
                        const unotui::rectangle Bounds = unotui::rectangle(
                                point(
                                        RealPosition.x,
                                        RealPosition.y
                                ),
                                point(
                                        RealPosition.x + pixel(TextSize*NodeNameWidth/2),
                                        RealPosition.y - pixel(TextSize*NodeNameHeight)
                                )
                        );
                        
                        gColor.AddRectangle( unotui::colored_rectangle (
                                Bounds,
                                unotui::color::light_sky_blue
                        ) );
                        
                        gColor.AddOutline(
                                Bounds,
                                int( -5/this->Viewzone ),
                                unotui::TheTheme.PrimaryBack,
                                unotui::TheTheme.PrimaryLit,
                                unotui::TheTheme.PrimaryLit,
                                unotui::TheTheme.PrimaryBack
                        );
                        
                        gColor.AddOutline(
                                unotui::colored_rectangle(
                                        Bounds,
                                        unotui::TheTheme.Primary
                                ),
                                int( 1/this->Viewzone )
                        );
                        
                        const fpoint NodeCenter = Bounds.Center();
                        
                        const auto ConnectedNodes = Node->ConnectedOutgoingGet();
                        for( auto ConnectedNode : ConnectedNodes ) {
                                const filter_node ConnectedData = ConnectedNode->DataGet();
                                const point ConnectedRealPosition = this->ToRealPosition( ConnectedData.Position );
                                
                                gColor.AddArrow(
                                        NodeCenter,
                                        ConnectedRealPosition,
                                        6 /this->Viewzone,
                                        16 /this->Viewzone,
                                        32/this->Viewzone,
                                        unotui::color::black
                                );
                                
                        }
                        
                        
                }
                
                gText.AddText(
                        std::to_string( ViewOrigin.x ) + ", " + std::to_string( ViewOrigin.y ),
                        32,
                        point(0, 32),
                        unotui::color::green*0.3f
                );
                
                // Draw arrow from graph origin to mouse position.
                gColor.AddArrow( ToRealPosition( dpoint(0) ), unotui::MousePosition(), 10, unotui::color::black );
                
                gText.Update();
                gColor.Update();
                
        }
        
        void w_filtergraph::OnDraw()
        {
                gColor.Draw();
                gText.Draw();
        }
        
        void w_filtergraph::OnEvent( std::shared_ptr<unotui::widget_event> Event )
        {
                auto ScrollLines = dynamic_cast<unotui::we_scrolllines*>(Event.get());
                
                if( ScrollLines ) {
                        this->ZoomBy( 0.2 * ScrollLines->Lines );
                        Event->Handle();
                }
                
                auto ScrollPages = dynamic_cast<unotui::we_scrollpages*>(Event.get());
                if( ScrollPages ) {
                        this->ZoomBy( 0.5 * ScrollPages->Pages );
                        Event->Handle();
                }
                
        }
        
        void w_filtergraph::OnMousePressed( const int Button )
        {
                
                const bool bShiftPressed = unotui::IsKeyPressed( GLFW_KEY_LEFT_SHIFT ) || unotui::IsKeyPressed( GLFW_KEY_RIGHT_SHIFT );
                
                switch( Button ) {
                        case GLFW_MOUSE_BUTTON_LEFT: {
                                if( bShiftPressed ) {
                                        this->StartDragging();
                                }
                                break;
                        }
                        case GLFW_MOUSE_BUTTON_MIDDLE: {
                                this->StartDragging();
                                break;
                        }
                }
        }
        
        void w_filtergraph::OnMouseReleased( const int Button, const bool )
        {
                switch( Button ) {
                        case GLFW_MOUSE_BUTTON_LEFT: {
                                this->StopDragging();
                        }
                        case GLFW_MOUSE_BUTTON_MIDDLE: {
                                this->StopDragging();
                                break;
                        }
                }
        }
        
        void w_filtergraph::OnKeyInput( const int& Action, const int& Key, const int& /*Modifiers*/ )
        {
                
                switch( Key ) {
                        case GLFW_KEY_HOME: {
                                
                                if( Action != GLFW_PRESS )
                                        break;
                                
                                this->ViewOrigin = {0.0, 0.0};
                                this->Invalidate( unotui::ValidityState::ParametersUpdated );
                                
                                break;
                        }
                        case GLFW_KEY_SPACE: {
                                if( Action == GLFW_PRESS ) {
                                        this->StartDragging();
                                } else if( Action == GLFW_RELEASE ) {
                                        this->StopDragging();
                                }
                        }
                }
        }
        
        point w_filtergraph::ToRealPosition( const dpoint NodePosition )
        {
                const point WindowSize   = point(1.0f);
                const dpoint Ratio       = dpoint(WindowSize.xpixels(), WindowSize.ypixels())/w_filtergraph::PixelsPerGraphUnit;
                
                dpoint ShiftedPosition;
                ShiftedPosition = NodePosition - ViewOrigin;
                
                return point( ShiftedPosition / Ratio / this->Viewzone );
        }
        
        dpoint w_filtergraph::ToImaginaryPosition( const point Position )
        {
                const point  WindowSize  = point(1.0f);
                const dpoint Ratio       = dpoint(WindowSize.xpixels(), WindowSize.ypixels())/w_filtergraph::PixelsPerGraphUnit;
                
                const dpoint ScaledPosition = (dpoint)Position * Ratio * this->Viewzone;
                
                return ScaledPosition - ViewOrigin;
        }
        
        dpoint w_filtergraph::MousePositionInGraphCoordinates()
        {
                return ToImaginaryPosition( unotui::MousePosition() );
        }
        
        void w_filtergraph::StartDragging()
        {
                this->bDragging = true;
                this->DragOrigin = MousePositionInGraphCoordinates();
                this->OldOrigin = this->ViewOrigin;
        }
        
        void w_filtergraph::StopDragging()
        {
                if( this->bDragging ) {
                        this->bDragging  = false;
                        this->AdjustViewToDrag();
                }
        }
        
        void w_filtergraph::AdjustViewToDrag()
        {
                this->ViewOrigin = this->OldOrigin - this->DragOffset();
                this->Invalidate( unotui::ValidityState::ParametersUpdated );
        }
        
        dpoint w_filtergraph::DragOffset()
        {
                const dpoint MousePosition = MousePositionInGraphCoordinates();
                /* Eeeeeeh.. Well. Some bullshit vector maths here. OriginDifference
                 * accounts for the difference between origin before the drag, and
                 * origin after the drag. This is needed because when you adjust the
                 * graph canvas according to mouse drag -- you move the mouse cursor
                 * relative to ViewOrigin. This in turn means that if you don't
                 * add the OriginDifference -- the cursor would 'move' further and
                 * futher as the time progresses thus creating velocity. I don't
                 * want any velocity here, so I add OriginDifference, thus
                 * stabilizing this mess.
                 */
                return MousePosition - this->DragOrigin + this->OriginDifference();
        }
        
        dpoint w_filtergraph::OriginDifference()
        {
                return this->ViewOrigin - this->OldOrigin;
        }
        
        void w_filtergraph::ZoomBy( double Amount )
        {
                const double NewViewzone = unotui::clamp( this->Viewzone - Amount, 0.2, 20.0 );
                
                // Somehow the dragging code also compensates for zoom offset.
                if( !bDragging ) {
                        this->StartDragging();
                        this->Viewzone = NewViewzone;
                        this->StopDragging();
                } else {
                        this->Viewzone = NewViewzone;
                }
                
                this->Invalidate( unotui::ValidityState::ParametersUpdated );
        }
        
} // namespace unotu
