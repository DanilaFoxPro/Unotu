#ifndef __UNOTU_UTILITY_TAB_PRESETS_CARD_SEARCH_H_
#define __UNOTU_UTILITY_TAB_PRESETS_CARD_SEARCH_H_

#include <unotu\utility\deps\unotui_includes.h>

namespace TabPresets
{
        
struct CardSearch : public unotui::w_tab
{
//:: Constructors.
        CardSearch();
        virtual void PostConstruct();
//:: Data.
        std::shared_ptr<unotui::w_horizontalbox> TopBox;
        
        std::shared_ptr<unotui::w_editabletext> SearchField;
        std::shared_ptr<unotui::w_button> AddCard;
        
        std::shared_ptr<unotui::w_genericscrollbox> Scrollbox;
//:: Overrides.
        virtual void OnRefresh( unotui::ValidityState_t );
        virtual void OnEvent( std::shared_ptr<unotui::widget_event> );
//:: Functions.
        void Search();
};

}

#endif
