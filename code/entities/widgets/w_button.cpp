#include <entities\widgets\w_button.h>

#include <utility\widget.h>
#include <utility\text.h>

        //:: Constructors.

w_button::w_button (
        const std::string& Text,
        int ActionID
)
{
        this->ConstructorProxy( Text, ActionID, "" );
}

w_button::w_button (
        const std::string& Text,
        const std::string& ActionName
)
{
        this->ConstructorProxy( Text, this->NoAction, ActionName );
}

w_button::w_button(
        point Position,
        point Size,
        
        const std::string& Text,
        int ActionID
)
{
        this->ConstructorProxy( Text, ActionID, "" );
        
        this->Position = Position;
        this->Size = Size;
}

w_button::w_button(
        point Position,
        point Size,
        
        const std::string& Text,
        const std::string& ActionName
)
{
        this->ConstructorProxy( Text, this->NoAction, ActionName );
        
        this->Position = Position;
        this->Size = Size;
}

        //:: Constructor helpers.

void w_button::ConstructorProxy(
        const std::string& Text,
        int ActionID,
        const std::string& ActionName
)
{
        this->Text = Text;
        this->ActionID = ActionID;
        this->ActionName = ActionName;
        
        this->SetOptions();
}

void w_button::SetOptions()
{
        this->bInvalidateOnMousePass = true;
}

        //:: Overrides.

void w_button::OnMousePressed( const int Button )
{
        this->ThrowEvent( std::make_shared<we_click>( this->ActionID, this->ActionName, Button ) );
}

void w_button::OnRefresh(
        ValidityState_t Reason
)
{
        
        if( !( Reason & ( ValidityState::ParametersUpdated | ValidityState::Resized | ValidityState::Mouse ) ) )
        {
                return;
        }
        
        const fpoint Pixel = point( pixel(1) );
        
        const fpoint FPosition = this->Position;
        const fpoint FSize     = this->Size;
        const fpoint Center    = SecondPosition( FPosition, FSize/2.0f );
        
        const int TextXPadding = 4;
        const int TextYPadding = 3;
        
        if( !this->bOverrideSize ) {
                FontSize = FSize.y/Pixel.y - TextYPadding*2*Pixel.y;
        
                const float SpaceTextTakes = this->Text.size()*(float)FontSize/2.0f*Pixel.x;
                const float AvailableVerticalSpace = FSize.x-TextXPadding*Pixel.x;

                if( SpaceTextTakes > AvailableVerticalSpace ){
                        FontSize = ( FSize.x-TextXPadding*Pixel.x )/Pixel.x/((float)this->Text.size()/2.0f);
                }
                
        } else if( this->bOverrideSizeFragile ) {
                this->bOverrideSize = false;
                this->bOverrideSizeFragile = false;
        }
        
        const fpoint TextOrigin =
                fpoint(
                        Center.x - ((int)this->Text.size()*FontSize)*Pixel.x/4.0f,
                        Center.y + FontSize*Pixel.y/2.0f
                );
        
        
        this->gColor.Clear();
        this->gText.Clear();
        
        this->gColor.AddRectangle(
                colored_rectangle(
                        this->Position,
                        SecondPosition( this->Position, this->Size ),
                        this->bMouseOver ? this->ColorHighlighted : this->Color
                )
        );
        
        this->gText.AddText( this->Text, FontSize, TextOrigin, this->TextColor );
        
        // Testing outline.
        
        const fpoint FPosition2 = SecondPosition( this->Position, this->Size );
        
        const rgba OutlineColor1 = TheTheme.PrimaryLit;
        const rgba OutlineColor2 = TheTheme.PrimaryBack;
        const rgba OutlineColor2Light = color::gray * 0.8f;
        
        if( this->bMouseOver ) {
                this->gColor.AddOutline(
                        rectangle( FPosition, FPosition2 ),
                        3,
                        OutlineColor1,
                        OutlineColor2Light,
                        OutlineColor2Light,
                        OutlineColor1
                );
        } else {
                this->gColor.AddOutline(
                        rectangle( FPosition, FPosition2 ),
                        3,
                        OutlineColor2,
                        OutlineColor1,
                        OutlineColor1,
                        OutlineColor2
                );
        }
        
        
        this->gColor.Update();
        this->gText.Update();
}

void w_button::OnDraw()
{
        this->gColor.Draw();
        this->gText.Draw();
}
