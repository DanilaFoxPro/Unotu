#ifndef __UNOTU_ENTITIES_WIDGETS_W_BUFFER_H_
#define __UNOTU_ENTITIES_WIDGETS_W_BUFFER_H_

#include <entities\widgets\widget.h>
#include <utility\colors.h>

/**
 * @brief Caches all child widgets in a buffer on refresh.
 *        Draws the buffer on draw.
 */
struct w_buffer : widget
{
//:: Constructors.
        w_buffer();
        w_buffer( point, point );
        virtual ~w_buffer();
//:: Data.
        GLuint Framebuffer = GL_NONE;
        GLuint FramebufferTexture = GL_NONE;
        
        std::pair<int, int> Offset {0, 0};
        rgba BackgroundColor {color::white};
//:: Functions.
        void SetOptions();
        void CreateFramebuffer();
        void ResizeFramebufferTexture();
//:: Overrides.
        virtual void OnRefresh( ValidityState_t );
        virtual void OnPostRefresh( ValidityState_t, bool );
        virtual void OnDraw();
//:: Helpers.
        void SetOffset( const fpoint& );
};

#endif

