target_sources_local(
  ${APP_LIBRARY_NAME}
  PUBLIC
    ent_application.h
    ent_tab.h
    ent_opengl.h
    ent_window.h
  PRIVATE
    ent_application.cpp
    ent_tab.cpp
    ent_opengl.cpp
    ent_window.cpp
)

add_subdirectory( chat )
add_subdirectory( modules )
add_subdirectory( widgets )
