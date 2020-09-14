#include <utility\tab_presets\edit_card.h>

#include <entities\widgets\w_debugoverlay.h>

#include <utility\data.h>
#include <utility\colors.h>

#include <utility\tab_presets\card_search.h>


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
        this->CardContent->SetText( Card.Content );
}

void EditCard::CommonSetup()
{
        this->Background = color::gray;
        
        this->CardTitle     = std::make_shared<w_editabletext>();
        this->CardContent   = std::make_shared<w_editabletextbox>();
}


void EditCard::PostConstruct()
{
        this->ConfirmButton = this->AddChild( new w_button( "Confirm", 1 ) );
        this->CancelButton  = this->AddChild( new w_button( "Cancel",  0 ) );
        
        // Already created in constructor via 'CommonSetup'.
        this->AddChild( this->CardTitle );
        this->AddChild( this->CardContent );
        
        this->CardContent->TextBox->FontSize = 32;
        
        this->AddChild( new w_debugoverlay() );
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
                        this->CardContent->GetOriginalText()
                );
                
                AddCard( Card );
        } else {
                const indexed_card Card = indexed_card(
                        this->CardID,
                        this->CardTitle->Text,
                        this->CardContent->GetOriginalText()
                );
                
                const bool ReplacedCard = ReplaceCard( Card );
                
                if( !ReplacedCard ) {
                        printf( "Failed to replace card ID %i!\n", Card.ID );
                }
        }
}

        //:: Overrides.

void EditCard::OnRefresh( ValidityState_t Reason )
{
        if( !( Reason & ValidityState::Resized ) ){
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

void TabPresets::EditCard::OnEvent( std::shared_ptr<widget_event> Event )
{
        auto* ClickEvent = dynamic_cast<we_click*>( Event.get() );
        if( ClickEvent ) {
                if( ClickEvent->ActionID == 1 ) {
                        this->bConfirmed = true;
                }
                TheWindowManager.Cur().SwitchTab( new TabPresets::CardSearch );
                return Event->Handle();
        }
}
