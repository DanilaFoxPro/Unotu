#ifndef __UNOTU_CODE_ENTITIES_WIDGETS_W_FILTERGRAPH_H_
#define __UNOTU_CODE_ENTITIES_WIDGETS_W_FILTERGRAPH_H_

#include <unotu\utility\deps\unotui_includes.h>

#include <unotu\utility\classes\graph.h>

namespace unotu
{

struct w_filtergraph : public unotui::widget
{
        const double MinimumViewzone = 0.2;
        const double MaximumViewzone = 20.0;
        
        double Viewzone = 1.0;
        
        std::pair<double, double> Position = { 0.0, 0.0 };
        
        node_graph< int, int > Graph;
};

} // namespace unotu

#endif
