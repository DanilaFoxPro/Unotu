#ifndef __W_EDITABLETEXT_H_
#define __W_EDITABLETEXT_H_

#include <entities\widgets\widget.h>
#include <entities\modules\m_caret.h>
#include <entities\modules\m_text.h>
#include <utility\colors.h>
#include <types.h>

/*
 * @brief Editable one-line text.
 *
 */
struct w_editabletext : public widget, public m_caret, public m_text
{
        
//:: Constructors.
        w_editabletext() = default;
        w_editabletext( const point&, const point&, const int = 0 );
        
        void PostConstruct();
        
//:: Data.
        
        // Main.
        
        std::string Text;
        
        // Appearance.
        
        rgba TextColor          = color::black;
        rgba BackgroundColor    = color::white;
        rgba OutlineColor       = color::gray;
        
        int OutlineThickness	= 2;
        
        // State.
        
                /** Used by the mouse selection system. @see OnMousePressed() */
        bool bBegunClickSelection = false;
        
        // Options.
        
        bool bCommitOnEnter = false;
        bool bClearOnCommit = true;
        
        // Geometry.
        
        color_geometry gColor;
        text_geometry gText;
        
//:: Overrides.
        
        virtual void OnTick() final;
        virtual void OnRefresh( ValidityState_t ) final;
        virtual void OnDraw() final;
        
        // Input.
        
        virtual void OnKeyboardFocused() final;
        
        virtual void OnCharacterInput( const std::string& ) final;
        virtual void OnKeyInput( const int&, const int& ) final;
        
        virtual void OnMousePressed( const int Button ) final;
        virtual void OnMouseReleased( const int Button, const bool bFocusingClick ) final;
        
//:: Functions
        
        void CommitText();
        
        int FontSize();
        text_coord PositionToTextCoord( const fpoint Position );
        
        virtual void TextSet( const std::string& );
        virtual void TextClear();
        virtual std::string TextGet();
        virtual std::string* TextGetRef();
                
};

#endif
