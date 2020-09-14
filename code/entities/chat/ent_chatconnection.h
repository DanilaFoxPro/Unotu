#ifndef __UNOTU_ENTITIES_CHAT_ENT_CHATCONNECTION_H_
#define __UNOTU_ENTITIES_CHAT_ENT_CHATCONNECTION_H_

struct ent_chatconnection;

#include <entities\widgets\widget.h>

#include <string>
#include <vector>

struct ent_chatconnection {
        ent_chatconnection() = default;
        virtual ~ent_chatconnection() = default;

        // TODO: Extract all log stuff into MLog.
        std::vector< std::unique_ptr<std::string> > OutputLog;
        std::vector< std::weak_ptr<widget> > EventListeners;

        std::string GetLog();
        void ShiftLog( std::size_t, bool = true );
        void LogLine( const std::string& );
        void LogAppend( const std::string& );

        void AddListener( const std::weak_ptr<widget>& );
        void NotifyListeners();

        virtual void OnTick() {};

};

#endif /* CODE_ENTITIES_CHAT_CHATCONNECTION_H_ */
