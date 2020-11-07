target_sources_local(
        ${APP_LIBRARY_NAME}
        PUBLIC
                ent_application.h
        PRIVATE
                ent_application.cpp
)


add_subdirectory( widgets )
