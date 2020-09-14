#ifndef __POINT_H_
#define __POINT_H_

#include <utility\unit.h>

struct fpoint;
/** @brief Represents a UI point. Stored as Ratio+Pixels.
 */
struct point
{
	unit x;
	unit y;
	point();
	point( const unit& );
	point( const unit&, const unit& );
        point( const fpoint& );
	
	std::string string() const;
	
	void rationalize( int x_limit, int y_limit );
};

point operator+( const point& , const point& );
point operator-( const point& );
point operator-( const point& , const point& );
point operator*( const point& , const point& );
point operator/( const point& , const point& );

point operator*( const point& , const int& );
point operator/( const point& , const int& );

#endif
