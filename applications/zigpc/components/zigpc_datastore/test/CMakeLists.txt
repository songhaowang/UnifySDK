target_add_unittest(
  zigpc_datastore
  NAME zigpc_datastore_test
  SOURCES zigpc_datastore_test.c
  DEPENDS uic_datastore_mock zigpc_config_mock)
