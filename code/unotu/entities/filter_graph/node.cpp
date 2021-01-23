#include <unotu\entities\filter_graph\node.h>

#include <unotu\entities\widgets\w_filtergraph.h>

namespace unotu
{

using graph_node_type = graph_node< filter_node*, void* >;

filter_node::~filter_node()
{
        delete CachedOutput;
}

/** @brief Returns original node name. */
std::string filter_node::NameGet() const
{
        return this->Name;
}

/** @brief Returns node name split to fit node size. */
std::string filter_node::NameGetSplit() const
{
        const std::vector<unotui::split_line> Lines = unotui::SplitText(
                this->Name,
                std::size_t(w_filtergraph::NodeMaxWidth)
        );
        const std::string Assembled = AssembleText( this->Name, Lines );
        return Assembled;
}

/** @brief Returns 'split_lines' that signify relationship between
 *         original node name, and the split version.
 */
std::vector<unotui::split_line> filter_node::NameGetLines() const
{
        const std::vector<unotui::split_line> Lines = unotui::SplitText(
                this->Name,
                std::size_t(w_filtergraph::NodeMaxWidth)
        );
        return Lines;
}

/** @brief Sets the node name to string specified. */
void filter_node::NameSet( const std::string Name )
{
        this->Name = Name;
}

/** @brief Returns node size, in characters. */
ipoint filter_node::SizeGet() const
{
        std::vector<unotui::split_line> Lines = NameGetLines();
        
        const std::size_t NodeNameHeight = Lines.size();
        const std::size_t NodeNameWidth  = unotui::LongestLine( Lines );
        
        return ipoint( NodeNameWidth, NodeNameHeight );
        
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
