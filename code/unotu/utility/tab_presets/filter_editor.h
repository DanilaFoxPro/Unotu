#ifndef __UNOTU_CODE_UNOTU_UTILITY_TAB_PRESETS_FILTER_EDITOR_H_
#define __UNOTU_CODE_UNOTU_UTILITY_TAB_PRESETS_FILTER_EDITOR_H_

#include <unotu\utility\deps\unotui_includes.h>

namespace unotu {

namespace TabPresets {

        struct filter_editor : public unotui::w_tab {
                filter_editor();
                void PostConstruct() override;
        };

}

} // namespace unotu

#endif
