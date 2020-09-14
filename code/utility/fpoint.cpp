#include <utility\fpoint.h>

//:: Constructors.

fpoint::fpoint()
{
        this->x = 0.0f;
        this->y = 0.0f;
}

fpoint::fpoint( const float x, const float y )
{
        this->x = x;
        this->y = y;
}

fpoint::fpoint( const point& p )
{
        this->x = p.x.xratio();
        this->y = p.y.yratio();
}

fpoint::fpoint( const unit& Unit )
{
        this->x = Unit.xratio();
        this->y = Unit.yratio();
}


std::string fpoint::string() const
{
	return "< " + std::to_string( this->x ) + ", " + std::to_string( this->y ) + " >"; 
}

//:: Operators.

fpoint operator+( const fpoint& fpoint_1, const fpoint& fpoint_2 )
{
	return fpoint( fpoint_1.x + fpoint_2.x, fpoint_1.y + fpoint_2.y );
}

fpoint operator-( const fpoint& fpoint_ )
{
	return fpoint( -fpoint_.x, -fpoint_.y );
}

fpoint operator-( const fpoint& fpoint_1, const fpoint& fpoint_2 )
{
	return fpoint( fpoint_1.x - fpoint_2.x, fpoint_1.y - fpoint_2.y );
}

fpoint& operator+=( fpoint& fpoint_1, const fpoint& fpoint_2 )
{
	fpoint_1 = fpoint_1 + fpoint_2;
	return fpoint_1;
}

fpoint& operator-=( fpoint& fpoint_1, const fpoint& fpoint_2 )
{
	fpoint_1 = fpoint_1 - fpoint_2;
	return fpoint_1;
}

fpoint operator*( const fpoint& fpoint_1, const fpoint& fpoint_2 )
{
	return { fpoint_1.x * fpoint_2.x, fpoint_1.y * fpoint_2.y };
}

fpoint operator/( const fpoint& fpoint_1, const fpoint& fpoint_2 )
{
	return { fpoint_1.x / fpoint_2.x, fpoint_1.y / fpoint_2.y };
}

fpoint operator*( const fpoint Point, const int Integer )
{
        return { Point.x * Integer, Point.y * Integer };
}

fpoint operator/( const fpoint Point, const int Integer )
{
        return { Point.x / Integer, Point.y / Integer };
}

fpoint operator*( const int Integer, const fpoint Point )
{
        return { Point.x * Integer, Point.y * Integer };
}

fpoint operator/( const int Integer, const fpoint Point )
{
        return { Point.x / Integer, Point.y / Integer };
}


fpoint operator*( const fpoint Point, const float Float )
{
        return fpoint( Point.x*Float, Point.y*Float );
}

fpoint operator/( const fpoint Point, const float Float )
{
        return fpoint( Point.x/Float, Point.y/Float );
}

fpoint operator*( const float Float, const fpoint Point )
{
        return fpoint( Point.x*Float, Point.y*Float );
}

fpoint operator/( const float Float, const fpoint Point )
{
        return fpoint( Point.x/Float, Point.y/Float );
}
