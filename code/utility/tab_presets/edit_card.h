#ifndef __UNOTU_UTILITY_TAB_PRESETS_EDIT_CARD_H_
#define __UNOTU_UTILITY_TAB_PRESETS_EDIT_CARD_H_

#include <entities\widgets\w_tab.h>

#include <entities\widgets\w_button.h>
#include <entities\widgets\w_editabletext.h>
#include <entities\widgets\w_editabletextbox.h>
#include <utility\classes\card.h>

namespace TabPresets
{
struct EditCard : public w_tab
{
//:: Constructors.
        EditCard();
        EditCard( const indexed_card& );
        void CommonSetup();
        virtual void PostConstruct();
        ~EditCard();
//:: Widgets.
        std::shared_ptr<w_button> ConfirmButton;
        std::shared_ptr<w_button> CancelButton;
        
        std::shared_ptr<w_editabletext> CardTitle;
        std::shared_ptr<w_editabletextbox> CardContent;
//:: Data.
        bool bNewCard = true;
        int CardID = -1;
        
        bool bConfirmed = false;
//:: Functions.
        void SaveCard();
//:: Overrides.
        virtual void OnRefresh( ValidityState_t );
        virtual void OnEvent( std::shared_ptr<widget_event> );
};
}// namespace TabPresets

#endif
