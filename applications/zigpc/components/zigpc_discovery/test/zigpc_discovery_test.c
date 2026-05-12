// Includes from the component being tested
#include "zigpc_discovery.h"

// Includes from other ZigPC components
#include "zigpc_attrmgmt.h"
#include "zigpc_attrmgmt_helper.h"
#include "zigpc_ncp.h"
#include "zigpc_ncp_fixt.h"

// Test includes
#include "unity.h"

// Includes from other components
#include "attribute_store.h"
#include "attribute_store_fixt.h"
#include "attribute_store_helper.h"
#include "datastore.h"
#include "unify_dotdot_attribute_store.h"
#include "unify_dotdot_defined_attribute_types.h"
#include "zigpc_config.h"
#include "zigpc_config_mock.h"

#include <string.h>

static unsigned discover_network_call_count = 0;
static unsigned connect_call_count          = 0;
static unsigned disconnect_call_count       = 0;
static zigpc_config_t test_config;
static const char *connected_instance = NULL;

static sl_status_t test_discover_network_callback(void)
{
  discover_network_call_count++;

  return zigpc_ncp_notify_endpoint_interviewed("zb-0001", 2, false, true);
}

static sl_status_t test_discover_network_not_available_callback(void)
{
  discover_network_call_count++;

  return SL_STATUS_NOT_AVAILABLE;
}

static sl_status_t test_connect_callback(const char *cpc_instance)
{
  connected_instance = cpc_instance;
  connect_call_count++;

  return zigpc_ncp_notify_endpoint_interviewed("zb-0001", 1, true, false);
}

static sl_status_t test_disconnect_callback(void)
{
  disconnect_call_count++;
  return SL_STATUS_OK;
}

void suiteSetUp(void)
{
  zigpc_config_mock_Init();
  TEST_ASSERT_EQUAL(SL_STATUS_OK, datastore_init(":memory:"));
  TEST_ASSERT_EQUAL(SL_STATUS_OK, attribute_store_init());
  TEST_ASSERT_EQUAL(SL_STATUS_OK, unify_dotdot_attribute_store_init());
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_attrmgmt_init());
}

int suiteTearDown(int num_failures)
{
  zigpc_config_mock_Destroy();
  attribute_store_teardown();
  datastore_teardown();
  return num_failures;
}

void setUp(void)
{
  memset(&test_config, 0, sizeof(test_config));
  test_config.cpc_instance = "cpcd_0";
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    attribute_store_delete_all_children(attribute_store_get_root()));
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(NULL));
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_ncp_register_endpoint_interview_callback(NULL));
  discover_network_call_count = 0;
  connect_call_count = 0;
  disconnect_call_count = 0;
  connected_instance = NULL;
}

void tearDown(void) {}

void test_zigpc_discovery_run_full_interview_without_registered_discovery_is_not_available(
  void)
{
  TEST_ASSERT_EQUAL(SL_STATUS_NOT_AVAILABLE,
                    zigpc_discovery_run_full_interview());
}

void test_zigpc_discovery_init_without_registered_discovery_is_ok(void)
{
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_discovery_setup());
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_discovery_init());
}

void test_zigpc_discovery_init_with_registered_interface_without_discovery_is_invalid_state(
  void)
{
  zigpc_ncp_interface_t interface = {
    .connect = test_connect_callback,
    .disconnect = test_disconnect_callback,
  };

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(&interface));
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_discovery_setup());
  zigpc_get_config_ExpectAndReturn(&test_config);
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_fixt_setup());

  TEST_ASSERT_EQUAL(SL_STATUS_INVALID_STATE, zigpc_discovery_init());
  TEST_ASSERT_EQUAL(0, zigpc_ncp_fixt_teardown());
  TEST_ASSERT_EQUAL_UINT(1, disconnect_call_count);
}

void test_zigpc_discovery_init_with_registered_interface_and_not_available_backend_status_returns_backend_status(
  void)
{
  zigpc_ncp_interface_t interface = {
    .connect = test_connect_callback,
    .disconnect = test_disconnect_callback,
    .discover_network = test_discover_network_not_available_callback,
  };

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(&interface));
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_discovery_setup());
  zigpc_get_config_ExpectAndReturn(&test_config);
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_fixt_setup());

  TEST_ASSERT_EQUAL(SL_STATUS_NOT_AVAILABLE, zigpc_discovery_init());
  TEST_ASSERT_EQUAL_UINT(1, discover_network_call_count);
  TEST_ASSERT_EQUAL(0, zigpc_ncp_fixt_teardown());
  TEST_ASSERT_EQUAL_UINT(1, disconnect_call_count);
}

void test_zigpc_discovery_setup_before_connect_preserves_connect_and_full_interview_notifications(
  void)
{
  zigpc_ncp_interface_t interface = {
    .connect = test_connect_callback,
    .disconnect = test_disconnect_callback,
    .discover_network = test_discover_network_callback,
  };

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(&interface));
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_discovery_setup());
  zigpc_get_config_ExpectAndReturn(&test_config);
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_fixt_setup());

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_discovery_init());
  TEST_ASSERT_EQUAL_STRING(test_config.cpc_instance, connected_instance);
  TEST_ASSERT_EQUAL_UINT(1, connect_call_count);
  TEST_ASSERT_EQUAL_UINT(1, discover_network_call_count);

  attribute_store_node_t endpoint_node
    = zigpc_attrmgmt_get_endpoint_node("zb-0001", 1);

  TEST_ASSERT_NOT_EQUAL(ATTRIBUTE_STORE_INVALID_NODE, endpoint_node);
  TEST_ASSERT_NOT_EQUAL(
    ATTRIBUTE_STORE_INVALID_NODE,
    attribute_store_get_first_child_by_type(endpoint_node,
                                            DOTDOT_ATTRIBUTE_ID_ON_OFF_ON_OFF));
  TEST_ASSERT_EQUAL(ATTRIBUTE_STORE_INVALID_NODE,
                    attribute_store_get_first_child_by_type(
                      endpoint_node,
                      DOTDOT_ATTRIBUTE_ID_LEVEL_CURRENT_LEVEL));

  endpoint_node = zigpc_attrmgmt_get_endpoint_node("zb-0001", 2);

  TEST_ASSERT_NOT_EQUAL(ATTRIBUTE_STORE_INVALID_NODE, endpoint_node);
  TEST_ASSERT_EQUAL(ATTRIBUTE_STORE_INVALID_NODE,
                    attribute_store_get_first_child_by_type(
                      endpoint_node,
                      DOTDOT_ATTRIBUTE_ID_ON_OFF_ON_OFF));
  TEST_ASSERT_NOT_EQUAL(
    ATTRIBUTE_STORE_INVALID_NODE,
    attribute_store_get_first_child_by_type(endpoint_node,
                                            DOTDOT_ATTRIBUTE_ID_LEVEL_CURRENT_LEVEL));

  TEST_ASSERT_EQUAL(0, zigpc_ncp_fixt_teardown());
  TEST_ASSERT_EQUAL_UINT(1, disconnect_call_count);
}

void test_zigpc_discovery_teardown_unregisters_endpoint_callback(void)
{
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_discovery_setup());
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_ncp_notify_endpoint_interviewed("zb-0001",
                                                         3,
                                                         true,
                                                         false));

  TEST_ASSERT_EQUAL(0, zigpc_discovery_teardown());
  TEST_ASSERT_EQUAL(SL_STATUS_NOT_AVAILABLE,
                    zigpc_ncp_notify_endpoint_interviewed("zb-0001",
                                                         3,
                                                         true,
                                                         false));
}
