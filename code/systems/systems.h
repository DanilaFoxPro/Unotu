#ifndef __SYSTEMS_H_
#define __SYSTEMS_H_

#include <stdint.h>

#include <utility\executable_enum.h>

#include <unotui.h>

class unotui_bridge : public unotui::ent_interface
{
        virtual void PostSetup() override;
};

namespace systems
{
	void entry( uint8_t Executable = Executable::Undefined );
}// Namepsace system.

#endif
