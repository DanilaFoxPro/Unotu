#include <utility\tab_presets\chat_select.h>

#include <entities\ent_window.h>
#include <utility\tab_presets\chat.h>

namespace TabPresets
{

ChatSelect::ChatSelect()
{
        this->Title = "Chat Launch";
}


void ChatSelect::PostConstruct()
{
        this->Text = this->AddChild( new w_text( point(), "Choose_" ) );
        
        this->ServerButton = this->AddChild( new w_button( "Launch Server", 0 ) );
        this->ClientButton = this->AddChild( new w_button( "Launch Client", 1 ) );
}

void ChatSelect::OnRefresh( ValidityState_t Reason )
{
        
        if( !( Reason & ValidityState::Resized ) ){
                return;
        }
        
        const fpoint Pixel = point(pixel(1));
        
        const float TopMargin     = 0.9f;
        const float ButtonMargin1 = 0.6f;
        const float ButtonMargin2 = 0.4f;
        
        const float XPadding      = 0.1f;
        
        const int   TextFontSize  = 0.22f/Pixel.y;
        const float ButtonWidth   = 1.0f-XPadding*2.0f;
        const float ButtonHeight  = 0.1f;
        
        this->Text->Position  = point( XPadding, TopMargin );
        this->Text->font_size = TextFontSize;
        
        this->ServerButton->Position = point( XPadding, ButtonMargin1 );
        this->ServerButton->Size     = point( ButtonWidth, ButtonHeight );
        
        this->ClientButton->Position = point( XPadding, ButtonMargin2 );
        this->ClientButton->Size     = point( ButtonWidth, ButtonHeight );
        
}


void ChatSelect::OnEvent( std::shared_ptr<widget_event> Event )
{
        we_click* ClickEvent = dynamic_cast<we_click*>( Event.get() );
        if( ClickEvent ) {
                switch( ClickEvent->ActionID ) {
                        case 0:
                        {
                                TheWindowManager.Cur().SwitchTab( new TabPresets::Chat( true ) );
                                break;
                        }
                        case 1:
                        {
                                TheWindowManager.Cur().SwitchTab( new TabPresets::Chat( false ) );
                                break;
                        }
                }
                return Event->Handle();
        }
}


} //namespace ChatPresets
