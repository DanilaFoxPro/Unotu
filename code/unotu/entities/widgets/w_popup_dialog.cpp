#include <unotu\entities\widgets\w_popup_dialog.h>

#include <unotu\entities\events\widget.h>

namespace unotu {

w_popup_dialog::w_popup_dialog( const std::string Text, bool bHasCancelButton )
{
        this->Text = std::make_shared<unotui::w_textbox>( point(), point(), Text );
        this->bHasCancelButton = bHasCancelButton;
        
        this->Position = point( 0.25f, 0.75f );
        this->Size     = point( 0.50f, 0.50f );
        // Don't overshadow overlay.
        this->Layer    = unotui::LayerReference::Frontmost-1;
}


void w_popup_dialog::PostConstruct()
{
        this->Container = AddChild( new unotui::w_verticalbox() );
        
        this->Container->Padding   = pixel(32);
        this->Container->bPadSides = true;
        
        this->Container->AddChild( this->Text );
        
        this->ButtonContainer = this->Container->AddChild( new unotui::w_horizontalbox() );
        
        this->ButtonContainer->Padding   = point( 0, 0.1f );
        this->ButtonContainer->bPadSides = false;
        
        this->ButtonOkay = this->ButtonContainer->AddChild( new unotui::w_button( "Okay", action::Okay ) );
        if( bHasCancelButton ) {
                this->ButtonCancel = this->ButtonContainer->AddChild(
                        new unotui::w_button( "Cancel", action::Cancel )
                );
        }
}

void w_popup_dialog::OnRefresh( unotui::ValidityState_t Reason )
{
        if( Reason == unotui::ValidityState::Resized ) {
                gColor.Clear();
                gColor.AddRectangle(
                        unotui::colored_rectangle(
                                fpoint( 0.0f, 1.0f ),
                                fpoint( 1.0f, 0.0f ),
                                rgba( unotui::color::black, 0.2f )
                        )
                );
                gColor.Update();
                
                this->Container->Position = { 0.25f, 0.75f };
                this->Container->Size     = { 0.50f, 0.50f };
                
        }
}

void w_popup_dialog::OnDraw()
{
        gColor.Draw();
}


void w_popup_dialog::OnEvent( std::shared_ptr<unotui::widget_event> Event )
{
        using unotui::widget_event;
        using unotui::we_click;
        
        widget_event* Pointer = Event.get();
        
        we_click* ClickEvent;
        if(( ClickEvent = dynamic_cast<we_click*>(Pointer) )) {
                switch( ClickEvent->ActionID ) {
                        case action::Okay: {
                                ThrowEvent( std::make_shared<we_popup_action_confirm>() );
                                break;
                        }
                        case action::Cancel: {
                                ThrowEvent( std::make_shared<we_popup_action_cancel>() );
                                break;
                        }
                }
        }
        
}

bool w_popup_dialog::DoesCollide( unotui::point )
{
        return true;
}

        
} // namespace unotu
