#include <unotu\utility\tab_presets\edit_card.h>

#include <unotu\utility\data.h>
#include <unotu\utility\tab_presets\card_search.h>

namespace unotu {

namespace TabPresets
{
        //:: Constructors.

EditCard::EditCard()
{
        this->CommonSetup();
        this->Title = "New Card";
}

EditCard::EditCard( const indexed_card& Card )
{
        this->CommonSetup();
        this->Title = "Editing Card";
        
        this->bNewCard = false;
        this->CardID = Card.ID;
        
        this->CardTitle->Text = Card.Title;
        this->CardContent->TextSet( Card.Content );
}

void EditCard::CommonSetup()
{
        this->Background = unotui::color::gray;
        
        this->CardTitle     = std::make_shared<unotui::w_editabletext>();
        this->CardContent   = std::make_shared<unotui::w_editabletextbox>();
}


void EditCard::PostConstruct()
{
        this->ConfirmButton = this->AddChild( new unotui::w_button( "Confirm", 1 ) );
        this->CancelButton  = this->AddChild( new unotui::w_button( "Cancel",  0 ) );
        
        // Already created in constructor via 'CommonSetup'.
        this->AddChild( this->CardTitle );
        this->AddChild( this->CardContent );
        
        this->CardContent->TextBox->FontSize = 32;
        
}

EditCard::~EditCard()
{
        if( this->bConfirmed ){
                this->SaveCard();
        }
}

        //:: Functions.
        
void EditCard::SaveCard()
{
        if( this->bNewCard ) {
                const card Card = card(
                        this->CardTitle->Text,
                        this->CardContent->OriginalTextGet()
                );
                
                AddCard( Card );
        } else {
                const indexed_card Card = indexed_card(
                        this->CardID,
                        this->CardTitle->Text,
                        this->CardContent->OriginalTextGet()
                );
                
                const bool ReplacedCard = ReplaceCard( Card );
                
                if( !ReplacedCard ) {
                        printf( "Failed to replace card ID %i!\n", Card.ID );
                }
        }
}

        //:: Overrides.

void EditCard::OnRefresh( unotui::ValidityState_t Reason )
{
        if( !( Reason & unotui::ValidityState::Resized ) ){
                return;
        }
        
        const fpoint Pixel = point( pixel(1) );
        
        const float TopMargin     = (ratio(1.0f)-pixel(25)).yratio();
        const float TextBoxMargin = TopMargin-Pixel.y*65;
        const float ButtonsMargin = pixel(50+20).yratio();
        
        const float XPadding = Pixel.x*30;
        const float ButtonsXPadding = XPadding;
        
        const float ButtonsWidth  = (1.0f-(XPadding*2+ButtonsXPadding))/2.0f;
        const float ButtonsHeight = Pixel.y*50;
        
        this->CardTitle->Position = point( XPadding, TopMargin );
        this->CardTitle->SetSecondPosition( point( ratio(1.0f)-XPadding, TextBoxMargin+Pixel.y*10 ) );
        
        this->CardContent->Position = point( XPadding, TextBoxMargin );
        this->CardContent->SetSecondPosition( point( ratio(1.0f)-XPadding, ButtonsMargin+Pixel.y*10 ) );
        
        this->CancelButton->Position = point( XPadding, ButtonsMargin );
        this->CancelButton->Size = point( ButtonsWidth, ButtonsHeight );
        
        this->ConfirmButton->Position = point( XPadding+ButtonsWidth+ButtonsXPadding, ButtonsMargin );
        this->ConfirmButton->Size = point( ButtonsWidth, ButtonsHeight );
        
}
       

} //namespace TabPresets

void TabPresets::EditCard::OnEvent( std::shared_ptr<unotui::widget_event> Event )
{
        auto* ClickEvent = dynamic_cast<unotui::we_click*>( Event.get() );
        if( ClickEvent ) {
                if( ClickEvent->ActionID == 1 ) {
                        this->bConfirmed = true;
                }
                unotui::TheWindowManager.Cur().SwitchTab( new TabPresets::CardSearch );
                return Event->Handle();
        }
}

} // namespace unotu
