#include <unotu/utility/tab_presets/filter_editor.h>

#include <unotu/entities/widgets/w_filtergraph.h>

namespace unotu {

namespace TabPresets {

filter_editor::filter_editor()
{
        this->Title = "Filter Editor";
}

void filter_editor::PostConstruct()
{
        this->AddChild( new w_filtergraph() );
}


} // namespace TabPresets

} // namespace unotu
