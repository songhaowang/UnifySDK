# ZigPC attribute management library
add_library(zigpc_attrmgmt src/zigpc_attrmgmt.c src/zigpc_attrmgmt_helper.c)
target_link_libraries(zigpc_attrmgmt PRIVATE unify unify_dotdot_attribute_store)
target_include_directories(zigpc_attrmgmt PUBLIC include)

if(BUILD_TESTING)
  target_add_mock(zigpc_attrmgmt)
  add_subdirectory(test)
endif()
