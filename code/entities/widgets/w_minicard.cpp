#include <entities\widgets\w_minicard.h>

#include <entities\ent_opengl.h>
#include <utility\widget.h>

#include <utility\colors.h>

const int TITLE_FONT            = 24;
const int CONTENT_FONT          = 18;

//const int MAX_CONTENT_LENGTH    = 128;

const int MIN_WIDTH             = 200;
const int MIN_HEIGHT            = 140;
const int PADDING               = 2;

w_minicard::w_minicard
(
        const point& Position,
        const std::string& Title,
        const std::string& Content,
        const point& Size
)
{
        
        this->CommonSetup();
        
        this->Position = Position;
        this->Size = Size;
        
        this->SetTitle( Title );
        this->SetContent( Content );
        
}

w_minicard::w_minicard( const card& Card, const point& Position, const point& Size )
{
        this->CommonSetup();
        
        this->Position = Position;
        this->Size = Size;
        
        this->SetTitle( Card.Title );
        this->SetContent( Card.Content );
        
}

w_minicard::w_minicard(const indexed_card& Card, const point&, const point&)
{
        this->CommonSetup();
        
        this->Position = Position;
        this->Size = Size;
        
        this->CardID = Card.ID;
        this->SetTitle( Card.Title );
        this->SetContent( Card.Content );
}


void w_minicard::PostConstruct()
{
        this->AddChild( this->TitleText );
        this->AddChild( this->ContentText );
}

void w_minicard::CommonSetup()
{
        
        this->bInvalidateOnMousePass = true; // For highlight to update.
        
        this->TitleText = std::make_shared<w_text>();
        this->ContentText = std::make_shared<w_textbox>();
        
        this->TitleText->font_size = TITLE_FONT;
        this->ContentText->FontSize = CONTENT_FONT;
        
        this->TitleText->color = color::black;
        
        this->ContentText->FontColor = color::black;
        this->ContentText->BackgroundColor = color::white;
        this->ContentText->OutlineThickness = 2;
        
        this->ContentText->OutlineColorLeft   = TheTheme.PrimaryLit;
        this->ContentText->OutlineColorRight  = TheTheme.PrimaryBack;
        this->ContentText->OutlineColorTop    = TheTheme.PrimaryBack;
        this->ContentText->OutlineColorBottom = TheTheme.PrimaryLit;
        
}


w_minicard::~w_minicard() {}

void w_minicard::OnTick()
{
        
        //:: Size fixup.
        
        if( this->Size.x.xpixels() < MIN_WIDTH )
        {
                this->Size.x = MIN_WIDTH;
                this->Invalidate( ValidityState::Resized );
        }
        
        if( this->Size.y.ypixels() < MIN_HEIGHT )
        {
                this->Size.y = MIN_HEIGHT;
                this->Invalidate( ValidityState::Resized );
        }
                
}

void w_minicard::OnRefresh( ValidityState_t Reason )
{
        
        const fpoint FPosition = this->Position;
        const fpoint FPosition2= SecondPosition( this->Position, this->Size );
        
        //:: Own geometry.
        
        this->gColor.Clear();
        
        this->gColor.AddRectangle(
                colored_rectangle(
                        FPosition,
                        FPosition2,
                        this->bMouseOver ? TheTheme.Highlight : TheTheme.Primary
                )
        );
        
        const rgba OutlineColor1 = TheTheme.PrimaryLit;
        const rgba OutlineColor2 = TheTheme.PrimaryBack;
        const rgba OutlineColor2Light = color::gray * 0.8f;
        
        if( this->bMouseOver ) {
                this->gColor.AddOutline(
                        rectangle( FPosition, FPosition2 ),
                        -4,
                        OutlineColor1,
                        OutlineColor2Light,
                        OutlineColor2Light,
                        OutlineColor1
                );
        } else {
                this->gColor.AddOutline(
                        rectangle( FPosition, FPosition2 ),
                        -4,
                        OutlineColor2,
                        OutlineColor1,
                        OutlineColor1,
                        OutlineColor2
                );
        }
        
        this->gColor.Update();
        
        //:: Title.
        
        const fpoint Padding = point( PADDING );
        const fpoint AvailableArea = (
                fpoint(this->Size)
                -
                fpoint( Padding.x*2, Padding.y*4 )
        );    
        
        this->TitleText->Position = this->Position + point( pixel(PADDING), -pixel(PADDING) );
        const float TitleLowerYPos = this->TitleText->Position.y.yratio()-AvailableArea.y*0.15f;
        this->TitleText->SetSecondPosition
        (
                point(
                        ratio(FPosition2.x-Padding.x),
                        ratio(TitleLowerYPos)
                )
        );
        this->TitleText->font_size = this->TitleText->Size.y.ypixels();
        
        this->TitleText->Invalidate( Reason );
        
        //:: Content.
        
        this->ContentText->Position = point(
                this->Position.x + pixel(PADDING),
                TitleLowerYPos - Padding.y*2.0f
        );
        
        this->ContentText->SetSecondPosition(
                point(
                        FPosition2.x - Padding.x,
                        FPosition2.y + Padding.y
                )
        );
        
        this->ContentText->Invalidate( Reason );
        
}

void w_minicard::OnDraw()
{
        //:: Draw colored rectangles.
        this->gColor.Draw();
}

void w_minicard::OnMousePressed( const int Button )
{
        if( Button == GLFW_MOUSE_BUTTON_LEFT ) {
                this->ThrowEvent( std::make_shared<we_click>( 1, "Edit", Button ) );
        } else if( Button == GLFW_MOUSE_BUTTON_RIGHT ) {
                this->ThrowEvent( std::make_shared<we_click>( 0, "Delete", Button ) );
        }
}


        //:: Helpers.

void w_minicard::SetTitle( const std::string& Title )
{
        this->TitleText->text = Title;
}

void w_minicard::SetContent( const std::string& Content )
{
        this->ContentText->TextSet( Content );
        
}

