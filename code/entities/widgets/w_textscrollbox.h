#ifndef __W_TEXTSCROLLBOX_H_
#define __W_TEXTSCROLLBOX_H_

#include <entities\widgets\widget.h>
#include <entities\widgets\w_textbox.h>
#include <entities\widgets\w_scrollbar.h>
#include <entities\modules\m_text.h>

#include <utility\colors.h>

struct w_textscrollbox : public widget, public m_textlines
{
//:: Data.
        std::shared_ptr<w_textbox> TextBox;
        std::shared_ptr<w_scrollbar> ScrollBar;

        rgba BackgroundColor;
        rgba OutlineColor;

        int OutlineThickness;
        
//:: Geometry.
        
        color_geometry gColor;

//:: Constructors.
        w_textscrollbox
        (
                const point& position_ = point( 0.0f, 1.0f ),
                const point& size_ = point( 1.0f, 1.0f ),
                
                const std::string&      text            = "",
                const int&              font_size       = 16,
                const rgba&             text_color      = rgba( color::black, 1.0f ),
                
                const rgba&     background_color_       = color::white,
                const rgba&     outline_color_          = color::gray,
                const int&      outline_thickness_      = 2
        );
        void PostConstruct();
        virtual ~w_textscrollbox() = default;

//:: Overrides.
        virtual void OnRefresh( ValidityState_t );
        virtual void OnDraw();

        void OnEvent( std::shared_ptr<widget_event> );

//:: Helpers.
        void SetScrollOffset( float );
        void SetScrollOffset( std::size_t );
        void ScrollIntoView( std::size_t );

        void ScrollToTop();
        void ScrollToBottom();

        float GetScrollOffsetLines();

//:: Text module.
        virtual void SetText( const std::string& );
        virtual void ClearText();
        virtual void TextUpdated();
        
        virtual std::string GetText();
        virtual std::string* GetTextRef();
        
        virtual std::string GetOriginalText();
        virtual std::string* GetOriginalTextRef();
};


#endif
