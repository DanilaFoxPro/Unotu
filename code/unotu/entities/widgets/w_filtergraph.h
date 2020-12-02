#ifndef __UNOTU_CODE_ENTITIES_WIDGETS_W_FILTERGRAPH_H_
#define __UNOTU_CODE_ENTITIES_WIDGETS_W_FILTERGRAPH_H_

#include <unotu\utility\deps\unotui_includes.h>

#include <unotu\utility\classes\graph.h>

namespace unotu
{

struct filter_node
{
        filter_node() = default;
        filter_node(
                dpoint Position,
                std::string Name = "No name"
        ) : Position{Position}, Name{Name} {};
        
        dpoint Position;
        std::string Name = "No name";
};

struct w_filtergraph : public unotui::widget
{
        
        //:: Data.
        
        const double MinimumViewzone = 0.2;
        const double MaximumViewzone = 20.0;
        
        double Viewzone = 1.0;
        
        dpoint ViewOrigin = { -0.5, -0.5 };
        
        node_graph< filter_node, void* > Graph;
        
        unotui::color_geometry gColor;
        unotui::text_geometry  gText;
        
        //:: Functions.
        
        w_filtergraph();
        
        void OnTick() override;
        void OnRefresh( unotui::ValidityState_t ) override;
        void OnDraw() override;
        
        //:: Input.
        
        void OnMousePressed( const int Button ) override;
        void OnMouseReleased( const int Button, const bool bFocusingClick ) override;
        
        //:: Helpers.
        
        point ToRealPosition( const dpoint NodePosition );
        
};

} // namespace unotu

#endif
