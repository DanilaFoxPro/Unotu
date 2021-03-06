#ifndef __UNOTU_UTILITY_TAB_PRESETS_CARD_SEARCH_H_
#define __UNOTU_UTILITY_TAB_PRESETS_CARD_SEARCH_H_

#include <unotu\utility\deps\unotui_includes.h>

namespace unotu {

namespace TabPresets
{
        
struct card_search : public unotui::w_tab
{
//:: Constructors.
        card_search();
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
        
        void RemoveCardVisual( const int Identifier );
};

}

} // namespace unotu

#endif
