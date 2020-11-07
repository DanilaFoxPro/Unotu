#ifndef __UNOTU_ENTITIES_ENT_APPLICATION_H_
#define __UNOTU_ENTITIES_ENT_APPLICATION_H_

#include <string>

#include <SQLiteCpp\SQLiteCpp.h>

#include <unotu\utility\executable_enum.h>

struct ent_application
{
        const std::string Name = "Unotu";
                /** Database that stores the cards. */
        std::unique_ptr<SQLite::Database> Database;

        uint8_t Executable = Executable::Undefined; // Executable from which the application is running.
};

extern ent_application TheApplication;

#endif
