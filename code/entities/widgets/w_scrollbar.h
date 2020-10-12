#ifndef __W_SCROLLBAR_H_
#define __W_SCROLLBAR_H_

#include <entities\widgets\widget.h>

struct w_scrollbar : public widget
{

//:: Constants.
        static const int IdealWidth = static_cast<int>(32);
        static const int IdealPreviewWidth = static_cast<int>(12);
        
//:: Scrolling.
        double ScrollViewzone = 0.2;
        double ScrollOffset = 0.0;
        
//:: Input.
        bool bHasMouse = false;
        
//:: Geometry.
        color_geometry gColor = color_geometry(GL_STREAM_DRAW);
        text_geometry gText;
        
        color_geometry gPreviewColor = color_geometry(GL_STREAM_DRAW);
        
//:: Functions.
        
        w_scrollbar() = default;
        
        w_scrollbar(
                point position_,
                point size_,
                int layer_ = 0
        )
        {
                Position = position_;
                Size = size_;
                Layer = layer_;
        }
        
        virtual void OnMousePressed( const int );
        virtual void OnMouseReleased( const int, const bool );
        
        virtual bool DoesHintCollide( point );
        
        virtual void OnTick();
        virtual void OnRefresh( ValidityState_t );
        virtual void OnDraw();
        
        void OffsetByViewzone( double Ratio );
        void OffsetByRatio( double Ratio );
        
        double ViewzoneOffset() const;
        double MaximumOffset() const;
        
};

#endif
