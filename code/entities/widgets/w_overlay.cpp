#include <entities\widgets\w_overlay.h>

#include <utility\text.h>
#include <workers\widget_worker.h>
#include <workers\window_worker.h> // Mouse position.
#include <utility\widget.h>
#include <entities\widgets\w_editabletextbox.h>

#include <utility\colors.h>

void w_overlay::CheckpointFrame()
{
        
        namespace chrono = std::chrono;
        
        const chrono::high_resolution_clock::duration AvgPeriod =
                this->FrameTimer.ElapsedTime()/this->FPSAverageStep;
        this->AverageFPS = chrono::seconds(1)/AvgPeriod;
        this->FrameTimer.Start();
        this->Invalidate();
}

/**
 * @brief Creates a label-like text with background at given position.
 * @note  Should only be called 'OnReferesh()'.
 */
void w_overlay::CreateLabel(
        const std::string&      Label,
        const point             Origin,
        const int               FontSize,
        const rgba              Color,
        const rgba              BackgroundColor
) {
        
        if( Label.size() == 0 ) {
                return;
        }
        
        const std::size_t LabelWidth  = LongestLine(Label);
        const std::size_t LabelHeight = CountCharacter(Label, '\n')+1;
        
        //:: Background.
        
        gColor.AddRectangle (
                        colored_rectangle (
                        Origin,
                        SecondPosition
                        (
                                Origin,
                                point
                                (
                                        pixel(FontSize*LabelWidth/2),
                                        pixel(FontSize*LabelHeight)
                                )
                        ),
                        BackgroundColor
                )
        );
        
        //:: FPS.
        
        this->gText.AddText( Label, FontSize, Origin, Color );
}


void w_overlay::OnTick()
{
        
        if( bDisplayFrameCount || bDisplayMouse ) {
                this->Invalidate();
        }
        
        if( this->bDisplayFPS ) {
                const ent_window& TheWindow = TheWindowManager.Cur();
                
                if( ( TheWindow.CurrentFrame % this->FPSAverageStep ) == 0 ) {
                        this->CheckpointFrame();
                }
        }
        
}

void w_overlay::OnRefresh( ValidityState_t )
{
        gColor.Clear();
        gText.Clear();
        
        const ent_window& TheWindow = TheWindowManager.Cur();
        
        std::string TopLeftLabel;
        std::string TopRightLabel;
        
        if( this->bDisplayFPS ) {
                //:: Stuff.
        
                std::string FPS_STR = std::to_string(this->AverageFPS);
                std::size_t dot_position = FPS_STR.find( '.' );
                
                if( dot_position != std::string::npos ) {
                        FPS_STR.erase( dot_position+2 ); // Keeps the dot and one digit after it.
                }
                
                TopLeftLabel += FPS_STR + "\n";
                
        } // If display FPS.
        
        if( this->bDisplayCurrentWindow ) {
                TopLeftLabel += "Cur: " + std::to_string( TheWindowManager.CurIndex() ) + "\n";
        }
        
        if( this->bDisplayFrameCount ) {
                TopRightLabel += std::to_string( TheWindow.CurrentFrame ) + "\n";
        } // If display frame count.
        
        if( this->bDisplayMouse )
        {
                const point MousePos = MousePosition();
                        
                gColor.AddRectangle (
                                colored_rectangle (
                                MousePos,
                                SecondPosition( MousePos, pixel(8) ),
                                color::red
                        )
                );
                
                // Textbox debug.
                ent_window& TheWindow = TheWindowManager.Cur();
                
                if( !TheWindow.KeyboardFocus.expired() ) {
                        
                        const std::shared_ptr<widget> Locked = TheWindow.KeyboardFocus.lock();
                        w_editabletextbox* TextBox = nullptr;
                        
                        if(
                                ( TextBox = dynamic_cast<w_editabletextbox*>(Locked.get()) )
                                && TextBox->DoesCollide(MousePos)
                        ) {
                                
                                const text_coord TextCoord = TextBox->TextBox->PositionToTextCoord(MousePos);
                                const std::string CoordText = std::to_string( TextCoord.first ) + ":" + std::to_string( TextCoord.second );
                                
                                const point Origin = MousePos+point(8,-8);
                                this->CreateLabel( CoordText, Origin );
                                
                        }
                }
                
        } // If display mouse.
        
        // Remove trailing newlines from labels.
        
        if( TopLeftLabel.size() && TopLeftLabel[TopLeftLabel.size()-1] == '\n' ) {
                TopLeftLabel.pop_back();
        }
        
        if( TopRightLabel.size() && TopRightLabel[TopRightLabel.size()-1] == '\n' ) {
                TopRightLabel.pop_back();
        }
        
        // Add labels.
        
        const point TopLeft  = point( ratio(0.0f), ratio(1.0f) );
        const point TopRight = point( ratio(1.0f)-pixel(LongestLine(TopRightLabel)*this->DefaultFontSize/2), ratio(1.0f) );
        
        this->CreateLabel( TopLeftLabel,  TopLeft );
        this->CreateLabel( TopRightLabel, TopRight );
        
        gColor.Update();
        gText.Update();
        
}

void w_overlay::OnDraw()
{
        
        //:: Draw rectangles.
        this->gColor.Draw();
        
        //:: Draw text.
        this->gText.Draw();
}
