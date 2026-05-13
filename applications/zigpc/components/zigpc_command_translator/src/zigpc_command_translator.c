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

#include "unify_dotdot_attribute_store_helpers.h"
#include "zigpc_ncp.h"

static bool zigpc_command_translator_callbacks_registered = false;

static sl_status_t zigpc_command_translator_command_supported(bool supported)
{
  return supported ? SL_STATUS_OK : SL_STATUS_NOT_AVAILABLE;
}

static bool zigpc_command_translator_supports_on_off(dotdot_unid_t unid,
                                                     dotdot_endpoint_id_t endpoint_id)
{
  return dotdot_is_supported_on_off_on_off(unid, endpoint_id);
}

static bool zigpc_command_translator_supports_move_to_level(
  dotdot_unid_t unid, dotdot_endpoint_id_t endpoint_id)
{
  return dotdot_is_supported_level_current_level(unid, endpoint_id);
}

static bool zigpc_command_translator_backend_supports(bool operation_available)
{
  return operation_available;
}

static bool zigpc_command_translator_backend_available(bool operation_available)
{
  return zigpc_command_translator_backend_supports(operation_available)
         && zigpc_ncp_is_connected();
}

sl_status_t zigpc_command_translator_init(void)
{
  if (zigpc_command_translator_callbacks_registered) {
    return SL_STATUS_OK;
  }

  uic_mqtt_dotdot_on_off_generated_on_callback_set(zigpc_command_translator_on);
  uic_mqtt_dotdot_on_off_generated_off_callback_set(
    zigpc_command_translator_off);
  uic_mqtt_dotdot_level_generated_move_to_level_callback_set(
    zigpc_command_translator_move_to_level);
  zigpc_command_translator_callbacks_registered = true;

  return SL_STATUS_OK;
}

int zigpc_command_translator_teardown(void)
{
  if (zigpc_command_translator_callbacks_registered == false) {
    return 0;
  }

  uic_mqtt_dotdot_on_off_generated_on_callback_unset(zigpc_command_translator_on);
  uic_mqtt_dotdot_on_off_generated_off_callback_unset(
    zigpc_command_translator_off);
  uic_mqtt_dotdot_level_generated_move_to_level_callback_unset(
    zigpc_command_translator_move_to_level);
  zigpc_command_translator_callbacks_registered = false;

  return 0;
}

sl_status_t zigpc_command_translator_on(
  dotdot_unid_t unid,
  dotdot_endpoint_id_t endpoint_id,
  uic_mqtt_dotdot_callback_call_type_t call_type)
{
  const zigpc_ncp_interface_t *interface = zigpc_ncp_get_interface();

  if (call_type == UIC_MQTT_DOTDOT_CALLBACK_TYPE_SUPPORT_CHECK) {
    return zigpc_command_translator_command_supported(
      zigpc_command_translator_backend_supports(
        (interface != NULL) && (interface->send_on_off != NULL))
      && zigpc_command_translator_supports_on_off(unid, endpoint_id));
  }

  if ((zigpc_command_translator_backend_available(
         (interface != NULL) && (interface->send_on_off != NULL))
       == false)
      || (zigpc_command_translator_supports_on_off(unid, endpoint_id) == false)) {
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
      zigpc_command_translator_backend_supports(
        (interface != NULL) && (interface->send_on_off != NULL))
      && zigpc_command_translator_supports_on_off(unid, endpoint_id));
  }

  if ((zigpc_command_translator_backend_available(
         (interface != NULL) && (interface->send_on_off != NULL))
       == false)
      || (zigpc_command_translator_supports_on_off(unid, endpoint_id) == false)) {
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

  if (call_type == UIC_MQTT_DOTDOT_CALLBACK_TYPE_SUPPORT_CHECK) {
    return zigpc_command_translator_command_supported(
      zigpc_command_translator_backend_supports(
        (interface != NULL) && (interface->move_to_level != NULL))
      && zigpc_command_translator_supports_move_to_level(unid, endpoint_id));
  }

  if ((zigpc_command_translator_backend_available(
         (interface != NULL) && (interface->move_to_level != NULL))
       == false)
      || (zigpc_command_translator_supports_move_to_level(unid, endpoint_id)
          == false)) {
    return SL_STATUS_NOT_AVAILABLE;
  }

  return interface->move_to_level(unid,
                                  endpoint_id,
                                  level,
                                  transition_time,
                                  options_mask,
                                  options_override);
}
