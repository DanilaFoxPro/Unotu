#ifndef __UNOTU_UTILITY_TAB_PRESETS_VIEW_CARD_H_
#define __UNOTU_UTILITY_TAB_PRESETS_VIEW_CARD_H_

#include <unotu/utility/deps/unotui_includes.h>

namespace unotu {

namespace TabPresets
{

struct view_card : public unotui::w_tab
{
//:: Constructors
        virtual void PostConstruct();
        virtual ~view_card();
//:: Data.
        // TODO: Some kind of 'w_scrollable'.
};

}// namespace TabPresets

} // namespace unotu

#endif
