#include <entities\widgets\w_horizontalbox.h>

#include <utility\widget.h>

void w_horizontalbox::OnRefresh( ValidityState_t Reason )
{
        if( !( Reason & (ValidityState::Resized | ValidityState::ChildrenUpdated) ) ) {
                return;
        }
        
        if( this->Children.size() == 0 ) {
                return;
        }
        
        /* TODO: Move as much of this as possible into separate function(s),
         *       so I don't have to copy stuff when making 'w_verticalbox'.
         */
        
        // Makes sure all widgets have weight.
        this->Weights.resize( this->Children.size(), 1.0f );
        
        float TotalWeight = 0.0f;
        for( float Cur : this->Weights ) {
                TotalWeight += Cur;
        }
        
        const fpoint FPosition = this->Position;
        const fpoint FPosition2 = SecondPosition( this->Position, this->Size );
        const fpoint FSize = this->Size;
        
        const fpoint Padding = this->Padding;
        
        // Range of the space on which to put widgets. (Takes care of X padding.)
        std::pair<float, float> Range = std::make_pair( FPosition.x, FPosition2.x );
        if( this->bPadSides ) {
                Range.first += Padding.x;
                Range.second -= Padding.x;
        }
        
        // Sum of all spaces the widgets will occupy, minus the padding.
        const float PaddedRange = Range.second-Range.first-Padding.x*(this->Children.size()-1);
        // This always results in a valid size when multiplied by any child's weight.
        const float WeightedRange = PaddedRange / TotalWeight;
        
        float CurrentOffset = Range.first;
        
        for( std::size_t i = 0; i < this->Children.size(); i++ ) {
                std::shared_ptr<widget>& Current = this->Children[i];
                
                const fpoint Position = fpoint( CurrentOffset, FPosition.y-Padding.y );
                const fpoint Size = fpoint( WeightedRange*this->Weights[i], FSize.y-Padding.y*2.0f );
                
                CurrentOffset += Size.x+Padding.x;
                
                Current->Position = Position;
                Current->Size = Size;
                
                Current->Invalidate( ValidityState::Resized );
        }
        
}
