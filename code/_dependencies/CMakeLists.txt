
add_subdirectory( UnotUI EXCLUDE_FROM_ALL )
add_subdirectory( SQLiteCpp-3.0.0 EXCLUDE_FROM_ALL )

cmake_policy(SET CMP0079 NEW) # Allow 'target_link_libraries' here.

target_link_libraries( ${APP_LIBRARY_NAME} PUBLIC  UnotUI )
target_link_libraries( ${APP_LIBRARY_NAME} PRIVATE SQLiteCpp )
