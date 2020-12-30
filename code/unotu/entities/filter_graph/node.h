#ifndef __UNOTU_CODE_ENTITIES_FILTER_GRAPH_NODE_H_
#define __UNOTU_CODE_ENTITIES_FILTER_GRAPH_NODE_H_

#include <unotu\utility\deps\unotui_includes.h>
#include <unotu\utility\classes\graph.h>
#include <stdexcept>

#include <string>

namespace unotu
{

struct filter_node;

struct filter_node_parameter;
struct fnp_stringlist;
struct fnp_stringlist_any;
struct fnp_stringlist_every;
struct fnp_string;
struct fnp_boolean;

/** @brief Filter node parameter type.
 */
enum fnp_type
{
        none,
        stringlist,
        stringlist_any,
        stringlist_every,
        string,
        boolean
};

struct user_error_exception : public std::runtime_error
{
        
        user_error_exception( const std::string& What, filter_node* Source )
        : std::runtime_error( What.c_str() )
        {
                this->Source = Source;
        }
        
        filter_node* Source = nullptr;
        
};

struct filter_node
{
        using graph_node_type = graph_node< filter_node, void* >;
        
        filter_node() = default;
        filter_node(
                dpoint Position,
                std::string Name = "No name"
        ) : Position{Position}, Name{Name} {};
        
        std::weak_ptr<graph_node_type> ParentGraphNode;
        
        dpoint Position;
        std::string Name = "No name";
        
        graph_node_type* ParentNodeGet() const
        {
                return ParentGraphNode.lock().get();
        }
        
        virtual bool IsAcceptableParameter( filter_node_parameter* /*Parameter*/ ) const { return false; }
        std::size_t InputCountGet() const
        {
                graph_node_type* ParentNode = ParentNodeGet();
                assert( ParentNode, "'filter_node' with no graph node parent!" );
                
                return ParentNode->EdgesIncomingGet().size();
        }
        std::vector<filter_node_parameter*> InputParametersGet()
        {
                std::vector<filter_node_parameter*> Output;
                for( auto Node : ParentNodeGet()->ConnectedIncomingGet() ) {
                        auto& FilterNode = Node->DataReferenceGet();
                        Output += FilterNode.OutputGet();
                }
                return Output;
        }
        
        virtual fnp_type OutputTypeGet() const { return fnp_type::none; }
        virtual filter_node_parameter* OutputGet() { return nullptr; }
        
};

struct filter_node_parameter
{
        
        fnp_type Type = fnp_type::none;
        
        virtual std::string AsString() { return ""; }
        virtual std::string FriendlyName() const { return "Node Parameter"; }
        
};

struct fnp_stringlist: public filter_node_parameter
{
        fnp_stringlist()
        {
                this->Type = fnp_type::stringlist;
        }
        
        std::vector<std::string> List;
        std::string FriendlyName() const override
        {
                return "List";
        }
        std::string AsString() override
        {
                std::string Output;
                for( std::string& String : List ) {
                        if( Output.size() ) {
                                Output += "\n";
                        }
                        Output += String;
                }
                return Output;
        }
};

struct fnp_stringlist_any: public fnp_stringlist
{
        fnp_stringlist_any()
        {
                this->Type = fnp_type::stringlist_any;
        }
        
        std::string FriendlyName() const override
        {
                return "Text";
        }
        std::string AsString() override
        {
                if( List.size() ) {
                        return List[0];
                } else {
                        return "";
                }
        }
};

struct fnp_stringlist_every: public fnp_stringlist
{
        fnp_stringlist_every()
        {
                this->Type = fnp_type::stringlist_every;
        }
        
        std::string FriendlyName() const override
        {
                return "Text";
        }
};

struct fnp_string: public filter_node_parameter
{
        fnp_string()
        {
                this->Type = fnp_type::string;
        }
        
        std::string String;
        
        std::string FriendlyName() const override
        {
                return "Text";
        }
        std::string AsString() override
        {
                return String;
        }
};

struct fnp_boolean: public filter_node_parameter
{
        fnp_boolean()
        {
                this->Type = fnp_type::boolean;
        }
        
        bool Boolean;
        std::string FriendlyName() const override
        {
                return "Checkmark";
        }
        std::string AsString() override
        {
                return Boolean ? "true" : "false";
        }
};

// ---------------------------------------------------------------------------

struct fn_out: public filter_node
{
        bool IsAcceptableParameter( filter_node_parameter* Parameter ) const override
        {
                return Parameter->Type == fnp_type::boolean && InputCountGet() < 1;
        }
        fnp_type OutputTypeGet() const override
        {
                return fnp_type::boolean;
        }
        filter_node_parameter* OutputGet() override
        {
                auto Parameters = this->InputParametersGet();
                throw user_error_exception( "OUT requires a boolean parameter.", this );
                auto Parameter = Parameters[0];
                auto BoolParameter = dynamic_cast<fnp_boolean*>(Parameter);
                assert( BoolParameter, "The only 'OUT' node parameter should be a boolean parameter." );
        }
};

} // namespace unotu

#endif
