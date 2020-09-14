#ifndef __UNOTU_ENTITIES_WIDGETS_GEOMETRY_TEXT_H_
#define __UNOTU_ENTITIES_WIDGETS_GEOMETRY_TEXT_H_

#include <entities\widgets\geometry\texture.h>

#include <vector>

#include <types.h>
#include <utility\colors.h>
#include <utility\text.h>

struct colored_patch
{
        colored_patch() = default;
        colored_patch( std::size_t Length, rgba Color )
        {
                this->Length = Length;
                this->Color = Color;
        }
        colored_patch( std::size_t Length, rgba Color, rgba BackgroundColor )
        {
                this->Length = Length;
                this->Color = Color;
                this->BackgroundColor = BackgroundColor;
        }
        std::size_t Length;
        rgba Color = color::black;
        rgba BackgroundColor = color::transparent;
};

struct color_change
{
        color_change() = default;
        color_change( text_coord Position, rgba Color )
        {
                this->Position = Position;
                this->Color = Color;
        }
        color_change( text_coord Position, rgba Color, rgba BackgroundColor )
        {
                this->Position = Position;
                this->Color = Color;
                this->BackgroundColor = BackgroundColor;
        }
        text_coord Position = {0, 0};
        rgba Color = color::black;
        rgba BackgroundColor = color::transparent;
};

struct text_geometry : public texture_geometry
{
        std::vector< colored_patch > Patches;
        
        text_geometry( GLenum Usage = GL_STATIC_DRAW );
        virtual void Draw(void);
        virtual void Clear(void);
        
        void AddText(
                const std::string& Text,
                int FontSize,
                fpoint Origin,
                rgba Color = color::black,
                float TopCut = 0.0f,
                float BottomCut = 0.0f
        );
        
        void AddText(
                const std::string& Text,
                std::vector<split_line> Lines,
                int FontSize,
                fpoint Origin,
                rgba Color = color::black,
                float TopCut = 0.0f,
                float BottomCut = 0.0f
        );
        
        void AddText(
                const std::string& Text,
                std::vector<split_line> Lines,
                int FontSize,
                fpoint Origin,
                std::vector<color_change> Colors = {{{0, 0}, color::black}},
                float TopCut = 0.0f,
                float BottomCut = 0.0f
        );
        
};

#endif
