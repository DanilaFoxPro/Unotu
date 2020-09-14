#ifndef __ENT_INTERFACE_H_
#define __ENT_INTERFACE_H_

#include <memory>
#include <vector>
#include <atomic>

#include <entities\widgets\widget.h>
#include <entities\widgets\w_tab.h>

struct release_listener
{
	int Button;
	std::weak_ptr<widget> Listener;
};

struct ent_tab
{
        //::Widgets.
        w_tab* PendingTab = nullptr;
        
        std::vector< std::shared_ptr<widget> > Widgets;
        
        std::shared_ptr<widget> LastHint;
        std::vector< release_listener > MouseReleaseListeners;
        
        //::Constructors.
        ent_tab() = default;
        virtual ~ent_tab() = default;
        
        //::Functions.
        std::weak_ptr<widget> AddWidget( widget* const );
        void SwitchTab( w_tab* );
        
        std::shared_ptr<widget>& operator[]( std::size_t index );
                
};

#endif
