#ifndef __UNOTU_ENTITIES_EVENTS_WIDGET_H_
#define __UNOTU_ENTITIES_EVENTS_WIDGET_H_

#include <unotu/utility/deps/unotui_includes.h>

namespace unotu {

struct we_popup_action: public unotui::widget_event {};
struct we_popup_action_confirm: public we_popup_action {};
struct we_popup_action_cancel:  public we_popup_action {};


} // namespace unotu

#endif
