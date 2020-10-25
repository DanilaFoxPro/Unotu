#ifndef __UNOTU_UTILITY_CLASSES_SIMPLE_TIMER_H_
#define __UNOTU_UTILITY_CLASSES_SIMPLE_TIMER_H_

#include <chrono>

struct simple_timer
{
        
        //:: Constructors.
        
        simple_timer();
        
        //:: Data.
        std::chrono::time_point<std::chrono::high_resolution_clock> InternalStartTime;
        
        //:: Functions.
        
        void Start();
        
        std::chrono::high_resolution_clock::duration ElapsedTime();
        
        uint32_t ElapsedMilliseconds();
        uint32_t ElapsedMicroseconds();
        
};

#endif
