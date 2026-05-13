# ZigPC command translator library
add_library(zigpc_command_translator src/zigpc_command_translator.c)
target_link_libraries(zigpc_command_translator
                      PRIVATE zigpc_ncp unify_dotdot_attribute_store
                              uic_dotdot_mqtt)
target_include_directories(zigpc_command_translator PUBLIC include)

if(BUILD_TESTING)
  target_add_mock(zigpc_command_translator)
  add_subdirectory(test)
endif()
