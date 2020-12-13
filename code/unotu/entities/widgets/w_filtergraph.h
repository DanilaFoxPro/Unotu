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
        
        const static int  PixelsPerGraphUnit = 512;
        const static uint NodeMaxWidth = 16;
        
        const double MinimumViewzone = 0.2;
        const double MaximumViewzone = 20.0;
        
        double Viewzone = 1.0;
        
        dpoint ViewOrigin = { -0.5, -0.5 };
        
        //
        
        bool bDragging = false;
        dpoint DragOrigin{};
        dpoint OldOrigin{};
        
        //
        
        using graph_type = node_graph< filter_node, void* >;
        using node_type  = graph_node< filter_node, void* >;
        
        node_graph< filter_node, void* > Graph;
        
        unotui::color_geometry gColor;
        unotui::text_geometry  gText;
        
        //:: Functions.
        
        w_filtergraph();
        
        void OnTick() override;
        void OnRefresh( unotui::ValidityState_t ) override;
        void OnDraw() override;
        
        //:: Input.
        
        void OnEvent( std::shared_ptr<unotui::widget_event> Event ) override;
        
        void OnMousePressed( const int Button ) override;
        void OnMouseReleased( const int Button, const bool bFocusingClick ) override;
        void OnKeyInput( const int& Action, const int& Key, const int& Modifiers ) override;
        
        //:: Helpers.
        
        point  ToRealPosition( const dpoint NodePosition );
        dpoint ToImaginaryPosition( const point Position );
        dpoint MousePositionInGraphCoordinates();
        
        void StartDragging();
        void StopDragging();
        
        void AdjustViewToDrag();
        
        dpoint DragOffset();
        dpoint OriginDifference();
        
        void ZoomBy( double Amount );
        
};

} // namespace unotu

#endif
