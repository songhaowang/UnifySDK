# ZigPC discovery library
add_library(zigpc_discovery src/zigpc_discovery.c)
target_link_libraries(zigpc_discovery PRIVATE zigpc_attrmgmt zigpc_ncp)
target_include_directories(zigpc_discovery PUBLIC include)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
