#include <entities\widgets\geometry\text.h>

#include <entities\ent_opengl.h>
#include <utility\shortcuts.h>
#include <entities\ent_window.h>

text_geometry::text_geometry( GLenum Usage )
{
        this->Usage = Usage;
}

void text_geometry::Draw()
{
        if( this->EnableBlend ) {
                glEnable( GL_BLEND );
        }
        
        glUseProgram( the_opengl.TextShader );
        glBindVertexArray( this->VAO );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->EBO );
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture( GL_TEXTURE_2D, the_opengl.FontTexture );
        
        std::size_t Offset = 0;
        for( colored_patch Patch : this->Patches ) {
                
                glUniform4f(
                        glGetUniformLocation( the_opengl.TextShader, "fontColor" ),
                        Patch.Color.r,
                        Patch.Color.g,
                        Patch.Color.b,
                        Patch.Color.a
                );
                glUniform4f(
                        glGetUniformLocation( the_opengl.TextShader, "fontBackgroundColor" ),
                        Patch.BackgroundColor.r,
                        Patch.BackgroundColor.g,
                        Patch.BackgroundColor.b,
                        Patch.BackgroundColor.a
                );
                
                glDrawElements(
                        GL_TRIANGLES,
                        Patch.Length,
                        GL_UNSIGNED_INT,
                        (void*)(Offset*sizeof(unsigned int))
                );
                
                Offset += Patch.Length;
        }

        if( this->EnableBlend ) {
                glDisable( GL_BLEND );
        }
}

void text_geometry::Clear()
{
        texture_geometry::Clear();
        this->Patches.clear();
}

void text_geometry::AddText(
        const std::string& Text,
        int FontSize,
        fpoint Origin,
        rgba Color,
        float TopCut,
        float BottomCut
)
{
        return this->AddText(
                Text,
                SplitTextNew( Text, std::size_t(-1) ),
                FontSize,
                Origin,
                Color,
                TopCut,
                BottomCut
        );
}

void text_geometry::AddText(
        const std::string& Text,
        std::vector<split_line> Lines,
        int FontSize,
        fpoint Origin,
        rgba Color,
        float TopCut,
        float BottomCut
)
{
        return this->AddText(
                Text,
                Lines,
                FontSize,
                Origin,
                {{{0, 0}, Color}},
                TopCut,
                BottomCut
        );
}

/**
 * @brief Bloated function for adding colored text.
 * 
 * @param Text     The source text.
 * @param Lines    Lines to be drawn, as references to source text.
 * @param FontSize Size of the font in pixels.
 * @param Origin   Origin point at which to start drawing the text.
 * @param Colors   Set of positions at which to change the text color.
 *                 
 * @param TopCut    How much to cut off of the top of the top line. As ratio.
 * @param BottomCut How much to cut off of the bottom of the bottom line. As ratio.
 */
void text_geometry::AddText(
        const std::string& Text,
        std::vector<split_line> Lines,
        int FontSize,
        fpoint Origin,
        std::vector<color_change> Colors,
        float TopCut,
        float BottomCut
)
{
        
        /* TODO: Looks like a mess, make more readable.
         *       Perhaps split into multiple functions.
         */
        
        if( Colors.size() == 0 ) {
                Colors = {{{0, 0}, color::black}};
        }
        
        ent_window& TheWindow = TheWindowManager.Cur();
        
        const float FontHeight = pixel(FontSize).ratio( TheWindow.Height );
        const float FontWidth  = pixel(FontSize).ratio( TheWindow.Width );
        
        TopCut    = clamp( TopCut, 0.0f, 1.0f );
        BottomCut = clamp( BottomCut, 0.0f, 1.0f );
        
        const float InvTopCut = 1.0f-TopCut;
        const float InvBottomCut = 1.0f-BottomCut;
        
        const float OriginX = Origin.x;
              float OriginY = Origin.y;
        
        const std::size_t LastLine = Lines.size()-1;
        std::size_t TotalCharactersAdded = 0;
        
        std::size_t CurColorChange = 0;
        std::size_t LastColorChangeChar = 0;
        
        for( std::size_t Line = 0; Line < Lines.size(); Line++ )
        {
                const split_line& SplitLine = Lines[Line];
                
                int RightShift = 0;
                for( std::size_t i = 0; i <= SplitLine.Length(); i++, TotalCharactersAdded++ ) {
                        
                        while( Colors.size() > CurColorChange+1
                                && Colors[CurColorChange+1].Position == text_coord{Line, i} ) {
                                
                                const auto ColorChange = Colors[CurColorChange];
                                const std::size_t SinceLast = TotalCharactersAdded-LastColorChangeChar;
                        
                                if( SinceLast != 0 ) {
                                        const std::size_t IndicesSinceLast = SinceLast*6;
                                        this->Patches.push_back(
                                                colored_patch(
                                                        IndicesSinceLast,
                                                        ColorChange.Color,
                                                        ColorChange.BackgroundColor
                                                )
                                        );
                                        LastColorChangeChar = TotalCharactersAdded;
                                } else if( this->Patches.size() != 0 ) {
                                        colored_patch& LastPatch = this->Patches[this->Patches.size()-1];
                                        LastPatch.Color = ColorChange.Color;
                                        LastPatch.BackgroundColor = ColorChange.BackgroundColor;
                                }
                                
                                CurColorChange++;
                        }
                        
                        const char& CurrentChar = Text[SplitLine[i]];
                        
                        if( CurrentChar == '\n' ) {
                                TotalCharactersAdded--;
                                continue;
                        }
                        
                        const float Shift = (float)(RightShift++);
                        rectangle_tc TexCoord = CharacterTextureCoordinates( CurrentChar );
                        const float TCCharHeight = 1.0f/16.0f;
                        float Y2 = OriginY-FontHeight;
                        
                        // Handle top and bottom cuts.
                        if( Line == 0 && TopCut != 0.0f ) {
                                TexCoord.y1 -= TCCharHeight*TopCut;
                                Y2 = OriginY-FontHeight*InvTopCut;
                        } else if( Line == LastLine && BottomCut != 0.0f ) {
                                TexCoord.y2 += TCCharHeight*BottomCut;
                                Y2 = OriginY-FontHeight*InvBottomCut;
                        }
                        
                        // Character rectangle.
                        const textured_rectangle Rect = textured_rectangle(
                                OriginX+(FontWidth/2.0f*Shift),
                                OriginY,
                                                                        
                                OriginX+(FontWidth/2.0f*(Shift+1.0f)),
                                Y2,
                                                                        
                                TexCoord
                        );
                        
                        this->AddRectangle( Rect );
                        
                }
                
                if( Line == 0 ) {
                        OriginY -= FontHeight*InvTopCut;
                } else {
                        OriginY -= FontHeight;
                }
        }
        
        const std::size_t SinceLastChange = TotalCharactersAdded-LastColorChangeChar;
        if( SinceLastChange != 0 ) {
                const std::size_t IndicesSinceLast = SinceLastChange*6;
                this->Patches.push_back(
                        colored_patch(
                                IndicesSinceLast,
                                Colors[CurColorChange].Color,
                                Colors[CurColorChange].BackgroundColor
                        )
                );
        }
        
}
