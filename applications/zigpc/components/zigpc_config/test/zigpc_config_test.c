// Includes from the component being tested
#include "zigpc_config.h"
#include "zigpc_config_fixt.h"

// Test includes
#include "unity.h"

// CMocks
#include "config_mock.h"

static const char *test_datastore_file = "zigpc-test.db";
static const char *test_cpc_instance   = "cpcd_0";
static const char *test_mqtt_host      = "localhost";
static const int test_mqtt_port        = 2000;

void suiteSetUp()
{
  config_mock_Init();
}

int suiteTearDown(int num_failures)
{
  config_mock_Destroy();
  return num_failures;
}

void setUp() {}

void tearDown() {}

void test_zigpc_config_init_registers_string_cpc_instance()
{
  config_add_string_ExpectAndReturn(CONFIG_KEY_ZIGPC_DATASTORE_FILE,
                                    "ZigPC datastore database file",
                                    DEFAULT_ZIGPC_DATASTORE_FILE,
                                    CONFIG_STATUS_OK);
  config_add_string_ExpectAndReturn(CONFIG_KEY_ZIGPC_CPC_INSTANCE,
                                    "ZigPC CPC instance",
                                    DEFAULT_ZIGPC_CPC_INSTANCE,
                                    CONFIG_STATUS_OK);

  TEST_ASSERT_EQUAL(0, zigpc_config_init());
}

void test_zigpc_config_fixture_populates_string_cpc_instance()
{
  config_get_as_string_ExpectAndReturn(CONFIG_KEY_ZIGPC_DATASTORE_FILE,
                                       0,
                                       CONFIG_STATUS_OK);
  config_get_as_string_IgnoreArg_result();
  config_get_as_string_ReturnThruPtr_result(&test_datastore_file);

  config_get_as_string_ExpectAndReturn(CONFIG_KEY_ZIGPC_CPC_INSTANCE,
                                       0,
                                       CONFIG_STATUS_OK);
  config_get_as_string_IgnoreArg_result();
  config_get_as_string_ReturnThruPtr_result(&test_cpc_instance);

  config_get_as_string_ExpectAndReturn(CONFIG_KEY_MQTT_HOST,
                                       0,
                                       CONFIG_STATUS_OK);
  config_get_as_string_IgnoreArg_result();
  config_get_as_string_ReturnThruPtr_result(&test_mqtt_host);

  config_get_as_int_ExpectAndReturn(CONFIG_KEY_MQTT_PORT, 0, CONFIG_STATUS_OK);
  config_get_as_int_IgnoreArg_result();
  config_get_as_int_ReturnThruPtr_result((int *)&test_mqtt_port);

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_config_fixt_setup());
  TEST_ASSERT_EQUAL_STRING(test_datastore_file, zigpc_get_config()->datastore_file);
  TEST_ASSERT_EQUAL_STRING(test_cpc_instance, zigpc_get_config()->cpc_instance);
  TEST_ASSERT_EQUAL_STRING(test_mqtt_host, zigpc_get_config()->mqtt_host);
  TEST_ASSERT_EQUAL(test_mqtt_port, zigpc_get_config()->mqtt_port);
}

void test_zigpc_config_fixture_fails_when_mqtt_port_read_fails()
{
  config_get_as_string_ExpectAndReturn(CONFIG_KEY_ZIGPC_DATASTORE_FILE,
                                       0,
                                       CONFIG_STATUS_OK);
  config_get_as_string_IgnoreArg_result();
  config_get_as_string_ReturnThruPtr_result(&test_datastore_file);

  config_get_as_string_ExpectAndReturn(CONFIG_KEY_ZIGPC_CPC_INSTANCE,
                                       0,
                                       CONFIG_STATUS_OK);
  config_get_as_string_IgnoreArg_result();
  config_get_as_string_ReturnThruPtr_result(&test_cpc_instance);

  config_get_as_string_ExpectAndReturn(CONFIG_KEY_MQTT_HOST,
                                       0,
                                       CONFIG_STATUS_OK);
  config_get_as_string_IgnoreArg_result();
  config_get_as_string_ReturnThruPtr_result(&test_mqtt_host);

  config_get_as_int_ExpectAndReturn(CONFIG_KEY_MQTT_PORT,
                                    0,
                                    CONFIG_STATUS_INVALID_TYPE);
  config_get_as_int_IgnoreArg_result();

  TEST_ASSERT_EQUAL(SL_STATUS_FAIL, zigpc_config_fixt_setup());
}
