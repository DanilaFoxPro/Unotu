#include <unotu\entities\widgets\w_filtergraph.h>

namespace unotu
{
        
        w_filtergraph::w_filtergraph()
        {
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
                        }
                }
        }
        
        void w_filtergraph::OnMouseReleased( const int Button, const bool )
        {
                switch( Button ) {
                        case GLFW_MOUSE_BUTTON_MIDDLE: {
                                this->StopDragging();
                        }
                }
        }
        
        point w_filtergraph::ToRealPosition( const dpoint NodePosition )
        {
                const point WindowSize   = point(1.0f);
                const dpoint Ratio       = dpoint(WindowSize.xpixels(), WindowSize.ypixels())/w_filtergraph::PixelsPerGraphUnit;
                
                const dpoint ShiftedPosition = NodePosition - ViewOrigin;
                
                return point( ShiftedPosition / Ratio / this->Viewzone );
        }
        
        dpoint w_filtergraph::ToImaginaryPosition( const point Position )
        {
                const point WindowSize   = point(1.0f);
                const dpoint Ratio       = dpoint(WindowSize.xpixels(), WindowSize.ypixels())/w_filtergraph::PixelsPerGraphUnit;
                const dpoint ShiftedPosition = (dpoint)Position * Ratio * this->Viewzone;
                return ShiftedPosition - ViewOrigin;
        }
        
        void w_filtergraph::StartDragging()
        {
                
        }
        
        void w_filtergraph::StopDragging()
        {
                
        }
        
} // namespace unotu
