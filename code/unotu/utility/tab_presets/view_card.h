#ifndef __UNOTU_UTILITY_TAB_PRESETS_VIEW_CARD_H_
#define __UNOTU_UTILITY_TAB_PRESETS_VIEW_CARD_H_

#include <unotui\entities\widgets\w_tab.h>

#include <unotui\entities\widgets\w_textscrollbox.h>

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
