#ifndef __UNOTU_TAB_PRESETS_CHAT_H_
#define __UNOTU_TAB_PRESETS_CHAT_H_

#include <memory>

#include <entities\widgets\w_tab.h>
#include <entities\widgets\w_textscrollbox.h>
#include <entities\widgets\w_editabletext.h>
#include <entities\chat\ent_chatconnection.h>

namespace TabPresets
{

struct Chat : public w_tab
{
//:: Constructors.
        Chat( bool = false );
        virtual void PostConstruct();
        virtual ~Chat();
        
//:: Overrides.
        virtual void OnTick();
        virtual void OnRefresh( ValidityState_t );
        
        virtual void OnEvent( std::shared_ptr<widget_event> );
        virtual void OnMousePressed( const int );

//:: Data.
        std::shared_ptr<w_textscrollbox> OutputLog;
        std::shared_ptr<w_editabletext> InputField;

        std::unique_ptr< ent_chatconnection > ChatConnection;
        
//:: Options.
        bool bIsServer; // Otherwise client.

};

}// namespace TabPresets

#endif
