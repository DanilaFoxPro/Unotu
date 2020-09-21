#include <utility\tab_presets\chat.h>

#include <workers\widget_worker.h>

namespace TabPresets
{
    
Chat::Chat( bool Server )
{
        this->Title = "Chat";
        this->bIsServer = Server;
}

void Chat::PostConstruct()
{
        this->OutputLog = this->AddChild( new w_textscrollbox() );
        this->InputField = this->AddChild( new w_editabletext() );
        
        this->InputField->bCommitOnEnter = true;
        
        this->ChatConnection = std::unique_ptr<ent_chatconnection>( new ent_chatconnection() );

        this->ChatConnection->AddListener( WidgetByPointer( this ) );

}

void Chat::OnTick()
{
        if( this->ChatConnection ) {
                this->ChatConnection->OnTick();
        }
}


void Chat::OnRefresh( ValidityState_t )
{
        
        const int InputBoxHeight = 32;

        this->OutputLog->Position = point( 0.0f, 1.0f );
        this->OutputLog->SetSecondPosition( point( 1.0f, InputBoxHeight ) );

        this->InputField->Position = point( 0.0f, InputBoxHeight );
        this->InputField->SetSecondPosition( point( 1.0f, 0.0f ) );

}

void Chat::OnEvent( std::shared_ptr<widget_event> Event )
{
        we_textcommit* TextCommit = dynamic_cast<we_textcommit*>( Event.get() );
        if( TextCommit ){
                // TODO: Send message.
                this->ChatConnection->LogLine( "<Asshat> " + *TextCommit->Text );
                this->OutputLog->ScrollToBottom();
                return Event->Handle();
        }
        we_logupdated* LogUpdated = dynamic_cast<we_logupdated*>( Event.get() );
        if( LogUpdated ){
                if( !this->ChatConnection ){
                        printf( "Chat connection null!\n" );
                        return;
                }
                this->OutputLog->TextBox->SetText( this->ChatConnection->GetLog() );
                this->OutputLog->Invalidate();
                return Event->Handle();
        }
}

#include <entities\ent_window.h>

void Chat::OnMousePressed( const int Button )
{
        if( Button == GLFW_MOUSE_BUTTON_LEFT ) {
                SetKeyboardFocus( std::weak_ptr<widget>( this->InputField ) );
        } else if( Button == GLFW_MOUSE_BUTTON_RIGHT ) {
                TheWindowManager.Cur().SwitchTab( new Chat( this->bIsServer ) );
        }
}

Chat::~Chat()
{
        printf( "Chat destroyed!\n" );
}
}// namespace TabPresets
