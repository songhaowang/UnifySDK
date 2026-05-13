/******************************************************************************
 * # License
 * <b>Copyright 2026 Silicon Laboratories Inc. www.silabs.com</b>
 ******************************************************************************
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 *****************************************************************************/

#include "zigpc_command_translator.h"
#include "zigpc_attrmgmt.h"
#include "zigpc_config.h"
#include "zigpc_ncp.h"
#include "zigpc_ncp_fixt.h"

#include "attribute_store.h"
#include "datastore.h"
#include "dotdot_mqtt_mock.h"
#include "unify_dotdot_attribute_store.h"
#include "unity.h"
#include "zigpc_config_mock.h"

#include <string.h>

static const char *last_unid             = NULL;
static uint8_t last_endpoint_id          = 0;
static bool last_on_off                  = false;
static uint8_t last_level                = 0;
static uint16_t last_transition_time     = 0;
static uint8_t last_options_mask         = 0;
static uint8_t last_options_override     = 0;
static unsigned send_on_off_call_count   = 0;
static unsigned move_to_level_call_count = 0;
static zigpc_config_t test_config;

static sl_status_t test_connect(const char *cpc_instance)
{
  TEST_ASSERT_NOT_NULL(cpc_instance);
  return SL_STATUS_OK;
}

static sl_status_t test_disconnect(void)
{
  return SL_STATUS_OK;
}

static sl_status_t test_send_on_off(const char *unid,
                                    uint8_t endpoint_id,
                                    bool on_off)
{
  last_unid        = unid;
  last_endpoint_id = endpoint_id;
  last_on_off      = on_off;
  send_on_off_call_count++;
  return SL_STATUS_OK;
}

static sl_status_t test_move_to_level(const char *unid,
                                      uint8_t endpoint_id,
                                      uint8_t level,
                                      uint16_t transition_time,
                                      uint8_t options_mask,
                                      uint8_t options_override)
{
  last_unid             = unid;
  last_endpoint_id      = endpoint_id;
  last_level            = level;
  last_transition_time  = transition_time;
  last_options_mask     = options_mask;
  last_options_override = options_override;
  move_to_level_call_count++;
  return SL_STATUS_OK;
}

void setUp(void)
{
  static const zigpc_ncp_interface_t interface = {
    .connect       = test_connect,
    .disconnect    = test_disconnect,
    .send_on_off   = test_send_on_off,
    .move_to_level = test_move_to_level,
  };

  memset(&test_config, 0, sizeof(test_config));
  last_unid               = NULL;
  last_endpoint_id        = 0;
  last_on_off             = false;
  last_level              = 0;
  last_transition_time    = 0;
  last_options_mask       = 0;
  last_options_override   = 0;
  send_on_off_call_count   = 0;
  move_to_level_call_count = 0;

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(&interface));
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    attribute_store_delete_all_children(attribute_store_get_root()));
}

void tearDown(void)
{
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(NULL));
  TEST_ASSERT_EQUAL(0, zigpc_command_translator_teardown());
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
  attribute_store_teardown();
  datastore_teardown();
  zigpc_config_mock_Destroy();
  return num_failures;
}

static void connect_test_backend(void)
{
  test_config.cpc_instance = "cpcd_0";
  zigpc_get_config_ExpectAndReturn(&test_config);
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_fixt_setup());
}

void test_zigpc_command_translator_init_registers_generated_callbacks(void)
{
  uic_mqtt_dotdot_on_off_generated_on_callback_set_Expect(
    zigpc_command_translator_on);
  uic_mqtt_dotdot_on_off_generated_off_callback_set_Expect(
    zigpc_command_translator_off);
  uic_mqtt_dotdot_level_generated_move_to_level_callback_set_Expect(
    zigpc_command_translator_move_to_level);

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_command_translator_init());

  uic_mqtt_dotdot_on_off_generated_on_callback_unset_Expect(
    zigpc_command_translator_on);
  uic_mqtt_dotdot_on_off_generated_off_callback_unset_Expect(
    zigpc_command_translator_off);
  uic_mqtt_dotdot_level_generated_move_to_level_callback_unset_Expect(
    zigpc_command_translator_move_to_level);
}

void test_zigpc_command_translator_teardown_unsets_generated_callbacks(void)
{
  uic_mqtt_dotdot_on_off_generated_on_callback_set_Expect(
    zigpc_command_translator_on);
  uic_mqtt_dotdot_on_off_generated_off_callback_set_Expect(
    zigpc_command_translator_off);
  uic_mqtt_dotdot_level_generated_move_to_level_callback_set_Expect(
    zigpc_command_translator_move_to_level);
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_command_translator_init());

  uic_mqtt_dotdot_on_off_generated_on_callback_unset_Expect(
    zigpc_command_translator_on);
  uic_mqtt_dotdot_on_off_generated_off_callback_unset_Expect(
    zigpc_command_translator_off);
  uic_mqtt_dotdot_level_generated_move_to_level_callback_unset_Expect(
    zigpc_command_translator_move_to_level);
  TEST_ASSERT_EQUAL(0, zigpc_command_translator_teardown());
}

void test_zigpc_command_translator_init_is_idempotent(void)
{
  uic_mqtt_dotdot_on_off_generated_on_callback_set_Expect(
    zigpc_command_translator_on);
  uic_mqtt_dotdot_on_off_generated_off_callback_set_Expect(
    zigpc_command_translator_off);
  uic_mqtt_dotdot_level_generated_move_to_level_callback_set_Expect(
    zigpc_command_translator_move_to_level);

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_command_translator_init());
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_command_translator_init());

  uic_mqtt_dotdot_on_off_generated_on_callback_unset_Expect(
    zigpc_command_translator_on);
  uic_mqtt_dotdot_on_off_generated_off_callback_unset_Expect(
    zigpc_command_translator_off);
  uic_mqtt_dotdot_level_generated_move_to_level_callback_unset_Expect(
    zigpc_command_translator_move_to_level);
}

void test_zigpc_command_translator_on_forwards_to_ncp_interface(void)
{
  connect_test_backend();
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint("zb-0001", 1, true, false));

  TEST_ASSERT_EQUAL(
    SL_STATUS_OK,
    zigpc_command_translator_on("zb-0001",
                                1,
                                UIC_MQTT_DOTDOT_CALLBACK_TYPE_NORMAL));

  TEST_ASSERT_EQUAL_UINT(1, send_on_off_call_count);
  TEST_ASSERT_EQUAL_STRING("zb-0001", last_unid);
  TEST_ASSERT_EQUAL_UINT8(1, last_endpoint_id);
  TEST_ASSERT_TRUE(last_on_off);
}

void test_zigpc_command_translator_move_to_level_forwards_to_ncp_interface(void)
{
  connect_test_backend();
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint("zb-0001", 2, false, true));

  TEST_ASSERT_EQUAL(
    SL_STATUS_OK,
    zigpc_command_translator_move_to_level(
      "zb-0001",
      2,
      UIC_MQTT_DOTDOT_CALLBACK_TYPE_NORMAL,
      42,
      15,
      0x12,
      0x34));

  TEST_ASSERT_EQUAL_UINT(1, move_to_level_call_count);
  TEST_ASSERT_EQUAL_STRING("zb-0001", last_unid);
  TEST_ASSERT_EQUAL_UINT8(2, last_endpoint_id);
  TEST_ASSERT_EQUAL_UINT8(42, last_level);
  TEST_ASSERT_EQUAL_UINT16(15, last_transition_time);
  TEST_ASSERT_EQUAL_UINT8(0x12, last_options_mask);
  TEST_ASSERT_EQUAL_UINT8(0x34, last_options_override);
}

void test_zigpc_command_translator_off_forwards_to_ncp_interface(void)
{
  connect_test_backend();
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint("zb-0001", 3, true, false));

  TEST_ASSERT_EQUAL(
    SL_STATUS_OK,
    zigpc_command_translator_off("zb-0001",
                                 3,
                                 UIC_MQTT_DOTDOT_CALLBACK_TYPE_NORMAL));

  TEST_ASSERT_EQUAL_UINT(1, send_on_off_call_count);
  TEST_ASSERT_EQUAL_STRING("zb-0001", last_unid);
  TEST_ASSERT_EQUAL_UINT8(3, last_endpoint_id);
  TEST_ASSERT_FALSE(last_on_off);
}

void test_zigpc_command_translator_on_rejects_unsupported_endpoint(void)
{
  connect_test_backend();

  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint("zb-level", 5, false, true));

  TEST_ASSERT_EQUAL(
    SL_STATUS_NOT_AVAILABLE,
    zigpc_command_translator_on("zb-level",
                                5,
                                UIC_MQTT_DOTDOT_CALLBACK_TYPE_NORMAL));
  TEST_ASSERT_EQUAL_UINT(0, send_on_off_call_count);
}

void test_zigpc_command_translator_move_to_level_rejects_unsupported_endpoint(void)
{
  connect_test_backend();

  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint("zb-onoff", 4, true, false));

  TEST_ASSERT_EQUAL(
    SL_STATUS_NOT_AVAILABLE,
    zigpc_command_translator_move_to_level("zb-onoff",
                                           4,
                                           UIC_MQTT_DOTDOT_CALLBACK_TYPE_NORMAL,
                                           42,
                                           15,
                                           0x12,
                                           0x34));
  TEST_ASSERT_EQUAL_UINT(0, move_to_level_call_count);
}

void test_zigpc_command_translator_on_support_check_uses_endpoint_capability(void)
{
  connect_test_backend();

  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint("zb-onoff", 4, true, false));
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint("zb-level", 5, false, true));
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_command_translator_on(
                      "zb-onoff",
                      4,
                      UIC_MQTT_DOTDOT_CALLBACK_TYPE_SUPPORT_CHECK));
  TEST_ASSERT_EQUAL(SL_STATUS_NOT_AVAILABLE,
                    zigpc_command_translator_on(
                      "zb-level",
                      5,
                      UIC_MQTT_DOTDOT_CALLBACK_TYPE_SUPPORT_CHECK));
  TEST_ASSERT_EQUAL(SL_STATUS_NOT_AVAILABLE,
                    zigpc_command_translator_on(
                      "zb-missing",
                      4,
                      UIC_MQTT_DOTDOT_CALLBACK_TYPE_SUPPORT_CHECK));
}

void test_zigpc_command_translator_move_to_level_support_check_uses_endpoint_capability(
  void)
{
  connect_test_backend();

  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint("zb-onoff", 4, true, false));
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint("zb-level", 5, false, true));
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_command_translator_move_to_level(
                      "zb-level",
                      5,
                      UIC_MQTT_DOTDOT_CALLBACK_TYPE_SUPPORT_CHECK,
                      0,
                      0,
                      0,
                      0));
  TEST_ASSERT_EQUAL(SL_STATUS_NOT_AVAILABLE,
                    zigpc_command_translator_move_to_level(
                      "zb-onoff",
                      4,
                      UIC_MQTT_DOTDOT_CALLBACK_TYPE_SUPPORT_CHECK,
                      0,
                      0,
                      0,
                      0));
  TEST_ASSERT_EQUAL(SL_STATUS_NOT_AVAILABLE,
                    zigpc_command_translator_move_to_level(
                      "zb-missing",
                      5,
                      UIC_MQTT_DOTDOT_CALLBACK_TYPE_SUPPORT_CHECK,
                      0,
                      0,
                      0,
                      0));
}

void test_zigpc_command_translator_requires_connected_backend(void)
{
  TEST_ASSERT_EQUAL(SL_STATUS_NOT_AVAILABLE,
                    zigpc_command_translator_on(
                      "zb-onoff",
                      1,
                      UIC_MQTT_DOTDOT_CALLBACK_TYPE_NORMAL));
  TEST_ASSERT_EQUAL(SL_STATUS_NOT_AVAILABLE,
                    zigpc_command_translator_on(
                      "zb-onoff",
                      1,
                      UIC_MQTT_DOTDOT_CALLBACK_TYPE_SUPPORT_CHECK));
  TEST_ASSERT_EQUAL(SL_STATUS_NOT_AVAILABLE,
                    zigpc_command_translator_move_to_level(
                      "zb-level",
                      1,
                      UIC_MQTT_DOTDOT_CALLBACK_TYPE_NORMAL,
                      10,
                      1,
                      0,
                      0));
  TEST_ASSERT_EQUAL(SL_STATUS_NOT_AVAILABLE,
                    zigpc_command_translator_move_to_level(
                      "zb-level",
                      1,
                      UIC_MQTT_DOTDOT_CALLBACK_TYPE_SUPPORT_CHECK,
                      10,
                      1,
                      0,
                      0));
}

void test_zigpc_command_translator_support_check_does_not_require_connected_backend(
  void)
{
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint("zb-onoff", 1, true, false));
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_attrmgmt_publish_endpoint("zb-level", 2, false, true));

  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_command_translator_on(
                      "zb-onoff",
                      1,
                      UIC_MQTT_DOTDOT_CALLBACK_TYPE_SUPPORT_CHECK));
  TEST_ASSERT_EQUAL(SL_STATUS_OK,
                    zigpc_command_translator_move_to_level(
                      "zb-level",
                      2,
                      UIC_MQTT_DOTDOT_CALLBACK_TYPE_SUPPORT_CHECK,
                      10,
                      1,
                      0,
                      0));
}
