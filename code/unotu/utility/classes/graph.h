#ifndef __UNOTU_CODE_UTILITY_CLASSES_GRAPH_H_
#define __UNOTU_CODE_UTILITY_CLASSES_GRAPH_H_

#include <memory>
#include <set>

  /////////////////////////////////
 /// Simple directional graph. ///
/////////////////////////////////

namespace unotu
{
        
        template< typename node_data, typename edge_data = void >
        class graph_node;
        
        template< typename node_data, typename edge_data = void>
        class node_graph;
        
        template< typename node_data, typename edge_data >
        class graph_edge
        {
                using graph_node_shared = std::shared_ptr<graph_node<node_data, edge_data>>;
                using graph_node_weak   = std::weak_ptr<graph_node<node_data, edge_data>>;
                
        public:
                
                graph_edge(
                        const graph_node_shared Source,
                        const graph_node_shared Destination
                );
                
                graph_node_weak SourceGet();
                graph_node_weak DestinationGet();
                void SourceSet( const graph_node_shared );
                void DestinationSet( const graph_node_shared );
        private:
                graph_node_weak Source;
                graph_node_weak Destination;
        };
        
        template< typename node_data, typename edge_data >
        class graph_node
        {
                
                using graph_node_shared = std::shared_ptr<graph_node<node_data, edge_data>>;
                using graph_node_weak   = std::weak_ptr<graph_node<node_data, edge_data>>;
                
                using graph_edge_shared = std::shared_ptr<graph_edge<node_data, edge_data>>;
                
        public:
                
                node_data DataGet();
                void DataSet( const node_data Data );
                
                node_graph<node_data, edge_data>* GraphGet();
                void GraphCut();
                
                std::set<graph_node_weak> ConnectedGet();
                std::set<graph_node_weak> ConnectedOutgoingGet();
                std::set<graph_node_weak> ConnectedIncomingGet();
                
                std::set<graph_edge_shared> EdgesGet( graph_node_shared Node );
                std::set<graph_edge_shared> EdgesGet();
                std::set<graph_edge_shared> EdgesOutgoingGet();
                std::set<graph_edge_shared> EdgesIncomingGet();
                
                bool ConnectTo( graph_node_shared Destination );
                void Disconnect( graph_node_shared Node );
                
        private:
                node_data Data;
                std::set< graph_edge_shared > Edges;
        };
        
        template< typename node_data, typename edge_data >
        class node_graph
        {
                
                using graph_node_shared = std::shared_ptr<graph_node<node_data, edge_data>>;
                using graph_node_weak   = std::weak_ptr<graph_node<node_data, edge_data>>;
                
        public:
                
                void NodeAdd( std::shared_ptr<graph_node<node_data, edge_data>> Node );
                void NodeRemove( std::shared_ptr<graph_node<node_data, edge_data>> Node );
                void NodeRemoveAll();
                
                std::set< graph_node_shared > NodeGetAll();
                
        private:
                
                std::set< graph_node_shared > Nodes;
                
        };
}

#endif
