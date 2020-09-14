#ifndef __W_TEXT_H_
#define __W_TEXT_H_

#include <entities\widgets\widget.h>
#include <utility\colors.h>
#include <stdio.h>

struct w_text : widget
{
	//Data.
	
	std::string text;
	rgba color;
	int font_size;
	
	text_geometry gText;
	
	//Constructor.
	
	w_text(void)
	{
		color = color::black;
		font_size = 16;
		
		Position = point( 0, 0 );
		Size = point( 16, 16 ); // Updated on 'UpdateGeometry()'.
		Layer = 0;
	}
	w_text(
		point position_,
		std::string text_ = "",
		int font_size_ = 32,
		int layer_ = 0,
		rgb color_ = {0.0f, 0.0f, 0.0f},
		float alpha = 1.0f
	)
	{
		text = text_;
		color = rgba( color_, alpha );
		font_size = font_size_;
		
		//Size is set on 'updateGeometry()'.
		Position = position_;
		Layer = layer_;
		bSolid = false;//TODO: Just testing.
	}
	
	//Functions.
	
	void OnDraw();
	
	void OnRefresh( ValidityState_t );
	
};

#endif
