#ifndef __FPOINT_H_
#define __FPOINT_H_

#include <utility\point.h>

struct fpoint
{
	float x, y;
	fpoint();
	fpoint( const float, const float );
	fpoint( const point& );
        fpoint( const unit& );
	
	std::string string() const;
};

fpoint operator+( const fpoint& , const fpoint& );
fpoint operator-( const fpoint& );
fpoint operator-( const fpoint& , const fpoint& );
fpoint& operator+=( fpoint& , const fpoint& );
fpoint& operator-=( fpoint& , const fpoint& );
fpoint operator*( const fpoint& , const fpoint& );
fpoint operator/( const fpoint& , const fpoint& );

fpoint operator*( const fpoint, const int );
fpoint operator/( const fpoint, const int );
fpoint operator*( const int, const fpoint );
fpoint operator/( const int, const fpoint );

fpoint operator*( const fpoint, const float );
fpoint operator/( const fpoint, const float );
fpoint operator*( const float, const fpoint );
fpoint operator/( const float, const fpoint );

#endif
