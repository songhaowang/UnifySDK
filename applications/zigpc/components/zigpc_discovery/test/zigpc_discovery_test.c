// Includes from the component being tested
#include "zigpc_discovery.h"

// Includes from other ZigPC components
#include "zigpc_attrmgmt.h"
#include "zigpc_attrmgmt_helper.h"
#include "zigpc_ncp.h"

// Test includes
#include "unity.h"

// Includes from other components
#include "attribute_store.h"
#include "attribute_store_helper.h"
#include "datastore.h"
#include "unify_dotdot_attribute_store.h"
#include "unify_dotdot_defined_attribute_types.h"

static unsigned discover_network_call_count = 0;

static sl_status_t test_discover_network_callback(void)
{
  discover_network_call_count++;

  return zigpc_ncp_notify_endpoint_interviewed("zb-0001", 1, true, false);
}

void suiteSetUp(void)
{
  TEST_ASSERT_EQUAL(SL_STATUS_OK, datastore_init(":memory:"));
  TEST_ASSERT_EQUAL(SL_STATUS_OK, attribute_store_init());
  TEST_ASSERT_EQUAL(SL_STATUS_OK, unify_dotdot_attribute_store_init());
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_attrmgmt_init());
}

int suiteTearDown(int num_failures)
{
  attribute_store_teardown();
  datastore_teardown();
  return num_failures;
}

void setUp(void)
{
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    attribute_store_delete_all_children(attribute_store_get_root()));
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(NULL));
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_ncp_register_endpoint_interview_callback(NULL));
  discover_network_call_count = 0;
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
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_discovery_init());
}

void test_zigpc_discovery_init_runs_ncp_interview_callback_and_populates_attribute_store(
  void)
{
  zigpc_ncp_interface_t interface = {
    .discover_network = test_discover_network_callback,
  };

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(&interface));

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_discovery_init());
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
}
