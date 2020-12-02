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
        std::shared_ptr<w_filtergraph> Graph = this->AddChild( new w_filtergraph() );
        Graph->Position = point(0, 1.0f);
        Graph->Size = point(1.0f, 1.0f);
}


} // namespace TabPresets

} // namespace unotu
