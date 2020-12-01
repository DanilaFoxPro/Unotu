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
        
        void w_filtergraph::OnRefresh( unotui::ValidityState_t )
        {
                auto Nodes = Graph.NodeGetAll();
                
                gText.Clear();
                gColor.Clear();
                
                printf( "%i nodes total!\n", (int)Nodes.size() );
                
                for( auto Node : Nodes ) {
                        const filter_node Data = Node->DataGet();
                        gText.AddText(
                                Data.Name,
                                32,
                                Data.Position
                        );
                }
                
                gText.Update();
                
        }
        
        void w_filtergraph::OnDraw()
        {
                gText.Draw();
        }
} // namespace unotu
