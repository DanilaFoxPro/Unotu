#include <unotu\entities\widgets\w_filtergraph.h>

namespace unotu
{
        
        w_filtergraph::w_filtergraph()
        {
                
                this->bKeyboardFocusable = true;
                
                Graph.NodeAdd(
                        std::make_shared<graph_node<filter_node, void*>>( filter_node(
                                {0.0, 0.0},
                                "I'm the origin!"
                        ) )
                );
                Graph.NodeAdd(
                        std::make_shared<graph_node<filter_node, void*>>( filter_node({0.1, 0.5}) )
                );
                Graph.NodeAdd(
                        std::make_shared<graph_node<filter_node, void*>>( filter_node({0.8, 0.6}) )
                );
                Graph.NodeAdd(
                        std::make_shared<graph_node<filter_node, void*>>( filter_node(
                                {0.5, 0.8},
                                "OUT"
                        ) )
                );
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
                
                for( auto Node : Nodes ) {
                        const filter_node Data = Node->DataGet();
                        gText.AddText(
                                Data.Name,
                                32,
                                this->ToRealPosition( Data.Position )
                        );
                }
                
                gText.AddText(
                        std::to_string( ViewOrigin.x ) + ", " + std::to_string( ViewOrigin.y ),
                        32,
                        point(0, 32),
                        unotui::color::green*0.3f
                );
                
                gText.Update();
                
        }
        
        void w_filtergraph::OnDraw()
        {
                gText.Draw();
        }
        
        void w_filtergraph::OnMousePressed( const int Button )
        {
                switch( Button ) {
                        case GLFW_MOUSE_BUTTON_MIDDLE: {
                                this->StartDragging();
                                break;
                        }
                }
        }
        
        void w_filtergraph::OnMouseReleased( const int Button, const bool )
        {
                switch( Button ) {
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
        
} // namespace unotu
