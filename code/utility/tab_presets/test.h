#ifndef __UNOTU_UTILITY_TAB_PRESETS_TEST_H_
#define __UNOTU_UTILITY_TAB_PRESETS_TEST_H_

#include <unotui\entities\widgets\w_tab.h>

namespace TabPresets
{
	struct Test : public w_tab
	{
                Test();
		virtual void PostConstruct();
                virtual void OnEvent( std::shared_ptr<widget_event> );
	};
}// namespace TabPresets

#endif
