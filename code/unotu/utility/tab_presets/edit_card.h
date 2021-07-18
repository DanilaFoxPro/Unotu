#ifndef __UNOTU_UTILITY_TAB_PRESETS_EDIT_CARD_H_
#define __UNOTU_UTILITY_TAB_PRESETS_EDIT_CARD_H_

#include <unotu/utility/deps/unotui_includes.h>

#include <unotu/utility/classes/card.h>

namespace unotu {

namespace TabPresets
{
struct edit_card : public unotui::w_tab
{
//:: Constructors.
        edit_card();
        edit_card( const indexed_card& );
        void CommonSetup();
        virtual void PostConstruct();
        ~edit_card();
//:: Widgets.
        std::shared_ptr<unotui::w_button> ConfirmButton;
        std::shared_ptr<unotui::w_button> CancelButton;
        
        std::shared_ptr<unotui::w_editabletext> CardTitle;
        std::shared_ptr<unotui::w_editabletextbox> CardContent;
//:: Data.
        bool bNewCard = true;
        int CardID = -1;
        
        bool bConfirmed = false;
//:: Functions.
        void SaveCard();
//:: Overrides.
        virtual void OnRefresh( unotui::ValidityState_t );
        virtual void OnEvent( std::shared_ptr<unotui::widget_event> );
};
}// namespace TabPresets

} // namespace unotu

#endif
