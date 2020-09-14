#include <utility\point.h>

#include <utility\fpoint.h>

point::point()
{
	x = pixel(0);
	y = pixel(0);
}

point::point( const unit& xy )
{
	x = xy;
	y = xy;
}

point::point( const unit& xn, const unit& yn )
{
	x = xn;
	y = yn;
}

point::point( const fpoint& FPoint )
{
        this->x = FPoint.x;
        this->y = FPoint.y;
}


std::string point::string() const
{
	return "< "
			+ x.string()
			+ ", "
			+ y.string()
			+ " >";
}

void point::rationalize( int x_limit, int y_limit )
{
	x.rationalize( x_limit );
	y.rationalize( y_limit );
}

point operator+( const point& point_1, const point& point_2 )
{
	return { point_1.x + point_2.x, point_1.y + point_2.y };
}

point operator-( const point& point_ )
{
	return { -point_.x, -point_.y };
}

point operator-( const point& point_1, const point& point_2 )
{
	return { point_1.x - point_2.x, point_1.y - point_2.y };
}

point operator*( const point& point_1, const point& point_2 )
{
	return { point_1.x * point_2.x, point_1.y * point_2.y };
}
point operator/( const point& point_1, const point& point_2 )
{
	return { point_1.x / point_2.x, point_1.y / point_2.y };
}

point operator*( const point& point_1, const int& integer )
{
	return point( point_1.x * integer, point_1.y*integer );
}

point operator/( const point& point_1, const int& integer )
{
	return point( point_1.x / integer, point_1.y/integer );
}
