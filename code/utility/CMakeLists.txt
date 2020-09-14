target_sources_local(
${APP_LIBRARY_NAME}
        PUBLIC
                clipboard.h
                colors.h
                data.h
                executable_enum.h
                fpoint.h
                general.h
                packed_data.h
                point.h
                shortcuts.h
                tab_presets.h
                text.h
                unit.h
                widget.h
                window.h
        PRIVATE
                clipboard.cpp
                data.cpp
                fpoint.cpp
                general.cpp
                point.cpp
                shortcuts.cpp
                text.cpp
                unit.cpp
                widget.cpp
                window.cpp
)

add_subdirectory( classes )
add_subdirectory( deps )
add_subdirectory( tab_presets )
