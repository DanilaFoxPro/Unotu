#ifndef __UNOTU_UTILITY_TAB_PRESETS_VIEW_CARD_H_
#define __UNOTU_UTILITY_TAB_PRESETS_VIEW_CARD_H_

#include <entities\widgets\w_tab.h>

#include <entities\widgets\w_textscrollbox.h>

namespace TabPresets
{

struct ViewCard : public w_tab
{
//:: Constructors
        virtual void PostConstruct();
        virtual ~ViewCard();
//:: Data.
        // TODO: Some kind of 'w_scrollable'.
};

}// namespace TabPresets

#endif
