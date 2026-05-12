# ZigPC NCP library
add_library(zigpc_ncp src/zigpc_ncp.c)
target_link_libraries(zigpc_ncp PRIVATE zigpc_config)
target_include_directories(zigpc_ncp PUBLIC include)

if(BUILD_TESTING)
  target_add_mock(zigpc_ncp)
  add_subdirectory(test)
endif()
