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
#include "zigpc_ncp.h"

#include "dotdot_mqtt_mock.h"
#include "unity.h"

static const char *last_unid             = NULL;
static uint8_t last_endpoint_id          = 0;
static bool last_on_off                  = false;
static uint8_t last_level                = 0;
static unsigned send_on_off_call_count   = 0;
static unsigned move_to_level_call_count = 0;

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
                                      uint8_t level)
{
  last_unid        = unid;
  last_endpoint_id = endpoint_id;
  last_level       = level;
  move_to_level_call_count++;
  return SL_STATUS_OK;
}

void setUp(void)
{
  static const zigpc_ncp_interface_t interface = {
    .send_on_off   = test_send_on_off,
    .move_to_level = test_move_to_level,
  };

  last_unid               = NULL;
  last_endpoint_id        = 0;
  last_on_off             = false;
  last_level              = 0;
  send_on_off_call_count   = 0;
  move_to_level_call_count = 0;

  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(&interface));
}

void tearDown(void)
{
  TEST_ASSERT_EQUAL(SL_STATUS_OK, zigpc_ncp_set_interface(NULL));
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
}

void test_zigpc_command_translator_on_forwards_to_ncp_interface(void)
{
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
  TEST_ASSERT_EQUAL(
    SL_STATUS_OK,
    zigpc_command_translator_move_to_level(
      "zb-0001",
      2,
      UIC_MQTT_DOTDOT_CALLBACK_TYPE_NORMAL,
      42,
      0,
      0,
      0));

  TEST_ASSERT_EQUAL_UINT(1, move_to_level_call_count);
  TEST_ASSERT_EQUAL_STRING("zb-0001", last_unid);
  TEST_ASSERT_EQUAL_UINT8(2, last_endpoint_id);
  TEST_ASSERT_EQUAL_UINT8(42, last_level);
}

void test_zigpc_command_translator_off_forwards_to_ncp_interface(void)
{
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
