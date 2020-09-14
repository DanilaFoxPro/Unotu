#ifndef __UNOTU_ENTITIES_ENT_APPLICATION_H_
#define __UNOTU_ENTITIES_ENT_APPLICATION_H_

#include <string>

#include <SQLiteCpp\SQLiteCpp.h>

#include <utility\classes\simple_timer.h>
#include <utility\executable_enum.h>

struct ent_application
{
        const std::string Name = "Unotu";
          /** Timer that counts the time it takes to render a frame. */
        simple_timer FrameTimer;
          /** Database that stores the cards. */
        std::unique_ptr<SQLite::Database> Database;

        uint8_t Executable = Executable::Undefined; // Executable from which the application is running.
};

extern ent_application TheApplication;

#endif
