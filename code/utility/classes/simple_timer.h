#ifndef __UNOTU_UTILITY_CLASSES_SIMPLE_TIMER_H_
#define __UNOTU_UTILITY_CLASSES_SIMPLE_TIMER_H_

#include <chrono>

namespace chrono = std::chrono;

struct simple_timer
{
        //:: Constructors.
        
        simple_timer();
        
        //:: Data.
        chrono::time_point<chrono::high_resolution_clock> InternalStartTime;
        
        //:: Functions.
        
        void Start();
        
        chrono::high_resolution_clock::duration ElapsedTime();
        
        uint32_t ElapsedMilliseconds();
        uint32_t ElapsedMicroseconds();
        
};

#endif
