#ifndef __UNOTU_ENTITIES_WIDGETS_W_BUTTON_H_
#define __UNOTU_ENTITIES_WIDGETS_W_BUTTON_H_

#include <entities\ent_window.h>
#include <entities\widgets\widget.h>
#include <utility\colors.h>

struct w_button : public widget
{
//:: Constructors.
        w_button() = default;
        
        w_button(
                const std::string& Text,
                int ActionID = 0
        );
        
        w_button(
                const std::string& Text,
                const std::string& ActionName
        );
        
        w_button(
                point Position, point Size,
                const std::string& Text = "",
                int ActionID = 0
        );
        
        w_button(
                point Position,
                point Size,
                const std::string& Text,
                const std::string& ActionName = ""
        );
        
//:: Constructor helpers.
        
        /**
         * @brief This is called almost all constructors to set the common variables.
         * 
         * @note Also sets options via 'SetOptions()'.
         */
        void ConstructorProxy(
                const std::string& Text,
                int ActionID,
                const std::string& ActionName
        );
        
        /**
         * @brief Sets default options. Called by constructors.
         */
        void SetOptions();
        
//:: Data.
        std::string Text;
        rgba TextColor = TheTheme.Text;
        
        rgba Color            = TheTheme.Primary; /// Background color.
        rgba ColorHighlighted = TheTheme.Highlight; /// Background color when mouse over.
        
        static const int NoAction = -1;
        int ActionID = this->NoAction;
        std::string ActionName;
        
//:: Cache.
        
        /// Cache of last-calculated FontSize.
        int FontSize;
        
//:: Options.
        
        /// Don't calculate font size on refresh, leave the value as-is.
        bool bOverrideSize        = false;
        /** If size is overriden, it should be only for only one refresh
         *  reset both 'bOverrideSize' and 'bOverrideSizeFragile' after
         *  that.
         */
        bool bOverrideSizeFragile = false;
        
//:: Geometry.
        
        color_geometry gColor;
        text_geometry  gText;
        
//:: Overrides.
        virtual void OnMousePressed( int );
        
        virtual void OnRefresh( ValidityState_t );
        virtual void OnDraw();
        
};

#endif
