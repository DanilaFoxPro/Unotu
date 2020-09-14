#include "unit.h"

#include <string>
#include <cmath>

#include <entities\ent_window.h>

#include <utility\shortcuts.h>

//::Constructors.

unit::unit( int pixels_, float ratio_ )
{
	this->i_pixel = pixels_;
	this->i_ratio = ratio_;
}

unit::unit( float ratio_, int pixels_ )
{
	this->i_pixel = pixels_;
	this->i_ratio = ratio_;
}

//::...

int unit::pixels( int limit ) const
{
	return i_pixel + ceil( i_ratio * (float)(limit) );
}

float unit::ratio( int limit ) const
{
	return ( (float)(i_pixel)/(float)(limit) ) + i_ratio;
}

int unit::xpixels() const
{
	const ent_window& the_window = TheWindowManager.Cur();
	return this->pixels( the_window.x );
}

int unit::ypixels() const
{
	const ent_window& the_window = TheWindowManager.Cur();
	return this->pixels( the_window.y );
}

float unit::xratio() const
{
	const ent_window& the_window = TheWindowManager.Cur();
	return this->ratio( the_window.x );
}

float unit::yratio() const
{
	const ent_window& the_window = TheWindowManager.Cur();
	return this->ratio( the_window.y );
}

float unit::percent( int limit ) const
{
	return ratio( limit )*100.0f;
}

std::string unit::string() const
{
	return "('" + std::to_string( i_pixel ) + "', '" + std::to_string( i_ratio ) + "')";
}

void unit::rationalize( int limit )
{
	i_ratio += (float)(i_pixel)/(float)(limit);
	i_pixel = 0;
}

//::Operators.

unit operator+( const unit& unit_1, const unit& unit_2 )
{
	unit result;
	
	result.i_pixel = unit_1.i_pixel + unit_2.i_pixel;
	result.i_ratio = unit_1.i_ratio + unit_2.i_ratio;
	
	return result;
}

unit operator-( const unit& unit_ )
{
	unit result;
	
	result.i_pixel = -unit_.i_pixel;
	result.i_ratio = -unit_.i_ratio;
	
	return result;
}

unit operator-( const unit& unit_1, const unit& unit_2 )
{
	unit result;
	
	result.i_pixel = unit_1.i_pixel - unit_2.i_pixel;
	result.i_ratio = unit_1.i_ratio - unit_2.i_ratio;
	
	return result;
}

unit operator*( const unit& unit_1, const unit& unit_2 )
{
	unit result;
	
	result.i_pixel = unit_1.i_pixel * unit_2.i_pixel;
	result.i_ratio = unit_1.i_ratio * unit_2.i_ratio;
	
	return result;
}

unit operator/( const unit& unit_1, const unit& unit_2 )
{
	unit result;
	
	result.i_pixel = unit_1.i_pixel / unit_2.i_pixel;
	result.i_ratio = unit_1.i_ratio / unit_2.i_ratio;
	
	return result;
}

//

unit& operator+=( unit& Unit1, const unit& Unit2 )
{
	Unit1.i_pixel += Unit2.i_pixel;
	Unit1.i_ratio += Unit2.i_ratio;
	return Unit1;
}
		
unit& operator-=( unit& Unit1, const unit& Unit2 )
{
	Unit1.i_pixel -= Unit2.i_pixel;
	Unit1.i_ratio -= Unit2.i_ratio;
	return Unit1;
}

//::Operators (int).

unit operator*( const unit& a, const int& b )
{
	unit result;
	
	result.i_pixel = a.i_pixel * b;
	result.i_ratio = a.i_ratio * (float)b;
	
	return result;
}
unit operator*( const int& b, const unit& a )
{
	return a*b;
}
unit operator/( const unit& a, const int& b )
{
	unit result;
	
	result.i_pixel = a.i_pixel / b;
	result.i_ratio = a.i_ratio / (float)b;
	
	return result;
}
unit operator/( const int& b, const unit& a )
{
	return a*b;
}

//::Constructors. (?)

unit pixel( int pixelz )
{
	return unit( pixelz, 0.0f );
}

unit ratio( float daratio )
{
	return unit( 0, daratio );
}

unit percent( float percent )
{
	return unit( 0, percent*100.0f );
}
