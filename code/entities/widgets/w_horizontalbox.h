#ifndef __UNOTU_ENTITIES_WIDGETS_W_HORIZONTALBOX_H_
#define __UNOTU_ENTITIES_WIDGETS_W_HORIZONTALBOX_H_

#include <entities\widgets\widget.h>

/**
 * @brief Evenly distrubutes the space between the children, horizontally.
 * @note  Children sorted by layer.
 */
struct w_horizontalbox : public widget
{
//:: Constructors.
        w_horizontalbox() = default;
//:: Data.
        point Padding = pixel(0);
        bool bPadSides = false;
        std::vector<float> Weights;
//:: Overrides.
        virtual void OnRefresh( ValidityState_t );
};

#endif
