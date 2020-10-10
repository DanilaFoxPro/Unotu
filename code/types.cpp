#include <types.h>

#include <coordinates.h>
#include <entities\ent_window.h>

//:: RGB operators.

rgb operator+( const rgb& Color1, const rgb& Color2 )
{
	return
	{
		Color1.r + Color2.r,
		Color1.g + Color2.g,
		Color1.b + Color2.b
	};
}

rgb operator-( const rgb& Color1, const rgb& Color2 )
{
	return
	{
		Color1.r - Color2.r,
		Color1.g - Color2.g,
		Color1.b - Color2.b
	};
}

rgb operator/( const rgb& Color1, const rgb& Color2 )
{
	return
	{
		Color1.r / Color2.r,
		Color1.g / Color2.g,
		Color1.b / Color2.b
	};
}

rgb operator*( const rgb& Color1, const rgb& Color2 )
{
	return
	{
		Color1.r * Color2.r,
		Color1.g * Color2.g,
		Color1.b * Color2.b
	};
}

rgb operator+( const rgb& Color, const float& Offset )
{
	return
	{
		Color.r + Offset,
		Color.g + Offset,
		Color.b + Offset
	};
}

rgb operator-( const rgb& Color, const float& Offset )
{
	return
	{
		Color.r - Offset,
		Color.g - Offset,
		Color.b - Offset
	};
}

rgb operator/( const rgb& Color, const float& Divider )
{
	return
	{
		Color.r / Divider,
		Color.g / Divider,
		Color.b / Divider
	};
}

rgb operator*( const rgb& Color, const float& Multiplier )
{
	return
	{
		Color.r * Multiplier,
		Color.g * Multiplier,
		Color.b * Multiplier
	};
}

rgb operator~( const rgb& Color )
{
        return rgb(
                1.0f-Color.r,
                1.0f-Color.g,
                1.0f-Color.b
        );
}


        //:: RGB functions.

rgb red( float Multiplier )
{
	return
	{
		Multiplier,
		0.0f,
		0.0f
	};
}

rgb green( float Multiplier )
{
	return
	{
		0.0f,
		Multiplier,
		0.0f
	};
}

rgb blue( float Multiplier )
{
	return
	{
		0.0f,
		0.0f,
		Multiplier
	};
}

        //:: Vertex operators.

vertex operator+( const vertex& A, const vertex& B )
{
        return vertex(
                A.x + B.x,
                A.y + B.y
        );
}

vertex operator-( const vertex& A, const vertex& B )
{
        return vertex(
                A.x - B.x,
                A.y - B.y
        );
}

        //:: Rectangle constructors.

rectangle::rectangle( float x1, float y1, float x2, float y2 )
{
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;
}

rectangle::rectangle( const fpoint& Position, const fpoint& Position2 )
{
	this->x1 = Position.x;
	this->y1 = Position.y;
	
	this->x2 = Position2.x;
	this->y2 = Position2.y;
}

rectangle::rectangle( const point& Position, const point& Position2 )
{
	
	this->x1 = Position.x.xratio();
	this->y1 = Position.y.yratio();
        
	this->x2 = Position2.x.xratio();
	this->y2 = Position2.y.yratio();
	
}

        //:: Colored rectangle constructors.

colored_rectangle::colored_rectangle( float x1_, float y1_, float x2_, float y2_, rgba color_ )
{
	x1 = x1_;
	y1 = y1_;
	
	x2 = x2_;
	y2 = y2_;
	
	color = color_;
	
}

colored_rectangle::colored_rectangle( const fpoint& Position, const fpoint& Position2, rgba Color )
{
	x1 = Position.x;
	y1 = Position.y;
	
	x2 = Position2.x;
	y2 = Position2.y;
	
	color = Color;
}

colored_rectangle::colored_rectangle( point pos, point pos2, rgba color_ )
{
	
	const ent_window& the_window = TheWindowManager.Cur();
	
	x1 = pos.x.ratio( the_window.x );
	y1 = pos.y.ratio( the_window.y );
		
	x2 = pos2.x.ratio( the_window.x );
	y2 = pos2.y.ratio( the_window.y );
	
	color = color_;
	
}

        //:: Textured rectangle functions.

textured_rectangle::textured_rectangle( point pos, point pos2, rectangle_tc tc_ )
{
	
	const ent_window& the_window = TheWindowManager.Cur();
	
	x1 = pos.x.ratio( the_window.x );
	y1 = pos.y.ratio( the_window.y );
		
	x2 = pos2.x.ratio( the_window.x );
	y2 = pos2.y.ratio( the_window.y );
	
	tc = tc_;
	
}

textured_rectangle::textured_rectangle( float x1_, float y1_, float x2_, float y2_, rectangle_tc tc_ )
{
	
	x1 = x1_;
	y1 = y1_;
	
	x2 = x2_;
	y2 = y2_;
	
	tc = tc_;
	
}
