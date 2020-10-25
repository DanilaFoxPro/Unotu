#ifndef __UNOTU_ENTITIES_WIDGETS_W_GENERICSCROLLBOX_H_
#define __UNOTU_ENTITIES_WIDGETS_W_GENERICSCROLLBOX_H_

#include <entities\widgets\widget.h>
#include <entities\widgets\w_buffer.h>
#include <entities\widgets\w_scrollbar.h>

#include <utility\classes\simple_timer.h>

struct w_genericscrollbox : public widget
{
//:: Constructors.
        w_genericscrollbox() = default;
        w_genericscrollbox( point, point, unit, int );
        void PostConstruct();
//:: Data.
        unit ItemHeight = pixel( 200 );
        int ItemPadding = 8;
        int XPadding = 8;
        
                /** How many milliseconds it takes for scrolling to arrive at target. */
        unsigned int ScrollSpeed = 200;
        simple_timer ScrollTimer;
        double ScrollOffset = 0.0f;
        
        std::shared_ptr<w_buffer> Buffer;
        std::shared_ptr<w_scrollbar> Scrollbar;
        
        std::vector< std::shared_ptr<widget> > Items;
//:: Overrides.
        virtual void OnTick();
        virtual void OnRefresh( ValidityState_t );
        virtual void OnEvent( std::shared_ptr<widget_event> );
        
        std::shared_ptr<widget>& operator[]( std::size_t );
//:: Functions.
        void AddItem( std::shared_ptr<widget> );
        void ClearItems();
        
        float ItemHeightRatio();
        float ItemYPaddingRatio();
        float VisibleItemsCount();
        float ScrollLength();
        std::size_t NextItem( float );
        float ItemOffset( std::size_t );
        
        void SetBackgroundColor( const rgba& Color );
        
};

#endif
