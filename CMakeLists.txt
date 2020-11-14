cmake_minimum_required (VERSION 3.5)


# ███████ ██    ██ ███    ██  ██████ ████████ ██  ██████  ███    ██ ███████
# ██      ██    ██ ████   ██ ██         ██    ██ ██    ██ ████   ██ ██
# █████   ██    ██ ██ ██  ██ ██         ██    ██ ██    ██ ██ ██  ██ ███████
# ██      ██    ██ ██  ██ ██ ██         ██    ██ ██    ██ ██  ██ ██      ██
# ██       ██████  ██   ████  ██████    ██    ██  ██████  ██   ████ ███████

# NOTE: This helper function assumes no generator expressions are used
#       for the source files
function(target_sources_local target)
  if(POLICY CMP0076)
    # New behavior is available, so just forward to it by ensuring
    # that we have the policy set to request the new behavior, but
    # don't change the policy setting for the calling scope
    cmake_policy(PUSH)
    cmake_policy(SET CMP0076 NEW)
    target_sources(${target} ${ARGN})
    cmake_policy(POP)
    return()
  endif()

  # Must be using CMake 3.12 or earlier, so simulate the new behavior
  unset(_srcList)
  get_target_property(_targetSourceDir ${target} SOURCE_DIR)

  foreach(src ${ARGN})
    if(NOT src STREQUAL "PRIVATE" AND
       NOT src STREQUAL "PUBLIC" AND
       NOT src STREQUAL "INTERFACE" AND
       NOT IS_ABSOLUTE "${src}")
      # Relative path to source, prepend relative to where target was defined
      file(RELATIVE_PATH src "${_targetSourceDir}" "${CMAKE_CURRENT_LIST_DIR}/${src}")
    endif()
    list(APPEND _srcList ${src})
  endforeach()
  target_sources(${target} ${_srcList})
endfunction()

# ████████ ██   ██ ███████     ██████  ██████   ██████       ██ ███████  ██████ ████████
#    ██    ██   ██ ██          ██   ██ ██   ██ ██    ██      ██ ██      ██         ██
#    ██    ███████ █████       ██████  ██████  ██    ██      ██ █████   ██         ██
#    ██    ██   ██ ██          ██      ██   ██ ██    ██ ██   ██ ██      ██         ██
#    ██    ██   ██ ███████     ██      ██   ██  ██████   █████  ███████  ██████    ██

project ( Unotu CXX ASM )
set( APP_NAME ${PROJECT_NAME} )
set( APP_LIBRARY_NAME ${APP_NAME}Library )

add_library( ${APP_LIBRARY_NAME} STATIC )
set_property(TARGET ${APP_LIBRARY_NAME} PROPERTY CXX_STANDARD 11)
target_compile_options(
        ${APP_LIBRARY_NAME}
        PRIVATE
                "-Wall"
)

target_include_directories(
  ${APP_LIBRARY_NAME}
  PUBLIC
    code
)

add_subdirectory(code)

### Executables

add_executable( ${APP_NAME} WIN32 "code/unotu/_executables/main.cpp" )
set_property(TARGET ${APP_NAME} PROPERTY CXX_STANDARD 11)
target_compile_options(
  ${APP_NAME}
  PRIVATE
    "-Wall"
)
target_link_libraries( ${APP_NAME} PRIVATE ${APP_LIBRARY_NAME} )
target_link_options(
        ${APP_NAME}
        PRIVATE
                "-static"
                "-lpthread"
                "-static-libgcc"
                "-static-libstdc++"
)

add_executable( ${APP_NAME}Test "code/unotu/_executables/test.cpp" )
set_property(TARGET ${APP_NAME}Test PROPERTY CXX_STANDARD 11)
target_compile_options(
  ${APP_NAME}
  PRIVATE
    "-Wall"
)
target_link_libraries( ${APP_NAME}Test PRIVATE ${APP_LIBRARY_NAME} )

add_executable( ${APP_NAME}Chat "code/unotu/_executables/chat.cpp" )
set_property(TARGET ${APP_NAME}Chat PROPERTY CXX_STANDARD 11)
target_compile_options(
  ${APP_NAME}Chat
  PRIVATE
    "-Wall"
)
target_link_libraries( ${APP_NAME}Chat PRIVATE ${APP_LIBRARY_NAME} )

### Data

# Data should be included after executables, because it may pack data into them. (Windows .rc file.)
add_subdirectory(data)
