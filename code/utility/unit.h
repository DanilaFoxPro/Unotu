#ifndef __UNIT_H_
#define __UNIT_H_

#include <string>

struct unit
{
	
	//::Data.
	int 	i_pixel;
	float 	i_ratio;
	
	//::Shortcut constructors.
	unit( int = 0, float = 0.0f );
	unit( float, int = 0 );
	
	//::Conversions.
	int pixels( int limit ) const;
	float ratio( int limit ) const;
	
	int xpixels() const;
	int ypixels() const;
	
	float xratio() const;
	float yratio() const;
	
	float percent( int limit ) const;
	
	void rationalize( int limit );
	std::string string() const;
        
};

unit operator+( const unit& , const unit& );
unit operator-( const unit& );
unit operator-( const unit& , const unit& );
unit operator*( const unit& , const unit& );
unit operator/( const unit& , const unit& );

unit& operator+=( unit&, const unit& );
unit& operator-=( unit&, const unit& );

unit operator*( const unit& , const int& );
unit operator*( const int& ,  const unit&  );
unit operator/( const unit& , const int& );
unit operator/( const int& ,  const unit& );

unit pixel( int pixelz );
unit ratio( float daratio );
unit percent( float percent );

#endif
