#include <utility\shortcuts.h>

#include <cmath>

//:: Other.

#ifdef __GNUG__
#  include <cstdlib>
#  include <memory>
#  include <cxxabi.h>
#endif

std::string DemangleClassName( const char* Name )
{
#ifdef __GNUG__
    int status = -4; // Some arbitrary value to eliminate the compiler warning.

    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(Name, NULL, NULL, &status),
        std::free
    };

    return (status==0) ? res.get() : Name ;
#else
    return std::string( Name );
    
#endif
}
