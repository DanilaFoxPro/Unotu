#include <utility\tab_presets\card_search.h>

#include <entities\widgets\w_minicard.h>
#include <entities\ent_application.h>
#include <entities\ent_window.h>

#include <utility\tab_presets\edit_card.h>
#include <utility\widget.h>
#include <utility\shortcuts.h>
#include <utility\data.h>

namespace TabPresets
{

CardSearch::CardSearch()
{
        this->Title = "Card Search";
        this->Background = color::gray;
}

void CardSearch::PostConstruct()
{
        this->TopBox      = this->AddChild( new w_horizontalbox() );
        this->SearchField = this->TopBox->AddChild( new w_editabletext() );
        this->AddCard     = this->TopBox->AddChild( new w_button("+") );
        this->Scrollbox   = this->AddChild( new w_genericscrollbox() );
        
        this->TopBox->Weights = { 1.0f, 0.1f };
        this->TopBox->Padding = point( pixel(10), 0 );
        
        this->SearchField->OutlineColor = color::gray * 0.5f;
        
        this->Search();
        
}

void CardSearch::OnRefresh( ValidityState_t Reason )
{
        const fpoint FPosition  = this->Position;
        const fpoint FPosition2 = SecondPosition( this->Position, this->Size );
        
        const fpoint Pixel = pixel(1);
        const fpoint Padding = Pixel*16.0f;
        const float PaddedWidth = this->Size.x.xratio()-Padding.x*2.0f;
        
        const float SearchFieldHeight = Pixel.y*32.0f;
        
        this->TopBox->Position = SecondPosition( FPosition, Padding );
        this->TopBox->Size = point( PaddedWidth, SearchFieldHeight );
        
        this->Scrollbox->Position = point( Padding.x, FPosition.y-(Padding.y*3.0f+SearchFieldHeight) );
        this->Scrollbox->SetSecondPosition( SecondPosition( FPosition2, -Padding ) );
        
}

void CardSearch::OnEvent( std::shared_ptr<widget_event> Event )
{
        auto Thrower = Event->Thrower.lock();
        auto ClickEvent = dynamic_cast<we_click*>( Event.get() );
        if( ClickEvent ) {
                const auto MiniCard = dynamic_cast<w_minicard*>( Thrower.get() );
                if( Thrower == this->AddCard ) {
                        TheWindowManager.Cur().SwitchTab( new TabPresets::EditCard );
                } else if( MiniCard ) {
                        switch( ClickEvent->Button ) {
                                case GLFW_MOUSE_BUTTON_LEFT: {
                                        try {
                                                const indexed_card Card = GetCard( MiniCard->CardID );
                                                TheWindowManager.Cur().SwitchTab( new TabPresets::EditCard( Card ) );
                                        } catch( std::exception& Except ) {
                                                printf( "%s -- Error while trying to edit a card (ID %i): %s\n",
                                                        ClassName( *this ).c_str(),
                                                        MiniCard->CardID,
                                                        Except.what()
                                                );
                                        }
                                        break;
                                }
                                case GLFW_MOUSE_BUTTON_RIGHT: {
                                        RemoveCard( MiniCard->CardID );
                                        // TODO: Consider optimizing by removing only the removed card from the results.
                                        // FIXME: This search causes a crash.
                                        this->Search();
                                        break;
                                }
                        }
                }
                return Event->Handle();
        }
        
        auto TextUpdateEvent = dynamic_cast<we_textupdated*>( Event.get() );
        if( TextUpdateEvent ) {
                this->Search();
                return Event->Handle();
        }
}

//:: Functions.

void CardSearch::Search()
{
        std::vector<indexed_card> Results = SearchCards( this->SearchField->Text );
        
        this->Scrollbox->ClearItems();
        for( indexed_card Cur : Results ) {
                this->Scrollbox->AddItem(
                        std::make_shared<w_minicard>( Cur )
                );
        }
        
}


} // namespace TabPresets
