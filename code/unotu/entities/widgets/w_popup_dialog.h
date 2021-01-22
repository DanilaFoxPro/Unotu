#ifndef __UNOTU_CODE_ENTITIES_WIDGETS_W_POPUP_DIALOG_H_
#define __UNOTU_CODE_ENTITIES_WIDGETS_W_POPUP_DIALOG_H_

#include <unotu\utility\deps\unotui_includes.h>

namespace unotu {
        
        struct w_popup_dialog: public unotui::widget
        {
                w_popup_dialog( const std::string Text, bool bHasCancelButton = true );
                void PostConstruct() override;
                
                bool bHasCancelButton = true;
                bool bAutoClose       = true;
                
                std::shared_ptr<unotui::w_verticalbox> Container;
                std::shared_ptr<unotui::w_textscrollbox> Text;
                std::shared_ptr<unotui::w_horizontalbox> ButtonContainer;
                std::shared_ptr<unotui::w_button> ButtonOkay;
                std::shared_ptr<unotui::w_button> ButtonCancel;
                
                unotui::color_geometry gColor;
                
                enum action {
                        Okay = -10, // Safeguard against additional buttons with same actions.
                        Cancel,
                };
                
                void OnRefresh( unotui::ValidityState_t ) override;
                void OnDraw();
                void OnEvent( std::shared_ptr<unotui::widget_event> ) override;
                
                bool DoesCollide( unotui::point ) override;
                
        };
        
} // namespace unotu

#endif
