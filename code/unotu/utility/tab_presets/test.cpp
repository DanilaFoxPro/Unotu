#include <unotu\utility\tab_presets\test.h>

#include <unotu\entities\widgets\w_minicard.h>
#include <unotu\utility\tab_presets\edit_card.h>
#include <unotu\utility\tab_presets\card_search.h>

namespace unotu {

namespace TabPresets
{

test::test()
{
        this->Title = "Testing";
}


void test::PostConstruct()
{
        
        const std::vector<std::string> Options = {
                "Edit Card Tab",
                "New Window",
                "Card Search",
                "Print Hierarchy",
                "Filter Editor"
        };
        
        std::shared_ptr<unotui::w_horizontalbox> HorizontalBox = this->AddChild( new unotui::w_horizontalbox() );
        HorizontalBox->Position = point( 0.1f, ratio(1.0f)-pixel(30) );
        HorizontalBox->Size = point( 0.8f, 32 );
        
        for( std::size_t i = 0; i < Options.size(); i++ ) {
                HorizontalBox->AddChild(
                        new unotui::w_button(
                                Options[i],
                                (int)i
                        )
                );
        }
        HorizontalBox->Weights = { 1.25f, 1.0f, 1.0f, 1.3f, 1.2f };
        HorizontalBox->Padding = point( 5, 0 );
        
        const std::string Text =
        "Fancy test of fancy text.\nNewline.\n\nC-c-combo.\nSome long string so autosplit can be tested. "
        "Have you ever wondered about the meaning of life? I'm wondering what meaning means. "
        "Is it value? From whos point of view? Meaning can also mean intention; 'what do you mean?' "
        "So whos intention? We waren't intending to be alive, since before being alive we didn't think "
        "and there cannot be intention without thinking. Although many things can be argued: "
        "did we think before being alive? What does it mean to be alive? Perhaps life is a continous process, "
        "in which case the intention was there for ages. Then, was the first lifeform indending to be alive? "
        "You can say some being created life. That only distances the answer, who created the being? "
        "And if the being was always there, why we couldn't have always existed? What is existance? Shit.";
        
        auto Buffer = this->AddChild(
                new unotui::w_buffer(
                        point( pixel(20), ratio(1.0f)-pixel(160) ),
                        point( 0.7f, 0.7f )
                )
        );
        
        Buffer->AddChild(
                new w_minicard(
                        point( pixel(50), ratio(1.0f)-pixel(200) ),
                        "Title",
                        Text,
                        point( 0.6f, 0.6f )
                )
        );
        
}

void test::OnEvent ( std::shared_ptr<unotui::widget_event> Event )
{
        auto* ClickEvent = dynamic_cast<unotui::we_click*>( Event.get() );
        if( ClickEvent ) {
                switch( ClickEvent->ActionID ) {
                        case 0: {
                                unotui::TheWindowManager.Cur().SwitchTab( new TabPresets::edit_card );
                                break;
                        }
                        case 1: {
                                unotui::CreateWindow();
                                break;
                        }
                        case 2: {
                                unotui::TheWindowManager.Cur().SwitchTab( new TabPresets::card_search );
                                break;
                        }
                        case 3: {
                                printf( "--===<<< Widget Hierarchy >>>===--\n\n" );
                                unotui::PrintWidgetHierarchy( unotui::TheWindowManager.Cur().ActiveTab.Widgets );
                                printf( "\n--===<<< End >>>===--\n" );
                        }
                }
                return Event->Handle();
        }
}

}// namespace TabPresets

} // namespace unotu
