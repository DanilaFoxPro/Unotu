#ifndef __UNOTU_ENTITIES_WIDGETS_W_SCROLLABLE_H_
#define __UNOTU_ENTITIES_WIDGETS_W_SCROLLABLE_H_

#include <entities\widgets\widget.h>
#include <entities\widgets\w_buffer.h>

struct w_scrollable : public widget
{
//:: Constructors.
        virtual void PostConstruct();
//:: Data.
        std::shared_ptr<w_buffer> Buffer;
//:: Helper functions.
        void AddScrollableChild( std::shared_ptr<widget> );
};

#endif
