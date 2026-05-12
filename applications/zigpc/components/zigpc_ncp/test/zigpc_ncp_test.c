// Includes from the component being tested
#include "zigpc_ncp.h"
#include "zigpc_ncp_fixt.h"
#include "zigpc_config.h"

// Test includes
#include "unity.h"

// CMocks
#include "zigpc_config_mock.h"

#include <string.h>

static zigpc_config_t test_config;
static const char *connected_instance = NULL;
static const char *replacement_connected_instance = NULL;
static unsigned disconnect_call_count = 0;
static unsigned alt_disconnect_call_count = 0;
static unsigned replacement_connect_call_count = 0;

static sl_status_t test_connect(const char *cpc_instance)
{
  connected_instance = cpc_instance;
  return SL_STATUS_OK;
}

static sl_status_t test_connect_alt(const char *cpc_instance)
{
  replacement_connected_instance = cpc_instance;
  replacement_connect_call_count++;
  return SL_STATUS_OK;
}

static sl_status_t test_disconnect(void)
{
  disconnect_call_count++;
  return SL_STATUS_OK;
}

static sl_status_t test_disconnect_alt(void)
{
  alt_disconnect_call_count++;
  return SL_STATUS_OK;
}

static sl_status_t test_disconnect_fail(void)
{
  disconnect_call_count++;
  return SL_STATUS_FAIL;
}

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
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(NULL));
  connected_instance = NULL;
  replacement_connected_instance = NULL;
  disconnect_call_count = 0;
  alt_disconnect_call_count = 0;
  replacement_connect_call_count = 0;
}

void tearDown(void) {}

void test_zigpc_ncp_fixture_allows_startup_without_registered_interface(void)
{
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_fixt_setup());
  TEST_ASSERT_EQUAL(0, zigpc_ncp_fixt_teardown());
}

void test_zigpc_ncp_fixture_uses_configured_cpc_instance_on_connect(void)
{
  zigpc_ncp_interface_t interface = {
    .connect = test_connect,
    .disconnect = test_disconnect,
  };

  test_config.cpc_instance = "cpcd_0";

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(&interface));
  zigpc_get_config_ExpectAndReturn(&test_config);

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_fixt_setup());
  TEST_ASSERT_EQUAL_STRING(test_config.cpc_instance, connected_instance);
}

void test_zigpc_ncp_fixture_fails_with_incomplete_registered_lifecycle(void)
{
  zigpc_ncp_interface_t interface = {
    .connect = test_connect,
  };

  test_config.cpc_instance = "cpcd_0";

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(&interface));

  TEST_ASSERT_EQUAL(SL_STATUS_INVALID_STATE, zigpc_ncp_fixt_setup());
  TEST_ASSERT_NULL(connected_instance);
}

void test_zigpc_ncp_fixture_disconnects_on_teardown_after_connect(void)
{
  zigpc_ncp_interface_t interface = {
    .connect = test_connect,
    .disconnect = test_disconnect,
  };

  test_config.cpc_instance = "cpcd_0";

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(&interface));
  zigpc_get_config_ExpectAndReturn(&test_config);

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_fixt_setup());
  TEST_ASSERT_EQUAL(0, zigpc_ncp_fixt_teardown());
  TEST_ASSERT_EQUAL_UINT(1, disconnect_call_count);
}

void test_zigpc_ncp_fixture_disconnects_previous_backend_when_clearing_interface(void)
{
  zigpc_ncp_interface_t interface = {
    .connect = test_connect,
    .disconnect = test_disconnect,
  };

  test_config.cpc_instance = "cpcd_0";

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(&interface));
  zigpc_get_config_ExpectAndReturn(&test_config);

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_fixt_setup());

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(NULL));

  TEST_ASSERT_EQUAL_UINT(1, disconnect_call_count);
  TEST_ASSERT_EQUAL(0, zigpc_ncp_fixt_teardown());
  TEST_ASSERT_EQUAL_UINT(1, disconnect_call_count);
}

void test_zigpc_ncp_fixture_disconnects_previous_backend_when_replacing_interface(void)
{
  zigpc_ncp_interface_t interface = {
    .connect = test_connect,
    .disconnect = test_disconnect,
  };
  zigpc_ncp_interface_t replacement = {
    .connect = test_connect_alt,
    .disconnect = test_disconnect_alt,
  };

  test_config.cpc_instance = "cpcd_0";

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(&interface));
  zigpc_get_config_ExpectAndReturn(&test_config);

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_fixt_setup());

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(&replacement));

  TEST_ASSERT_EQUAL_UINT(1, disconnect_call_count);
  zigpc_get_config_ExpectAndReturn(&test_config);
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_fixt_setup());
  TEST_ASSERT_EQUAL_STRING(test_config.cpc_instance, replacement_connected_instance);
  TEST_ASSERT_EQUAL_UINT(1, replacement_connect_call_count);
  TEST_ASSERT_EQUAL_UINT(0, alt_disconnect_call_count);
}

void test_zigpc_ncp_fixture_keeps_original_backend_when_replacement_disconnect_fails(void)
{
  zigpc_ncp_interface_t interface = {
    .connect = test_connect,
    .disconnect = test_disconnect,
  };
  zigpc_ncp_interface_t replacement = {
    .connect = test_connect_alt,
    .disconnect = test_disconnect_fail,
  };

  test_config.cpc_instance = "cpcd_0";

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(&interface));
  zigpc_get_config_ExpectAndReturn(&test_config);

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_fixt_setup());

  TEST_ASSERT_EQUAL(SL_STATUS_FAIL, zigpc_ncp_set_interface(&replacement));

  TEST_ASSERT_NOT_NULL(zigpc_ncp_get_interface());
  TEST_ASSERT_EQUAL_PTR(interface.connect, zigpc_ncp_get_interface()->connect);
  TEST_ASSERT_EQUAL_PTR(interface.disconnect, zigpc_ncp_get_interface()->disconnect);
  TEST_ASSERT_EQUAL_UINT(1, disconnect_call_count);
  TEST_ASSERT_EQUAL_UINT(0, replacement_connect_call_count);
  TEST_ASSERT_EQUAL_UINT(0, alt_disconnect_call_count);
}
