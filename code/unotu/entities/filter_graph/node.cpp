#include <unotu\entities\filter_graph\node.h>

namespace unotu
{

filter_node::~filter_node()
{
        delete CachedOutput;
}

filter_node_parameter* filter_node::CachedOutputSet( filter_node_parameter* Parameter )
{
        delete CachedOutput;
        CachedOutput = Parameter;
        return CachedOutput;
}

} // namespace unotu
