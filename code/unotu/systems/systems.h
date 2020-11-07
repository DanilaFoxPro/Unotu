#ifndef __UNOTU_SYSTEMS_SYSTEMS_H_
#define __UNOTU_SYSTEMS_SYSTEMS_H_

#include <stdint.h>

#include <unotu\utility\executable_enum.h>

#include <unotu\utility\deps\unotui_includes.h>

class unotui_bridge : public unotui::ent_interface
{
        virtual void PostSetup() override;
        
        virtual void OnNewWindow( unotui::ent_window& Window );
};

namespace systems
{
	void entry( uint8_t Executable = Executable::Undefined );
}// namespace systems

#endif
