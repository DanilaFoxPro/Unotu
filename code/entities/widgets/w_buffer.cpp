#include <entities\widgets\w_buffer.h>

#include <entities\ent_window.h>

#include <utility\widget.h>
#include <utility\window.h>

        //:: Constructors.

w_buffer::w_buffer()
{
        this->SetOptions();
        this->CreateFramebuffer();
}

w_buffer::w_buffer( point Position, point Size )
{
        this->Position = Position;
        this->Size     = Size;
        
        this->SetOptions();
        this->CreateFramebuffer();
}

w_buffer::~w_buffer()
{
        glDeleteFramebuffers( 1, &this->Framebuffer );
        glDeleteTextures( 1, &this->FramebufferTexture );
}

        //:: Functions.

void w_buffer::SetOptions()
{
        this->bDrawRecursive = false;
}

void w_buffer::CreateFramebuffer()
{
        if( this->Framebuffer != GL_NONE ) {
                return;
        }
        glGenFramebuffers( 1, &this->Framebuffer );
        glGenTextures    ( 1, &this->FramebufferTexture);
        
        this->ResizeFramebufferTexture();
        glBindFramebuffer( GL_FRAMEBUFFER, this->Framebuffer );
        
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->FramebufferTexture, 0 );
        GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, DrawBuffers);
        
        if( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE ) {
                printf( "%s:: Framebuffer creation failed!\n", ClassName( *this ).c_str() );
        }
        
        glBindFramebuffer( GL_FRAMEBUFFER, GL_NONE );
}

void w_buffer::ResizeFramebufferTexture()
{
        const int Width  = this->Size.x.xpixels();
        const int Height = this->Size.y.ypixels();
        
        glActiveTexture(GL_TEXTURE0);
        
        glBindTexture( GL_TEXTURE_2D, this->FramebufferTexture );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, GL_NONE );
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

        //:: Overrides.

void w_buffer::OnRefresh( ValidityState_t Reason )
{
        if( Reason & ValidityState::Resized ) {
                this->ResizeFramebufferTexture();
        }
}

void w_buffer::OnPostRefresh( ValidityState_t State, bool ChildrenRefreshed )
{
        if( State == ValidityState::Valid && !ChildrenRefreshed ) {
                return;
        }
        
        const ent_window& Current = TheWindowManager.Cur();
        const point pSecondPosition = SecondPosition( this->Position, this->Size );
        
        const int X0 = this->Position.x.xpixels();
        const int Y0 = pSecondPosition.y.ypixels();
        
        glBindFramebuffer( GL_FRAMEBUFFER, this->Framebuffer );
        
        // Offset the framebuffer viewport so it starts rendering at window origin instead of
        // framebuffer origin.
        glViewport( -X0+this->Offset.first, -Y0+this->Offset.second, Current.x, Current.y );
        
        glClearColor(
                this->BackgroundColor.r,
                this->BackgroundColor.g,
                this->BackgroundColor.b,
                this->BackgroundColor.a
        );
        glClear(GL_COLOR_BUFFER_BIT);
        DrawWidgets( this->Children );
        
        ViewportToWindow();
        
        glBindFramebuffer( GL_FRAMEBUFFER, GL_NONE );
}


void w_buffer::OnDraw()
{
        const point pSecondPosition = SecondPosition( this->Position, this->Size );
        
        const int Width  = this->Size.x.xpixels();
        const int Height = this->Size.y.ypixels();
        
        const int X0 = this->Position.x.xpixels();
        const int Y0 = pSecondPosition.y.ypixels();
        
        const int X1 = pSecondPosition.x.xpixels();
        const int Y1 = this->Position.y.ypixels();
        
        glBindFramebuffer( GL_READ_FRAMEBUFFER, this->Framebuffer );
        glBindFramebuffer( GL_DRAW_FRAMEBUFFER, GL_NONE );
        
        // TODO: Consider just rendering to a rectangle in case of transparency issues.
        glBlitFramebuffer(
                0, 0, Width, Height,
                X0, Y0, X1, Y1,
                GL_COLOR_BUFFER_BIT, GL_NEAREST
        );
        
        glBindFramebuffer( GL_READ_FRAMEBUFFER, GL_NONE );
        
}

        //:: Helpers.

void w_buffer::SetOffset( const fpoint& Offset )
{
        const fpoint Pixel = point(1);
        this->Offset = std::make_pair( (int)Offset.x/Pixel.x, (int)Offset.y/Pixel.y );
}

