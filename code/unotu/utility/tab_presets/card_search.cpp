#include <unotu\utility\tab_presets\card_search.h>

#include <unotu\utility\deps\unotui_includes.h>

#include <unotu\entities\widgets\w_minicard.h>
#include <unotu\entities\ent_application.h>

#include <unotu\utility\tab_presets\edit_card.h>
#include <unotu\utility\data.h>

namespace unotu {

namespace TabPresets
{

card_search::card_search()
{
        this->Title = "Card Search";
        this->Background = unotui::TheTheme.Primary;
}

void card_search::PostConstruct()
{
        this->TopBox      = this->AddChild( new unotui::w_horizontalbox() );
        this->SearchField = this->TopBox->AddChild( new unotui::w_editabletext() );
        this->AddCard     = this->TopBox->AddChild( new unotui::w_button("+") );
        this->Scrollbox   = this->AddChild( new unotui::w_genericscrollbox() );
        
        this->TopBox->Weights = { 1.0f, 0.1f };
        this->TopBox->Padding = point( pixel(10), 0 );
        
        this->SearchField->OutlineColor = unotui::color::gray * 0.5f;
        
        this->Scrollbox->ItemHeight = pixel(48) + ratio(0.2);
        
        this->Search();
        
}

void card_search::OnRefresh( unotui::ValidityState_t Reason )
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

void card_search::OnEvent( std::shared_ptr<unotui::widget_event> Event )
{
        auto Thrower = Event->Thrower.lock();
        auto ClickEvent = dynamic_cast<unotui::we_click*>( Event.get() );
        if( ClickEvent ) {
                const auto MiniCard = dynamic_cast<w_minicard*>( Thrower.get() );
                if( Thrower == this->AddCard ) {
                        unotui::TheWindowManager.Cur().SwitchTab( new TabPresets::edit_card );
                } else if( MiniCard ) {
                        switch( ClickEvent->Button ) {
                                case GLFW_MOUSE_BUTTON_LEFT: {
                                        try {
                                                const indexed_card Card = GetCard( MiniCard->CardID );
                                                unotui::TheWindowManager.Cur().SwitchTab( new TabPresets::edit_card( Card ) );
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
                                        // Optimization. Doesn't refresh the search, only removes the card visual.
                                        this->RemoveCardVisual( MiniCard->CardID );
                                        break;
                                }
                        }
                }
                return Event->Handle();
        }
        
        auto TextUpdateEvent = dynamic_cast<unotui::we_textupdated*>( Event.get() );
        if( TextUpdateEvent ) {
                this->Search();
                return Event->Handle();
        }
}

//:: Functions.

void card_search::Search()
{
        std::vector<indexed_card> Results = SearchCards( this->SearchField->Text );
        
        this->Scrollbox->ClearItems();
        for( indexed_card Cur : Results ) {
                std::shared_ptr<unotu::w_minicard> Card = std::make_shared<w_minicard>( Cur );
                Card->bEnforceMinimumSize = false;
                this->Scrollbox->AddItem( Card );
        }
        
}

/** Removes card from search results, by Identifier. Visual only, doesn't touch the database. */
void card_search::RemoveCardVisual(const int Identifier)
{
        auto& Items = this->Scrollbox->Items;
        for( std::size_t i = 0; i < Items.size(); i++ ) {
                const unotu::w_minicard* Card = dynamic_cast<unotu::w_minicard*>(Items[i].get());
                if( Card && Card->CardID == Identifier ) {
                        Items.erase( Items.begin()+i );
                        this->Scrollbox->Invalidate( unotui::ValidityState::ParametersUpdated );
                        return;
                }
        }
}


} // namespace TabPresets

} // namespace unotu
