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

static sl_status_t zigpc_command_translator_command_supported(bool supported)
{
  return supported ? SL_STATUS_OK : SL_STATUS_NOT_AVAILABLE;
}

sl_status_t zigpc_command_translator_init(void)
{
  uic_mqtt_dotdot_on_off_generated_on_callback_set(zigpc_command_translator_on);
  uic_mqtt_dotdot_on_off_generated_off_callback_set(
    zigpc_command_translator_off);
  uic_mqtt_dotdot_level_generated_move_to_level_callback_set(
    zigpc_command_translator_move_to_level);

  return SL_STATUS_OK;
}

sl_status_t zigpc_command_translator_on(
  dotdot_unid_t unid,
  dotdot_endpoint_id_t endpoint_id,
  uic_mqtt_dotdot_callback_call_type_t call_type)
{
  const zigpc_ncp_interface_t *interface = zigpc_ncp_get_interface();

  if (call_type == UIC_MQTT_DOTDOT_CALLBACK_TYPE_SUPPORT_CHECK) {
    return zigpc_command_translator_command_supported(
      (interface != NULL) && (interface->send_on_off != NULL));
  }

  if ((interface == NULL) || (interface->send_on_off == NULL)) {
    return SL_STATUS_NOT_AVAILABLE;
  }

  return interface->send_on_off(unid, endpoint_id, true);
}

sl_status_t zigpc_command_translator_off(
  dotdot_unid_t unid,
  dotdot_endpoint_id_t endpoint_id,
  uic_mqtt_dotdot_callback_call_type_t call_type)
{
  const zigpc_ncp_interface_t *interface = zigpc_ncp_get_interface();

  if (call_type == UIC_MQTT_DOTDOT_CALLBACK_TYPE_SUPPORT_CHECK) {
    return zigpc_command_translator_command_supported(
      (interface != NULL) && (interface->send_on_off != NULL));
  }

  if ((interface == NULL) || (interface->send_on_off == NULL)) {
    return SL_STATUS_NOT_AVAILABLE;
  }

  return interface->send_on_off(unid, endpoint_id, false);
}

sl_status_t zigpc_command_translator_move_to_level(
  dotdot_unid_t unid,
  dotdot_endpoint_id_t endpoint_id,
  uic_mqtt_dotdot_callback_call_type_t call_type,
  uint8_t level,
  uint16_t transition_time,
  uint8_t options_mask,
  uint8_t options_override)
{
  const zigpc_ncp_interface_t *interface = zigpc_ncp_get_interface();

  (void)transition_time;
  (void)options_mask;
  (void)options_override;

  if (call_type == UIC_MQTT_DOTDOT_CALLBACK_TYPE_SUPPORT_CHECK) {
    return zigpc_command_translator_command_supported(
      (interface != NULL) && (interface->move_to_level != NULL));
  }

  if ((interface == NULL) || (interface->move_to_level == NULL)) {
    return SL_STATUS_NOT_AVAILABLE;
  }

  return interface->move_to_level(unid, endpoint_id, level);
}
