add_executable( ${APP_NAME} WIN32 main.cpp )
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

add_executable( ${APP_NAME}Test test.cpp )
set_property(TARGET ${APP_NAME}Test PROPERTY CXX_STANDARD 11)
target_compile_options(
  ${APP_NAME}
  PRIVATE
    "-Wall"
)
target_link_libraries( ${APP_NAME}Test PRIVATE ${APP_LIBRARY_NAME} )

add_executable( ${APP_NAME}Chat chat.cpp )
set_property(TARGET ${APP_NAME}Chat PROPERTY CXX_STANDARD 11)
target_compile_options(
  ${APP_NAME}Chat
  PRIVATE
    "-Wall"
)
target_link_libraries( ${APP_NAME}Chat PRIVATE ${APP_LIBRARY_NAME} )
