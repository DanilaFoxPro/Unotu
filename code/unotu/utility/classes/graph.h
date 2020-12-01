#ifndef __UNOTU_CODE_UTILITY_CLASSES_GRAPH_H_
#define __UNOTU_CODE_UTILITY_CLASSES_GRAPH_H_

#include <memory>
#include <set>
#include <stdexcept>

  /////////////////////////////////
 /// Simple directional graph. ///
/////////////////////////////////

namespace unotu
{

//:: Interface.

template< typename node_data, typename edge_data = void>
class node_graph;

template< typename node_data, typename edge_data = void >
class graph_node;

template< typename node_data, typename edge_data = void>
class graph_edge;

template< typename node_data, typename edge_data >
class node_graph
{
        
        using graph_node_shared = std::shared_ptr<graph_node<node_data, edge_data>>;
        using graph_node_weak   = std::weak_ptr<graph_node<node_data, edge_data>>;
        
public:
        
        void NodeAdd( std::shared_ptr<graph_node<node_data, edge_data>> Node );
        void NodeRemove( std::shared_ptr<graph_node<node_data, edge_data>> Node );
        void NodeRemove( graph_node<node_data, edge_data>* Node );
        void NodeRemoveAll();
        
        std::set< graph_node_shared > NodeGetAll();
        
private:
        
        std::set< graph_node_shared > Nodes;
        
};

template< typename node_data, typename edge_data >
class graph_node
{
        
        using graph_ptr = node_graph<node_data, edge_data>*;
        
        using graph_node_shared = std::shared_ptr<graph_node<node_data, edge_data>>;
        using graph_node_weak   = std::weak_ptr<graph_node<node_data, edge_data>>;
        
        using graph_edge_shared = std::shared_ptr<graph_edge<node_data, edge_data>>;
        
public:
        
        graph_node( const node_data Data );
        ~graph_node();
        
        node_data DataGet();
        void DataSet( const node_data Data );
        
        node_graph<node_data, edge_data>* GraphGet();
        void GraphCut();
        
        std::set<graph_node_shared> ConnectedGet();
        std::set<graph_node_shared> ConnectedOutgoingGet();
        std::set<graph_node_shared> ConnectedIncomingGet();
        
        std::set<graph_edge_shared> EdgesGet( graph_node_shared Node );
        std::set<graph_edge_shared> EdgesGet();
        std::set<graph_edge_shared> EdgesOutgoingGet();
        std::set<graph_edge_shared> EdgesIncomingGet();
        
        bool ConnectTo( graph_node_shared Destination, const edge_data Data = {} );
        void Disconnect( graph_node_shared Node );
        
private:
        node_data Data;
        graph_ptr ParentGraph = nullptr;
        std::set< graph_edge_shared > Edges;
        
        friend node_graph<node_data, edge_data>;
        friend graph_edge<node_data, edge_data>;
        
};

template< typename node_data, typename edge_data >
class graph_edge
{
        using graph_node_shared = std::shared_ptr<graph_node<node_data, edge_data>>;
        using graph_node_weak   = std::weak_ptr<graph_node<node_data, edge_data>>;
        using graph_node_ptr    = graph_node<node_data, edge_data>*;
        
public:
        
        graph_edge(
                const graph_node_shared Source,
                const graph_node_shared Destination,
                const edge_data Data = {}
        );
        
        graph_node_weak SourceGet();
        graph_node_weak DestinationGet();
        graph_node_weak OtherGet( graph_node_shared Node );
        graph_node_weak OtherGet( graph_node_ptr Node );
        
        void Cut();
        
private:
        graph_node_weak Source;
        graph_node_weak Destination;
        edge_data Data;
};

        //:: Implementation.

        // Graph.

template<typename node_data, typename edge_data>
void node_graph<node_data, edge_data>::NodeAdd(
        std::shared_ptr<graph_node<node_data, edge_data> > Node
)
{
        // Already a part of this graph.
        if( Node->ParentGraph == this ) {
                return;
        }
        
        // Part of another graph.
        if( Node->ParentGraph ) {
                Node->ParentGraph->NodeRemove( Node );
        }
        
        this->Nodes.insert( Node );
        Node->ParentGraph = this;
        
}

template<typename node_data, typename edge_data>
void node_graph<node_data, edge_data>::NodeRemove(
        std::shared_ptr<graph_node<node_data, edge_data> > Node
)
{
        if( this->Nodes.erase( Node ) ) {
                Node->ParentGraph = nullptr;
        }
}

template<typename node_data, typename edge_data>
void node_graph<node_data, edge_data>::NodeRemove(
        graph_node<node_data, edge_data>* Node
)
{
        // Works because shared_ptr comparisons compare stored pointers, and std::set should respect that.
        return this->NodeRemove( std::shared_ptr<graph_node<node_data, edge_data>>( Node ) );
}


template<typename node_data, typename edge_data>
void node_graph<node_data, edge_data>::NodeRemoveAll()
{
        for( auto& Node : this->Nodes ) {
                Node->ParentGraph = nullptr;
        }
        
        this->Nodes.clear();
        
}

template<typename node_data, typename edge_data>
std::set<std::shared_ptr<graph_node<node_data, edge_data>>>
node_graph<node_data, edge_data>::NodeGetAll()
{
        return this->Nodes;
}


        // Graph node.

template<typename node_data, typename edge_data>
graph_node<node_data, edge_data>::graph_node(
        const node_data Data
)
{
        this->Data = Data;
}

template<typename node_data, typename edge_data>
graph_node<node_data, edge_data>::~graph_node()
{
        for( auto Edge : this->Edges ) {
                Edge->Cut();
        }
}


template<typename node_data, typename edge_data>
node_data graph_node<node_data, edge_data>::DataGet()
{
        return this->Data;
}

template<typename node_data, typename edge_data>
void graph_node<node_data, edge_data>::DataSet( const node_data Data )
{
        this->Data = Data;
}

template<typename node_data, typename edge_data>
node_graph<node_data, edge_data>* graph_node<node_data, edge_data>::GraphGet()
{
        return this->ParentGraph;
}

template<typename node_data, typename edge_data>
void graph_node<node_data, edge_data>::GraphCut()
{
        if( this->ParentGraph ) {
                this->ParentGraph->NodeRemove( this );
        }
}

template<typename node_data, typename edge_data>
std::set<std::shared_ptr<graph_node<node_data, edge_data>>>
graph_node<node_data, edge_data>::ConnectedGet()
{
        std::set<std::shared_ptr<graph_node<node_data, edge_data>>> Connected;
        for( auto Edge : this->Edges ) {
                std::shared_ptr<graph_node<node_data, edge_data>> OtherNode = Edge->GetOther( this ).lock();
                if( OtherNode ) {
                        Connected.push_back( OtherNode );
                }
        }
        return Connected;
}

template<typename node_data, typename edge_data>
std::set<std::shared_ptr<graph_node<node_data, edge_data>>>
graph_node<node_data, edge_data>::ConnectedIncomingGet()
{
        std::set<std::shared_ptr<graph_node<node_data, edge_data>>> Incoming;
        for( auto Edge : this->Edges ) {
                std::shared_ptr<graph_node<node_data, edge_data>> Source = Edge->GetSource().lock();
                std::shared_ptr<graph_node<node_data, edge_data>> Destination = Edge->GetDestination().lock();
                // This supports self-connection.
                if( Destination == this  ) {
                        Incoming.push_back( Source );
                }
        }
        Incoming.erase( {} );
        return Incoming;
}

template<typename node_data, typename edge_data>
std::set<std::shared_ptr<graph_node<node_data, edge_data>>>
graph_node<node_data, edge_data>::ConnectedOutgoingGet()
{
        std::set<std::shared_ptr<graph_node<node_data, edge_data>>> Outgoing;
        for( auto Edge : this->Edges ) {
                std::shared_ptr<graph_node<node_data, edge_data>> Source = Edge->GetSource().lock();
                std::shared_ptr<graph_node<node_data, edge_data>> Destination = Edge->GetDestination().lock();
                // This supports self-connection.
                if( Source == this  ) {
                        Outgoing.push_back( Destination );
                }
        }
        Outgoing.erase( {} );
        return Outgoing;
}

template<typename node_data, typename edge_data>
std::set<std::shared_ptr<graph_edge<node_data, edge_data>>>
graph_node<node_data, edge_data>::EdgesGet(
        graph_node_shared Node
)
{
        std::set<std::shared_ptr<graph_node<node_data, edge_data>>> Connected;
        for( auto Edge : this->Edges ) {
                std::shared_ptr<graph_node<node_data, edge_data>> Source = Edge->GetSource().lock();
                std::shared_ptr<graph_node<node_data, edge_data>> Destination = Edge->GetDestination().lock();
                if( Source == Node || Destination == Node ) {
                        Connected.push_back( Destination );
                }
        }
        Connected.erase( {} );
        return Connected;
}

template<typename node_data, typename edge_data>
std::set<std::shared_ptr<graph_edge<node_data, edge_data>>>
graph_node<node_data, edge_data>::EdgesGet()
{
        return this->Edges;
}

template<typename node_data, typename edge_data>
std::set<std::shared_ptr<graph_edge<node_data, edge_data>>>
graph_node<node_data, edge_data>::EdgesIncomingGet()
{
        std::set<std::shared_ptr<graph_edge<node_data, edge_data>>> Incoming;
        for( auto Edge : this->Edges ) {
                const std::shared_ptr<graph_node<node_data, edge_data>> Destination = Edge->GetDestination().lock();
                if( Destination == this ) {
                        Incoming.push_back( Edge );
                }
        }
        Incoming.erase( {} );
        return Incoming;
}

template<typename node_data, typename edge_data>
std::set<std::shared_ptr<graph_edge<node_data, edge_data>>>
graph_node<node_data, edge_data>::EdgesOutgoingGet()
{
        std::set<std::shared_ptr<graph_edge<node_data, edge_data>>> Outgoing;
        for( auto Edge : this->Edges ) {
                const std::shared_ptr<graph_node<node_data, edge_data>> Source = Edge->GetSource().lock();
                if( Source == this ) {
                        Outgoing.push_back( Edge );
                }
        }
        Outgoing.erase( {} );
        return Outgoing;
}

template<typename node_data, typename edge_data>
bool graph_node<node_data, edge_data>::ConnectTo(
        std::shared_ptr<graph_node<node_data, edge_data>> Destination,
        const edge_data Data
)
{
        if( !Destination )
                return false;
        
        if( this->ParentGraph && Destination->ParentGraph == this->ParentGraph )
                return false;
        
        // Shared pointer to 'this' needed, so the edge has a connected weak pointer.
        std::shared_ptr< graph_node<node_data, edge_data> > SharedThis = ParentGraph.NodeGetShared(this);
        
        if( !SharedThis )
                return false;
        
        std::shared_ptr< graph_edge<node_data, edge_data> > Edge = std::make_shared<graph_edge<node_data, edge_data>>( SharedThis, Destination, Data );
        
        this->Edges.push_back( Edge );
        Destination->Edges.push_back( Edge );
        
        return true;
        
}



template<typename node_data, typename edge_data>
void graph_node<node_data, edge_data>::Disconnect(
        std::shared_ptr<graph_node<node_data, edge_data>> Node
)
{
        const auto RelevantEdges = this->EdgesGet( Node );
        for( auto Edge : RelevantEdges ) {
                Edge->Cut();
        }
}

        // Graph edge.

template<typename node_data, typename edge_data>
graph_edge<node_data, edge_data>::graph_edge(
        std::shared_ptr<graph_node<node_data, edge_data>> Source,
        std::shared_ptr<graph_node<node_data, edge_data>> Destination,
        const edge_data Data
)
{
        if( !Source || !Destination )
                throw std::invalid_argument("Tried to create an edge between nodes, but one of the passed nodes is null.");
        
        this->Source      = Source;
        this->Destination = Destination;
        this->Data        = Data;
}

template<typename node_data, typename edge_data>
std::weak_ptr<graph_node<node_data, edge_data>>
graph_edge<node_data, edge_data>::SourceGet()
{
        return this->Source;
}

template<typename node_data, typename edge_data>
std::weak_ptr<graph_node<node_data, edge_data>>
graph_edge<node_data, edge_data>::DestinationGet()
{
        return this->Destination;
}

template<typename node_data, typename edge_data>
std::weak_ptr<graph_node<node_data, edge_data>>
graph_edge<node_data, edge_data>::OtherGet(
        std::shared_ptr<graph_node<node_data, edge_data>> Node
)
{
        if( Node == Source.lock() ) {
                return Destination;
        } else {
                return Source;
        }
}

template<typename node_data, typename edge_data>
std::weak_ptr<graph_node<node_data, edge_data>>
graph_edge<node_data, edge_data>::OtherGet(
        graph_node<node_data, edge_data>* Node
)
{
        if( Node == Source.lock() ) {
                return Destination;
        } else {
                return Source;
        }
}

template<typename node_data, typename edge_data>
void
graph_edge<node_data, edge_data>::Cut()
{
        graph_node<node_data, edge_data>* LockSource      = this->Source.lock().get();
        graph_node<node_data, edge_data>* LockDestination = this->Destination.lock().get();
        
        if( LockSource )
                LockSource->Edges.erase( std::shared_ptr<graph_edge<node_data, edge_data>>(this) );
        
        if( LockDestination )
                LockDestination->Edges.erase( std::shared_ptr<graph_edge<node_data, edge_data>>(this) );
        
}

} // namespace unotu

#endif
