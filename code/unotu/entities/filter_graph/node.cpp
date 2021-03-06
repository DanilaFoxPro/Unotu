#include <unotu\entities\filter_graph\node.h>

#include <unotu\entities\widgets\w_filtergraph.h>

namespace unotu
{

using graph_node_type = graph_node< filter_node*, void* >;

filter_node::~filter_node()
{
        delete CachedOutput;
}

        //:: Visuals.

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

/** @brief Check if the node has any error attached to it. */
bool filter_node::ErrorHas() const
{
        return this->Error.length() != 0;
}

/** @brief Get the error attached to this node, if any. */
std::string filter_node::ErrorGet() const
{
        const size_t ErrorTextMaxWidth = 50;
        return unotui::AssembleText( Error, unotui::SplitText( Error, ErrorTextMaxWidth ) );
}

/** @brief Attach a specific error message to this node.
 *  @note  Overwrites any previously attached error message.
 */
void filter_node::ErrorSet( const std::string Error )
{
        this->Error = Error;
}

/** @brief Clear the error message attached to this node, if any. */
void filter_node::ErrorClear()
{
        this->ErrorSet( "" );
}

/** @brief Returns node size, in characters. */
ipoint filter_node::SizeGet() const
{
        std::vector<unotui::split_line> Lines = NameGetLines();
        
        const std::size_t NodeNameHeight = Lines.size();
        const std::size_t NodeNameWidth  = unotui::LongestLine( Lines );
        
        return ipoint( NodeNameWidth, NodeNameHeight );
        
}

        //:: Execution.

void filter_node::InputAdd( const filter_node* Node )
{
        if( Node == nullptr ) {
                return;
        }
        
        if( this->IsAcceptableParameter( Node->OutputTypeGet() ) ) {
                
                auto SelfGraphNode  = this->ParentGraphNode.lock();
                auto OtherGraphNode = Node->ParentGraphNode.lock();
                
                assert( SelfGraphNode  != nullptr, "Self graph node shouldn't ever be null." );
                assert( OtherGraphNode != nullptr, "Other graph node shouldn't ever be null." );
                
                OtherGraphNode->ConnectTo( SelfGraphNode );
                
        } else {
                // Useful error messages. I should probably find a way for 'IsAcceptableParameter()'
                // to provide specific error messages for unacceptable parameters.
                // TODO: ^
                throw user_error_exception(
                        "This node doesn't accept parameter of this type, currently: " + std::to_string( Node->OutputTypeGet() ),
                        this
                );
        }
        
}

std::size_t filter_node::InputCountGet() const
{
        graph_node_type* ParentNode = GraphNodeGet();
        
        return ParentNode->EdgesIncomingGet().size();
}

std::vector<filter_node_parameter*> filter_node::InputParametersGet()
{
        std::vector<filter_node_parameter*> Output;
        auto Connected = GraphNodeGet()->ConnectedIncomingGet();
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

        //:: Graph.

graph_node_type* filter_node::GraphNodeGet() const
{
        graph_node_type* const GraphNode = ParentGraphNode.lock().get();
        
        assert( GraphNode, "'filter_node' with no graph node parent!" );
        
        return GraphNode;
}

} // namespace unotu
