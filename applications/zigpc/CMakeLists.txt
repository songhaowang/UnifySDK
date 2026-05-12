add_subdirectory(components)

add_executable(zigpc main.c)
target_link_libraries(zigpc PUBLIC unify)

target_compile_definitions(
  zigpc PRIVATE CMAKE_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}"
                CMAKE_PROJECT_VERSION="${CMAKE_PROJECT_VERSION}")

install(TARGETS zigpc RUNTIME DESTINATION bin)
