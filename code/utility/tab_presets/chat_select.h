#ifndef __UNOTU_UTILITY_TAB_PRESETS_CHAT_SELECT_H_
#define __UNOTU_UTILITY_TAB_PRESETS_CHAT_SELECT_H_

#include <unotui\entities\widgets\w_tab.h>
#include <unotui\entities\widgets\w_button.h>
#include <unotui\entities\widgets\w_text.h>

namespace TabPresets
{

struct ChatSelect : w_tab
{
//:: Constructors.
        ChatSelect();
        virtual void PostConstruct();
//:: Overrides.
        virtual void OnRefresh( ValidityState_t );
        
        virtual void OnEvent( std::shared_ptr<widget_event> );
//:: Children.
        std::shared_ptr<w_text>   Text;
        std::shared_ptr<w_button> ServerButton;
        std::shared_ptr<w_button> ClientButton;
};

} // namespace TabPresets

#endif
