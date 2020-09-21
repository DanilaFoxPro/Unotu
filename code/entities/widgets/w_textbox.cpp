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

void w_textbox::OnRefresh( ValidityState_t )
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
                this->OutlineColor.alpha != 0.0f
        );
        
        if( bDrawOutline ) {
                gColor.AddOutline (
                        colored_rectangle( Position, Position2, this->OutlineColor ),
                        this->OutlineThickness
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
        
        const fpoint Pixel = pixel(1);
        const fpoint fFontSize = fpoint( FontSize/2.0f, FontSize ) * Pixel;
        
        // Padding.
        
        this->TextAreaSize = AreaSize( TextArea.first, TextArea.second );
        
        // How much the textbox can hold, in characters.
        const float SpaceWidth = TextAreaSize.x.xratio() / fFontSize.x;
        const float SpaceHeight = TextAreaSize.y.yratio() / fFontSize.y;

        SplitTextCache = SplitTextNew( Text, (std::size_t)SpaceWidth );
        const int HeightLimit = ceilf( SpaceHeight );
        
        const float TopCut = modf( Offset, nullptr );
        const float BottomCut = clamp( (float)modf( this->TotalLineCount-Offset-SpaceHeight, nullptr ), 0.0f, 1.0f );
        const float Cuts = TopCut+BottomCut;
        
        std::size_t NeededLinesCnt = (size_t)HeightLimit;
        if( Cuts >= 1.0f ) {
                NeededLinesCnt++;
        }
        
        // +1 to height limit to accomodate cuts.
        const std::vector<split_line> CutLinesLoc = CutLines( SplitTextCache, NeededLinesCnt, (size_t)Offset );
        
        this->TotalLineCount = SplitTextCache.size();
        this->VisibleRatio = clamp (
                SpaceHeight / (float)(TotalLineCount),
                0.0f,
                1.0f
        );
        
        // Hax!
        const auto CaretPtr = dynamic_cast<m_caret*>( this->Parent.lock().get() );
        
        if( CaretPtr && CaretPtr->HasCaretSelection() ) {
                
                const text_coord First  = CaretPtr->FirstCaretSelection();
                const text_coord Second = CaretPtr->SecondCaretSelection();
                
                      text_coord FirstOffset  = VerticallyOffsetTextCoord( First, static_cast<std::ptrdiff_t>(-Offset) );
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

void w_textbox::UpdateSplitText()
{
        const point TextPadding = point( pixel(3), 0 );
        const fpoint Pixel = pixel(1);
        
        const point Position2 = SecondPosition( Position, Size );
        
        std::pair< point, point > Area = OutlineAdjustedArea( Position, Position2, this->OutlineThickness );
        std::pair< point, point > TextArea = ApplyPadding( Area.first, Area.second, TextPadding );
        
        const fpoint fFontSize = fpoint( FontSize/2.0f, FontSize ) * Pixel;
        this->TextAreaSize = AreaSize( TextArea.first, TextArea.second );
        const float SpaceWidth = TextAreaSize.x.xratio() / fFontSize.x;
        SplitTextCache = SplitTextNew( Text, (std::size_t)SpaceWidth );
}


void w_textbox::OnDraw()
{
        
        //:: Draw colored rectangles.
        this->gColor.Draw();
        
        //:: Draw text.
        this->gText.Draw();

}

void w_textbox::SetOffset( const float& ratio )
{
	this->Offset = 
	(
		(float)(this->TotalLineCount)
		*
		clamp( ratio * (1.0f-this->VisibleRatio), 0.0f, 1.0f )
	);
}

std::size_t w_textbox::LineCount() const
{
	return this->TotalLineCount;
}

text_coord w_textbox::PositionToTextCoord( const fpoint Position )
{
        const fpoint Pixel = pixel(1);
              fpoint FFontSize = FontSize*Pixel;
              FFontSize.x /= 2.0f;
        
        fpoint LocalPosition = Localize( this->Position, Position );
        
        const auto &LineMap = this->GetLineMap();
        
        if( LineMap.size() == 0 ) {
                return {0, 0};
        }
        
        text_coord Result;
        
        Result.first = -LocalPosition.y/FFontSize.y+this->Offset;
        Result.second = LocalPosition.x/FFontSize.x;
        
        if( Result.first >= LineMap.size() ) {
                Result.first = LineMap.size()-1;
        }
        
        if( Result.second > LineMap[Result.first].Length() ) {
                Result.second = LineMap[Result.first].Length();
        }
        
        return Result;
        
}


        //:: Text module.

void w_textbox::SetText( const std::string& Text )
{
        this->Text = Text;
        this->Invalidate( ValidityState::ParametersUpdated );
}

void w_textbox::ClearText()
{
        this->Text.clear();
        this->Invalidate( ValidityState::ParametersUpdated );
}

void w_textbox::TextUpdated()
{
        this->UpdateSplitText();
}


std::string w_textbox::GetOriginalText()
{
        return this->Text;
}

std::string* w_textbox::GetOriginalTextRef()
{
        return &this->Text;
}

std::string w_textbox::GetText()
{
        return AssembleText( this->Text, this->SplitTextCache );
}

std::vector<split_line> w_textbox::GetLineMap()
{
        return this->SplitTextCache;
}
