#ifndef __UNOTU_CODE_ENTITIES_WIDGETS_W_FILTERGRAPH_H_
#define __UNOTU_CODE_ENTITIES_WIDGETS_W_FILTERGRAPH_H_

#include <unotu\utility\deps\unotui_includes.h>

#include <unotu\utility\classes\graph.h>

namespace unotu
{

struct filter_node
{
        std::string Name = "No name";
};

struct w_filtergraph : public unotui::widget
{
        
        //:: Data.
        
        const double MinimumViewzone = 0.2;
        const double MaximumViewzone = 20.0;
        
        double Viewzone = 1.0;
        
        std::pair<double, double> ViewOrigin = { 0.0, 0.0 };
        
        node_graph< filter_node, void* > Graph;
        
        unotui::color_geometry gColor;
        unotui::text_geometry  gText;
        
        //:: Functions.
        
        w_filtergraph();
        
        void OnRefresh( unotui::ValidityState_t ) override;
        
};

} // namespace unotu

#endif
