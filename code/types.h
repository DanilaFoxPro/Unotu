#ifndef __TYPES_H_
#define __TYPES_H_

#include <utility\fpoint.h>

/** Type to use for text coordinates, in YX format. */
using text_coord = std::pair<std::size_t, std::size_t>;
using line_coord = std::size_t;

struct rgb
{
        union{ float red, r; };
        union{ float green, g; };
        union{ float blue, b; };
        rgb()
        {
                this->red = this->green = this->blue = 0.0f;
        }
        rgb( float Red, float Green, float Blue )
        {
                this->red	= Red;
                this->green	= Green;
                this->blue	= Blue;
        }
};

rgb operator+( const rgb&, const rgb& );
rgb operator-( const rgb&, const rgb& );
rgb operator/( const rgb&, const rgb& );
rgb operator*( const rgb&, const rgb& );

rgb operator+( const rgb&, const float& );
rgb operator-( const rgb&, const float& );
rgb operator/( const rgb&, const float& );
rgb operator*( const rgb&, const float& );

rgb operator~( const rgb& );

rgb red( float Multiplier );
rgb green( float Multiplier );
rgb blue( float Multiplier );

struct rgba : public rgb
{
	union{ float alpha, a; };
	rgba( rgb color, float alpha = 1.0f )
	{
		this->r = color.r;
		this->g = color.g;
		this->b = color.b;
		this->a = alpha;
	}
	explicit rgba(
		float r_ = 0.0f,
		float g_ = 0.0f,
		float b_ = 0.0f,
		float a_ = 1.0f
	)
	{
		this->r = r_;
		this->g = g_;
		this->b = b_;
		this->a = a_;
	}
};

struct vertex
{
        vertex() = default;
        vertex( float x, float y ) : x{x}, y{y} {};
        float x, y;
};

vertex operator+( const vertex&, const vertex& );
vertex operator-( const vertex&, const vertex& );

struct colored_vertex : public vertex
{
        colored_vertex() = default;
        colored_vertex( float x, float y, rgba Color = {} ) : color{Color} { this->x = x; this->y = y; }
        colored_vertex( const vertex& Vertex, rgba Color = {} ) : color{Color} { this->x = Vertex.x; this->y = Vertex.y; }
	rgba color;
};

struct textured_vertex : public vertex
{
        textured_vertex() = default;
        textured_vertex( float x, float y, float tx, float ty ) : tx{tx}, ty{ty} { this->x = x; this->y = y; }
	float tx, ty;
};

struct rectangle_tc
{
	//Texture coordinates for top-left and bottom-right.
	float x1, y1;
	float x2, y2;
};

struct point;
struct rectangle
{
        //Top-left and bottom-right vertex positions.
	float x1, y1;
	float x2, y2;
	// float z;
        
        // Utility functions.
        rectangle() = default;
        rectangle( float, float, float, float );
        rectangle( const fpoint&, const fpoint& );
        rectangle( const point&, const point& );
};
struct colored_rectangle : public rectangle
{
	//...
	rgba color;
	
	//Utility functions.
        colored_rectangle() = default;
        colored_rectangle( float, float, float, float, rgba = rgba() );
        colored_rectangle( const fpoint&, const fpoint&, rgba = rgba() );
        colored_rectangle( point, point, rgba );
};

struct textured_rectangle : public rectangle
{
	//...
	union{ rectangle_tc texture_coordinates; rectangle_tc tc; };
	
	//Utility functions.
	textured_rectangle() = default;
	textured_rectangle( point, point, rectangle_tc );
	textured_rectangle( float, float, float, float, rectangle_tc );
};

#endif
