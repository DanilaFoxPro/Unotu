#ifndef __W_RECTANGLE_H_
#define __W_RECTANGLE_H_

#include <entities\widgets\widget.h>

struct w_rectangle : widget
{
	//Data.
	
	rgb color;
	
	color_geometry Geometry;
	
	//Constructor.
	
	w_rectangle( point position_, point size_, rgb colour, int layer_ = 0 )
	{
		Position = position_;
		Size = size_;
		Layer = layer_;
		
		color = colour;
	}
	
	//Functions.
	
	void OnDraw();
	
	void OnRefresh( ValidityState_t );
	std::vector<colored_rectangle> getRectangles();
};

#endif
