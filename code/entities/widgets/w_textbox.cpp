#include "w_textbox.h"

#include <utility\text.h>
#include <workers\widget_worker.h>
#include <utility\shortcuts.h>

#include <entities\ent_window.h>
#include <entities\ent_opengl.h>
#include <entities\modules\m_caret.h>

#include <utility\colors.h>
#include <utility\widget.h>

#include <stdio.h> //TODO: DEBUG.
#include <cmath>

void w_textbox::OnRefresh( ValidityState_t Reason )
{
        
        const point Position2 = SecondPosition( Position, Size );
        
        // Giving additional padding on X axis. Looks nicer with padding on the sides.
        // Especially when text and outline colors match.
        const point TextPadding = point( pixel(3), 0 );
        
        std::pair< point, point > Area = OutlineAdjustedArea( Position, Position2, this->OutlineThickness );
        std::pair< point, point > TextArea = ApplyPadding( Area.first, Area.second, TextPadding );
        
        gColor.Clear();
        gText.Clear();
        
        //:: Outline.
        
        const bool bDrawOutline = (
                this->OutlineThickness &&
                (
                        this->OutlineColorLeft.alpha   > 0.0f ||
                        this->OutlineColorRight.alpha  > 0.0f ||
                        this->OutlineColorTop.alpha    > 0.0f ||
                        this->OutlineColorBottom.alpha > 0.0f
                )
        );
        
        if( bDrawOutline ) {
                gColor.AddOutline(
                        rectangle( Position, Position2 ),
                        this->OutlineThickness,
                        
                        this->OutlineColorLeft,
                        this->OutlineColorRight,
                        this->OutlineColorTop,
                        this->OutlineColorBottom
                );
        }
        
        //:: Background.
        
        if( this->BackgroundColor.alpha != 0.0f ) {
                
                // Positions shifted by outline thickness so background and outline don't overlap.
                // Since both can be transparent.
                const fpoint BGPosition1 = Area.first;
                const fpoint BGPosition2 = Area.second;
                
                gColor.AddRectangle (
                        colored_rectangle (
                                BGPosition1.x,
                                BGPosition1.y,
                                
                                BGPosition2.x,
                                BGPosition2.y,
                                
                                this->BackgroundColor
                        )
                );
                
        }
        
        //:: Text
        
        if( Reason & ValidityState::Resized ) {
                // Text has to be split. So all cache has expired.
                this->TextUpdated();
        }
        
        this->ReadyText();
        
        // How much the textbox can hold, in characters.
        const float SpaceHeight = this->TextViewzoneY();
        
        const int HeightLimit = ceilf( SpaceHeight );
        
        const auto CutPair = TextCutsFromArea( this->TotalLineCount, SpaceHeight, Offset );
        
        const float& TopCut    = CutPair.first;
        const float& BottomCut = CutPair.second;
        const float  Cuts      = TopCut+BottomCut;
        
        std::size_t NeededLinesCnt = (size_t)HeightLimit;
        if( Cuts >= 1.0f ) {
                NeededLinesCnt += 1;
        }
        
        const std::vector<split_line> CutLinesLoc = CutLines( this->SplitTextCache, NeededLinesCnt, (size_t)Offset );
        
        // Hax!
        const auto CaretPtr = dynamic_cast<m_caret*>( this->Parent.lock().get() );
        
        if( CaretPtr && CaretPtr->HasCaretSelection() ) {
                
                const text_coord First  = ToTextCoord( CaretPtr->FirstCaretSelection(), this->LineMapGet() );
                const text_coord Second = ToTextCoord( CaretPtr->SecondCaretSelection(), this->LineMapGet() );
                
                text_coord FirstOffset        = VerticallyOffsetTextCoord( First, static_cast<std::ptrdiff_t>(-Offset) );
                const text_coord SecondOffset = VerticallyOffsetTextCoord( Second, static_cast<std::ptrdiff_t>(-Offset) );
                
                const rgba InvFontColor = rgba( ~this->FontColor, this->FontColor.alpha );
        
                gText.AddText(
                        this->Text,
                        CutLinesLoc,
                        this->FontSize,
                        TextArea.first,
                        {
                                {{0, 0}, this->FontColor},
                                {FirstOffset, InvFontColor, this->FontColor},
                                {SecondOffset, this->FontColor},
                        },
                        TopCut,
                        BottomCut
                );
                
        } else {
                gText.AddText(
                        this->Text,
                        CutLinesLoc,
                        FontSize,
                        TextArea.first,
                        this->FontColor,
                        TopCut,
                        BottomCut
                );
        }
        
        //::
        
        gColor.Update();
        gText.Update();
                
}

/** @brief Re-calculates all text-related cache entries. */
void w_textbox::UpdateSplitText()
{
        const point  TextPadding = point( pixel(3), 0 );
        const fpoint Pixel       = pixel(1);
        
        const point Position2    = SecondPosition( Position, Size );
        const float SpaceHeight  = this->TextViewzoneY();
        
        std::pair< point, point > Area     = OutlineAdjustedArea( Position, Position2, this->OutlineThickness );
        std::pair< point, point > TextArea = ApplyPadding( Area.first, Area.second, TextPadding );
        
        const fpoint fFontSize = fpoint( FontSize/2.0f, FontSize ) * Pixel;
        
        this->TextAreaSize = AreaSize( TextArea.first, TextArea.second );     // !!!
        
        const float SpaceWidth = TextAreaSize.x.xratio() / fFontSize.x;
        
        this->SplitTextCache = SplitTextNew( Text, (std::size_t)SpaceWidth ); // !!!
        
        this->TotalLineCount = SplitTextCache.size();
        this->VisibleRatio = clamp (
                SpaceHeight / (float)(TotalLineCount),
                0.0f,
                1.0f
        );
        
}

/** @brief Prepares all text-related cache entries. If needed. */
void w_textbox::ReadyText()
{
        if( this->bTextInvalidated ) {
                this->UpdateSplitText();
                this->bTextInvalidated = false;
        }
}


void w_textbox::OnDraw()
{
        
        //:: Draw colored rectangles.
        this->gColor.Draw();
        
        //:: Draw text.
        this->gText.Draw();

}

void w_textbox::SetOffset( const double Ratio )
{
        this->ReadyText();
        this->Offset = 
        (
                (this->TotalLineCount)
                *
                clamp( Ratio * (1.0-this->VisibleRatio), 0.0, 1.0 )
        );
}

void w_textbox::SetOutlineColor( const rgba& Color )
{
        this->OutlineColorLeft   = 
        this->OutlineColorRight  = 
        this->OutlineColorTop    = 
        this->OutlineColorBottom = Color;
}

std::size_t w_textbox::LineCount()
{
        this->ReadyText();
        return this->TotalLineCount;
}

/** @brief Position in text coordinates based on on-screen position. */
text_coord w_textbox::PositionToTextCoord( const fpoint Position )
{
        const fpoint Pixel = pixel(1);
        fpoint FFontSize = FontSize*Pixel;
        FFontSize.x /= 2.0f;
        
        fpoint LocalPosition = Localize( this->Position, Position );
        
        const auto &LineMap = this->LineMapGet();
        
        if( LineMap.size() == 0 ) {
                return {0, 0};
        }
        
        std::pair< std::ptrdiff_t, std::ptrdiff_t > Result;
        
        Result.first = -LocalPosition.y/FFontSize.y+this->Offset;
        Result.second = LocalPosition.x/FFontSize.x;
        
        Result.first = clamp(
                Result.first,
                (std::ptrdiff_t)(0),
                (std::ptrdiff_t)LineMap.size()-1
        );
        
        Result.second = clamp(
                Result.second,
                (std::ptrdiff_t)(0),
                (std::ptrdiff_t)LineMap[Result.first].Length()
        );
        
        return Result;
        
}

/** @brief How many characters can fit horizontally. */
float w_textbox::TextViewzoneX() const
{
        return TextAreaSize.x.xratio() / (pixel(this->FontSize).xratio()/2.0);
}

/** @brief How many lines of text can fit vertically. */
float w_textbox::TextViewzoneY() const
{
        return TextAreaSize.y.yratio() / pixel(this->FontSize).yratio();
}


        //:: Text module.

void w_textbox::TextSet( const std::string& Text )
{
        this->Text = Text;
        this->TextUpdated();
        this->Invalidate( ValidityState::ParametersUpdated );
}

void w_textbox::TextClear()
{
        this->Text.clear();
        this->Invalidate( ValidityState::ParametersUpdated );
}

void w_textbox::TextUpdated()
{
        this->bTextInvalidated = true;
}

std::string w_textbox::OriginalTextGet()
{
        return this->Text;
}

std::string* w_textbox::OriginalTextGetRef()
{
        // Just to be safe.
        this->TextUpdated();
        return &this->Text;
}

std::string w_textbox::TextGet()
{
        this->ReadyText();
        return AssembleText( this->Text, this->SplitTextCache );
}

std::vector<split_line> w_textbox::LineMapGet()
{
        this->ReadyText();
        return this->SplitTextCache;
}

std::size_t w_textbox::LineCountGet()
{
        this->ReadyText();
        return this->SplitTextCache.size();
}

