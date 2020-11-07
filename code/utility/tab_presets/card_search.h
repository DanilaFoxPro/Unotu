#ifndef __UNOTU_UTILITY_TAB_PRESETS_CARD_SEARCH_H_
#define __UNOTU_UTILITY_TAB_PRESETS_CARD_SEARCH_H_

#include <unotui\entities\widgets\w_tab.h>

#include <unotui\entities\widgets\w_horizontalbox.h>
#include <unotui\entities\widgets\w_editabletext.h>
#include <unotui\entities\widgets\w_button.h>
#include <unotui\entities\widgets\w_genericscrollbox.h>

namespace TabPresets
{
        
struct CardSearch : public w_tab
{
//:: Constructors.
        CardSearch();
        virtual void PostConstruct();
//:: Data.
        std::shared_ptr<w_horizontalbox> TopBox;
        
        std::shared_ptr<w_editabletext> SearchField;
        std::shared_ptr<w_button> AddCard;
        
        std::shared_ptr<w_genericscrollbox> Scrollbox;
//:: Overrides.
        virtual void OnRefresh( ValidityState_t );
        virtual void OnEvent( std::shared_ptr<widget_event> );
//:: Functions.
        void Search();
};

}

#endif
