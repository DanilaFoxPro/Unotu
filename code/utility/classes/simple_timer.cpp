#include <utility\classes\simple_timer.h>

namespace chrono = std::chrono;

simple_timer::simple_timer()
{
        this->Start();
}

void simple_timer::Start()
{
        this->InternalStartTime = chrono::high_resolution_clock::now();
}

chrono::high_resolution_clock::duration simple_timer::ElapsedTime()
{
        const auto TimePassed = chrono::high_resolution_clock::now() - this->InternalStartTime;
        
        return chrono::duration_cast<chrono::high_resolution_clock::duration>( TimePassed );
        
}


uint32_t simple_timer::ElapsedMicroseconds()
{
        return chrono::duration_cast<chrono::microseconds>( this->ElapsedTime() ).count();
}

uint32_t simple_timer::ElapsedMilliseconds()
{
        return chrono::duration_cast<chrono::milliseconds>( this->ElapsedTime() ).count();
}
