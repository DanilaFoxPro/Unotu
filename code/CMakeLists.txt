target_sources_local(
  ${APP_LIBRARY_NAME}
  PUBLIC
    coordinates.h
    font_symbols.h
    item.h
    types.h
    write.h
  PRIVATE
    item.cpp
    types.cpp
    write.cpp
)

add_subdirectory( entities )
add_subdirectory( events )
add_subdirectory( systems )
add_subdirectory( utility )
add_subdirectory( workers )

add_subdirectory( _dependencies )

add_subdirectory( _executables )
