#include <unotu\entities\filter_graph\node.h>

namespace unotu
{

using graph_node_type = graph_node< filter_node*, void* >;

filter_node::~filter_node()
{
        delete CachedOutput;
}

graph_node_type* filter_node::ParentNodeGet() const
{
        return ParentGraphNode.lock().get();
}

std::size_t filter_node::InputCountGet() const
{
        graph_node_type* ParentNode = ParentNodeGet();
        assert( ParentNode, "'filter_node' with no graph node parent!" );
        
        return ParentNode->EdgesIncomingGet().size();
}

std::vector<filter_node_parameter*> filter_node::InputParametersGet()
{
        assert( ParentNodeGet(), "'filter_node' should contain a reference to its graph representation." );
        std::vector<filter_node_parameter*> Output;
        auto Connected = ParentNodeGet()->ConnectedIncomingGet();
        for( auto Node : Connected ) {
                auto& FilterNode = Node->DataReferenceGet();
                Output += FilterNode->OutputGet();
        }
        return Output;
}

filter_node_parameter* filter_node::CachedOutputSet( filter_node_parameter* Parameter )
{
        delete CachedOutput;
        CachedOutput = Parameter;
        return CachedOutput;
}

} // namespace unotu
