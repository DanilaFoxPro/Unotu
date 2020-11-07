target_sources_local(
${APP_LIBRARY_NAME}
        PUBLIC
                data.h
                executable_enum.h
                tab_presets.h
        PRIVATE
                data.cpp
)

add_subdirectory( classes )
add_subdirectory( deps )
add_subdirectory( tab_presets )
