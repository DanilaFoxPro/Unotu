target_sources_local(
  ${APP_LIBRARY_NAME}
  PRIVATE
    packed_data.s
)

target_include_directories(
  ${APP_LIBRARY_NAME}
  PRIVATE
    .
    fonts
    shader_source
)

if( WIN32 ) # True if Windows (32 and 64 bit). Magic.
  target_sources_local(
    ${APP_NAME}
    PRIVATE
      Unotu.rc
  )
  target_sources_local(
    ${APP_NAME}Test
    PRIVATE
      Unotu.rc
  )
  target_sources_local(
    ${APP_NAME}Chat
    PRIVATE
      Unotu.rc
  )
endif()
