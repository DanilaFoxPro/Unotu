#ifndef __UNOTU_UTILITY_TAB_PRESETS_TEST_H_
#define __UNOTU_UTILITY_TAB_PRESETS_TEST_H_

#include <unotui\entities\widgets\w_tab.h>

#include <utility\deps\unotui_includes.h>

namespace TabPresets
{
	struct Test : public unotui::w_tab
	{
                Test();
		virtual void PostConstruct();
                virtual void OnEvent( std::shared_ptr<unotui::widget_event> );
	};
}// namespace TabPresets

#endif
