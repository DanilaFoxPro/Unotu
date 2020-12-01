#include <unotu\entities\widgets\w_filtergraph.h>

namespace unotu
{
        w_filtergraph::w_filtergraph()
        {
                Graph.NodeAdd(
                        std::make_shared<graph_node<filter_node, void*>>( filter_node() )
                );
                Graph.NodeAdd(
                        std::make_shared<graph_node<filter_node, void*>>( filter_node() )
                );
                Graph.NodeAdd(
                        std::make_shared<graph_node<filter_node, void*>>( filter_node() )
                );
        }
        
        void w_filtergraph::OnRefresh( unotui::ValidityState_t )
        {
                auto Nodes = Graph.NodeGetAll();
                
                printf( "%i nodes total!\n", (int)Nodes.size() );
                
                for( auto Node : Nodes ) {
                        printf( "%s\n", Node->DataGet().Name.c_str() );
                }
                
        }
} // namespace unotu
