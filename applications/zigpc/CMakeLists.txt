add_subdirectory(components)

add_executable(zigpc main.c)
target_link_libraries(zigpc PUBLIC unify zigpc_config zigpc_datastore zigpc_ncp)

target_compile_definitions(
  zigpc PRIVATE CMAKE_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}"
                CMAKE_PROJECT_VERSION="${CMAKE_PROJECT_VERSION}")

install(TARGETS zigpc RUNTIME DESTINATION bin COMPONENT uic-zigpc)

add_component_to_uic(uic-zigpc "Universal IOT controller - ZigBee Protocol Controller"
                     "uic-zigpc" "libunify" "" "")
