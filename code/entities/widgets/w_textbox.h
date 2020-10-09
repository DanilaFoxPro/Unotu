#ifndef __W_TEXTBOX_H
#define __W_TEXTBOX_H

#include <entities\ent_window.h>
#include <entities\widgets\widget.h>

#include <entities\modules\m_text.h>
#include <utility\colors.h>

struct w_textbox : public widget, public m_textlines
{
        //Data.

        std::string Text;
        int FontSize = 16;
        rgba FontColor = TheTheme.Text;
        float Offset = 0.0f;

        rgba BackgroundColor    = color::transparent;
        
        rgba
                OutlineColorLeft,
                OutlineColorRight,
                OutlineColorTop,
                OutlineColorBottom 
        = color::black;

        int OutlineThickness = 2;

        color_geometry gColor;
        text_geometry gText;

        //Cache.
        /** Split text cache. Useless here, but used in 'w_editabletextbox'. */
        std::vector<split_line> SplitTextCache;
        float VisibleRatio = 0.0f;
        std::size_t TotalLineCount = 0;
        point TextAreaSize = point();

//:: Constructors.

        w_textbox() = default;
        w_textbox(
                const point&    Position,
                const point&    Size = point( ratio( 0.5f ), ratio( 0.8f ) ),

                const std::string&      Text            = "",
                const int&              FontSize      = 16,
                const rgba&             FontColor     = rgba( color::black, 1.0f ),

                const rgba&     BackgroundColor       = rgba( color::black, 0.0f ),
                const int&      OutlineThickness      = 6,
                const rgba&     OutlineColor          = rgba( color::black, 1.0f )
        )
        {
                
                //:: Text.
                this->SetText( Text );
                this->FontSize	= FontSize;
                this->FontColor	= FontColor;
                
                this->Offset = 0.0f;
                
                //:: Body.
                this->BackgroundColor	= BackgroundColor;
                this->SetOutlineColor( OutlineColor );
                this->OutlineThickness	= OutlineThickness;
                
                //:: Widget base.
                this->Position = Position;
                this->Size = Size;
        }
        
//:: Functions.

        virtual void OnDraw();
        virtual void OnRefresh( ValidityState_t );

        // Internal.
        
        void UpdateSplitText();
        
        // Helpers.

        void SetOffset( const float& ratio );
        void SetOutlineColor( const rgba& Color );
        std::size_t LineCount() const;
        text_coord PositionToTextCoord( const fpoint Position );
        
//:: Text module.
        
        virtual void SetText( const std::string& );
        virtual void ClearText();
        virtual void TextUpdated();
        
        virtual std::string GetOriginalText();
        virtual std::string* GetOriginalTextRef();
        
        virtual std::string GetText();

        virtual std::vector<split_line> GetLineMap();
};

#endif
