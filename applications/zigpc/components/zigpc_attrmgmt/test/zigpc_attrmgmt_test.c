// Includes from the component being tested
#include "zigpc_attrmgmt.h"
#include "zigpc_attrmgmt_attribute_types.h"
#include "zigpc_attrmgmt_helper.h"

// Test includes
#include "unity.h"

// Includes from other components
#include "attribute_store.h"
#include "attribute_store_fixt.h"
#include "attribute_store_helper.h"
#include "datastore.h"
#include "unify_dotdot_attribute_store.h"
#include "unify_dotdot_attribute_store_test.h"
#include "unify_dotdot_defined_attribute_types.h"

#include <string.h>

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
}

void tearDown(void) {}

void test_zigpc_attrmgmt_helpers_create_and_resolve_endpoint_node(void)
{
  const char *unid               = "zb-0001";
  dotdot_endpoint_id_t endpoint_id = 2;
  char resolved_unid[MAXIMUM_UNID_SIZE] = {};
  dotdot_endpoint_id_t resolved_endpoint = 0;

  attribute_store_node_t unid_node = zigpc_attrmgmt_create_unid_node(unid);
  attribute_store_node_t endpoint_node
    = zigpc_attrmgmt_create_endpoint_node(unid, endpoint_id);

  TEST_ASSERT_NOT_EQUAL(ATTRIBUTE_STORE_INVALID_NODE, unid_node);
  TEST_ASSERT_NOT_EQUAL(ATTRIBUTE_STORE_INVALID_NODE, endpoint_node);
  TEST_ASSERT_EQUAL(unid_node, zigpc_attrmgmt_get_unid_node(unid));
  TEST_ASSERT_EQUAL(endpoint_node,
                    zigpc_attrmgmt_get_endpoint_node(unid, endpoint_id));
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_get_unid_from_node(endpoint_node,
                                                      resolved_unid));
  TEST_ASSERT_EQUAL_STRING(unid, resolved_unid);
  memset(resolved_unid, 0, sizeof(resolved_unid));
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_get_endpoint_from_node(endpoint_node,
                                                          resolved_unid,
                                                          &resolved_endpoint));
  TEST_ASSERT_EQUAL_STRING(unid, resolved_unid);
  TEST_ASSERT_EQUAL(endpoint_id, resolved_endpoint);
}

void test_zigpc_attrmgmt_publish_endpoint_creates_supported_cluster_attributes(
  void)
{
  const char *unid               = "zb-1234";
  dotdot_endpoint_id_t endpoint_id = 1;

  TEST_ASSERT_FALSE(dotdot_is_supported_on_off_on_off(unid, endpoint_id));
  TEST_ASSERT_FALSE(dotdot_is_supported_level_current_level(unid, endpoint_id));

  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint(unid,
                                                    endpoint_id,
                                                    true,
                                                    true));

  attribute_store_node_t endpoint_node
    = zigpc_attrmgmt_get_endpoint_node(unid, endpoint_id);

  TEST_ASSERT_NOT_EQUAL(ATTRIBUTE_STORE_INVALID_NODE, endpoint_node);
  TEST_ASSERT_NOT_EQUAL(
    ATTRIBUTE_STORE_INVALID_NODE,
    attribute_store_get_first_child_by_type(endpoint_node,
                                            DOTDOT_ATTRIBUTE_ID_ON_OFF_ON_OFF));
  TEST_ASSERT_NOT_EQUAL(
    ATTRIBUTE_STORE_INVALID_NODE,
    attribute_store_get_first_child_by_type(
      endpoint_node,
      DOTDOT_ATTRIBUTE_ID_LEVEL_CURRENT_LEVEL));

  TEST_ASSERT_TRUE(dotdot_is_supported_on_off_on_off(unid, endpoint_id));
  TEST_ASSERT_TRUE(dotdot_is_supported_level_current_level(unid, endpoint_id));
}

void test_zigpc_attrmgmt_publish_endpoint_removes_stale_on_off_support(void)
{
  const char *unid               = "zb-5678";
  dotdot_endpoint_id_t endpoint_id = 2;

  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint(unid,
                                                    endpoint_id,
                                                    true,
                                                    true));
  TEST_ASSERT_TRUE(dotdot_is_supported_on_off_on_off(unid, endpoint_id));
  TEST_ASSERT_TRUE(dotdot_is_supported_level_current_level(unid, endpoint_id));

  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint(unid,
                                                    endpoint_id,
                                                    false,
                                                    true));

  attribute_store_node_t endpoint_node
    = zigpc_attrmgmt_get_endpoint_node(unid, endpoint_id);

  TEST_ASSERT_EQUAL(ATTRIBUTE_STORE_INVALID_NODE,
                    attribute_store_get_first_child_by_type(
                      endpoint_node,
                      DOTDOT_ATTRIBUTE_ID_ON_OFF_ON_OFF));
  TEST_ASSERT_NOT_EQUAL(ATTRIBUTE_STORE_INVALID_NODE,
                        attribute_store_get_first_child_by_type(
                          endpoint_node,
                          DOTDOT_ATTRIBUTE_ID_LEVEL_CURRENT_LEVEL));
  TEST_ASSERT_FALSE(dotdot_is_supported_on_off_on_off(unid, endpoint_id));
  TEST_ASSERT_TRUE(dotdot_is_supported_level_current_level(unid, endpoint_id));
}

void test_zigpc_attrmgmt_publish_endpoint_removes_stale_level_support(void)
{
  const char *unid               = "zb-9abc";
  dotdot_endpoint_id_t endpoint_id = 3;

  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint(unid,
                                                    endpoint_id,
                                                    true,
                                                    true));
  TEST_ASSERT_TRUE(dotdot_is_supported_on_off_on_off(unid, endpoint_id));
  TEST_ASSERT_TRUE(dotdot_is_supported_level_current_level(unid, endpoint_id));

  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint(unid,
                                                    endpoint_id,
                                                    true,
                                                    false));

  attribute_store_node_t endpoint_node
    = zigpc_attrmgmt_get_endpoint_node(unid, endpoint_id);

  TEST_ASSERT_NOT_EQUAL(ATTRIBUTE_STORE_INVALID_NODE,
                        attribute_store_get_first_child_by_type(
                          endpoint_node,
                          DOTDOT_ATTRIBUTE_ID_ON_OFF_ON_OFF));
  TEST_ASSERT_EQUAL(ATTRIBUTE_STORE_INVALID_NODE,
                    attribute_store_get_first_child_by_type(
                      endpoint_node,
                      DOTDOT_ATTRIBUTE_ID_LEVEL_CURRENT_LEVEL));
  TEST_ASSERT_TRUE(dotdot_is_supported_on_off_on_off(unid, endpoint_id));
  TEST_ASSERT_FALSE(dotdot_is_supported_level_current_level(unid, endpoint_id));
}

void test_zigpc_attrmgmt_publish_endpoint_removes_both_stale_supports(void)
{
  const char *unid               = "zb-def0";
  dotdot_endpoint_id_t endpoint_id = 4;

  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint(unid,
                                                    endpoint_id,
                                                    true,
                                                    true));
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint(unid,
                                                    endpoint_id,
                                                    false,
                                                    false));

  attribute_store_node_t endpoint_node
    = zigpc_attrmgmt_get_endpoint_node(unid, endpoint_id);

  TEST_ASSERT_EQUAL(ATTRIBUTE_STORE_INVALID_NODE,
                    attribute_store_get_first_child_by_type(
                      endpoint_node,
                      DOTDOT_ATTRIBUTE_ID_ON_OFF_ON_OFF));
  TEST_ASSERT_EQUAL(ATTRIBUTE_STORE_INVALID_NODE,
                    attribute_store_get_first_child_by_type(
                      endpoint_node,
                      DOTDOT_ATTRIBUTE_ID_LEVEL_CURRENT_LEVEL));
  TEST_ASSERT_FALSE(dotdot_is_supported_on_off_on_off(unid, endpoint_id));
  TEST_ASSERT_FALSE(dotdot_is_supported_level_current_level(unid, endpoint_id));
}

void test_zigpc_attrmgmt_publish_endpoint_without_supported_clusters_is_ok(void)
{
  const char *unid               = "zb-1111";
  dotdot_endpoint_id_t endpoint_id = 5;

  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint(unid,
                                                    endpoint_id,
                                                    false,
                                                    false));

  attribute_store_node_t endpoint_node
    = zigpc_attrmgmt_get_endpoint_node(unid, endpoint_id);

  TEST_ASSERT_NOT_EQUAL(ATTRIBUTE_STORE_INVALID_NODE, endpoint_node);
  TEST_ASSERT_EQUAL(ATTRIBUTE_STORE_INVALID_NODE,
                    attribute_store_get_first_child_by_type(
                      endpoint_node,
                      DOTDOT_ATTRIBUTE_ID_ON_OFF_ON_OFF));
  TEST_ASSERT_EQUAL(ATTRIBUTE_STORE_INVALID_NODE,
                    attribute_store_get_first_child_by_type(
                      endpoint_node,
                      DOTDOT_ATTRIBUTE_ID_LEVEL_CURRENT_LEVEL));
  TEST_ASSERT_FALSE(dotdot_is_supported_on_off_on_off(unid, endpoint_id));
  TEST_ASSERT_FALSE(dotdot_is_supported_level_current_level(unid, endpoint_id));
}

void test_zigpc_attrmgmt_publish_endpoint_with_null_unid_fails(void)
{
  TEST_ASSERT_EQUAL(SL_STATUS_FAIL,
                    zigpc_attrmgmt_publish_endpoint(NULL, 1, true, false));
}
