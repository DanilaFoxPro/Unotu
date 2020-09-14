#ifndef __SYSTEMS_H_
#define __SYSTEMS_H_

#include <stdint.h>

#include <utility\executable_enum.h>

namespace systems
{
	void entry( uint8_t Executable = Executable::Undefined );
        void terminate();
}// Namepsace system.

#endif
