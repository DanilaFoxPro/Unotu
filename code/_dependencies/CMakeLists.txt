# cmake_policy( SET CMP0075 NEW ) # For 'plibsys'.

add_subdirectory( glfw-3.3.2 EXCLUDE_FROM_ALL )
add_subdirectory( glew-2.1.0/build/cmake EXCLUDE_FROM_ALL )
add_subdirectory( SQLiteCpp-3.0.0 EXCLUDE_FROM_ALL )

cmake_policy(SET CMP0079 NEW) # Allow 'target_link_libraries' here.

target_link_libraries( ${APP_LIBRARY_NAME} PRIVATE glfw )
target_link_libraries( ${APP_LIBRARY_NAME} PRIVATE glew_s )
target_link_libraries( ${APP_LIBRARY_NAME} PRIVATE SQLiteCpp )

# Why do I have to do that? Libraries should have been found!
target_include_directories(
        ${APP_LIBRARY_NAME}
        PRIVATE
                glew-2.1.0/include
)
