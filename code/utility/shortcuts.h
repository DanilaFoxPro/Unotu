#ifndef __SHORTCUTS_H_
#define __SHORTCUTS_H_

#include <vector>
#include <atomic>
#include <memory>

#include <utility\point.h>

//:: Mathematics.

//float clamp( const float& value, const float& minlim, const float& maxlim );

template< class T >
T min( const T& A, const T& B )
{
    return A < B ? A : B;
}

template< class T >
T max( const T& A, const T& B )
{
    return A > B ? A : B;
}

template< class T >
T clamp( const T& Value, const T& Min, const T& Max )
{
    return min( Max, max( Value, Min ) );
}

template< class T >
T lerp( const T& Value, const T& Target, const T& Alpha )
{
        const T Difference = Target-Value;
        return Value + Difference * Alpha;
}

template< class T >
int GreatestCommonDivisor( T a, T b ) 
{ 
    if ( b == T(0) )
    {
    	return a;
    }
    else
    {
    	return GreatestCommonDivisor(b, a % b);
    }
}

//:: Vector operators.

template < class Type, class Allocator >
std::vector<Type, Allocator> operator+( const std::vector<Type, Allocator>& A, const std::vector<Type, Allocator>& B )
{
	std::vector<Type, Allocator> Result = A;
	Result.insert( Result.end(), B.begin(), B.end() );
	return Result;
}

template < class Type, class Allocator >
std::vector<Type, Allocator>& operator+=( std::vector<Type, Allocator>& A, const std::vector<Type, Allocator>& B )
{
	A.insert( A.end(), B.begin(), B.end() );
	return A;
}

//:: Vector.

template < class Type >
std::vector< Type* > ResolveShared( const std::vector<std::shared_ptr<Type>>& Shared )
{
        std::vector< Type* > Resolved;
        Resolved.reserve( Shared.size() );
        
        for( std::size_t i = 0; i < Shared.size(); i++ ) {
                Resolved.push_back( Shared[i].get() );
        }
        
        return Resolved;
}

//:: Multithreading.

/*
 * @brief Simple double buffer implementation.
 * 
 * I'm not sure if it's thread safe at all.
 * 
 * */
template < class Type >
struct double_buffer
{
	Type Buffer1;
	Type Buffer2;
	
	bool bReadBuffer;
	
	double_buffer() = default; // @suppress("Class members should be properly initialized")
	
	Type& ReadBuffer()
	{
		return bReadBuffer ? Buffer1 : Buffer2;
	}
	Type& WriteBuffer()
	{
		if( bReadBuffer )
		{
			return Buffer2;
		}
		else
		{
			return Buffer1;
		}
	}
	void Swap()
	{
		bReadBuffer = !bReadBuffer;
	}
	void FlushBoth()
	{
		Buffer1 = Type();
		Buffer2 = Type();
	}
};

//:: Other.

std::string DemangleClassName( const char* );

template< class T >
std::string ClassName( const T& Class )
{
	return DemangleClassName( typeid(Class).name() );
}

#endif
