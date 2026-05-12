// Includes from the component being tested
#include "zigpc_datastore_fixt.h"
#include "zigpc_config.h"

// Test includes
#include "unity.h"

// Generic includes
#include <string.h>

// CMocks
#include "datastore_fixt_mock.h"
#include "zigpc_config_mock.h"

static zigpc_config_t test_config;

void suiteSetUp(void)
{
  zigpc_config_mock_Init();
}

int suiteTearDown(int num_failures)
{
  zigpc_config_mock_Destroy();
  return num_failures;
}

void setUp(void)
{
  memset(&test_config, 0, sizeof(test_config));
}

void tearDown(void) {}

void test_zigpc_datastore_fixture_uses_configured_file_and_version(void)
{
  test_config.datastore_file = "zigpc-test.db";

  zigpc_get_config_ExpectAndReturn(&test_config);
  datastore_fixt_setup_and_handle_version_ExpectAndReturn(test_config.datastore_file,
                                                          ZIGPC_DATASTORE_VERSION,
                                                          SL_STATUS_OK);

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_datastore_fixt_setup());
}
