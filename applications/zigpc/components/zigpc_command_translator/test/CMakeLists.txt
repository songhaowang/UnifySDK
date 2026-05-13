target_add_unittest(
  zigpc_command_translator
  NAME zigpc_command_translator_test
  SOURCES zigpc_command_translator_test.c
  DEPENDS uic_dotdot_mqtt_mock
  EXCLUDE uic_dotdot_mqtt)
