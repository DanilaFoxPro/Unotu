#ifndef __W_TEXTBOX_H
#define __W_TEXTBOX_H

#include <entities\ent_window.h>
#include <entities\widgets\widget.h>

#include <entities\modules\m_text.h>
#include <utility\colors.h>

struct w_textbox : public widget, public m_textlines
{
        //Data.
private:
        std::string Text;
        bool bTextInvalidated = true;
public:
        int FontSize = 16;
        rgba FontColor = TheTheme.Text;
        double Offset = 0.0f;

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

        // Cache.
        /** Cache of some things that need to be re-used. */
private:
        std::vector<split_line> SplitTextCache;
        float VisibleRatio = 0.0f;
        std::size_t TotalLineCount = 0;
        point TextAreaSize = point();
public:

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
                this->TextSet( Text );
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
private:
        void UpdateSplitText();
        void ReadyText();
public:
        
        // Helpers.
        
        void SetOffset( const double Ratio );
        void SetOutlineColor( const rgba& Color );
        
        std::size_t LineCount();
        text_coord PositionToTextCoord( const fpoint Position );
        float TextViewzoneX() const;
        float TextViewzoneY() const;
        
//:: Text module.
        
        virtual void TextSet( const std::string& );
        virtual void TextClear();
        virtual void TextUpdated();
        
        virtual std::string OriginalTextGet();
        virtual std::string* OriginalTextGetRef();
        
        virtual std::string TextGet();

        virtual std::vector<split_line> LineMapGet();
        virtual std::size_t LineCountGet();
};

#endif
