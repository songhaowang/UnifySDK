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

#ifndef ZIGPC_COMMAND_TRANSLATOR_H
#define ZIGPC_COMMAND_TRANSLATOR_H

#include "dotdot_mqtt.h"
#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

sl_status_t zigpc_command_translator_init(void);
int zigpc_command_translator_teardown(void);

sl_status_t zigpc_command_translator_on(
  dotdot_unid_t unid,
  dotdot_endpoint_id_t endpoint_id,
  uic_mqtt_dotdot_callback_call_type_t call_type);

sl_status_t zigpc_command_translator_off(
  dotdot_unid_t unid,
  dotdot_endpoint_id_t endpoint_id,
  uic_mqtt_dotdot_callback_call_type_t call_type);

sl_status_t zigpc_command_translator_move_to_level(
  dotdot_unid_t unid,
  dotdot_endpoint_id_t endpoint_id,
  uic_mqtt_dotdot_callback_call_type_t call_type,
  uint8_t level,
  uint16_t transition_time,
  uint8_t options_mask,
  uint8_t options_override);

#ifdef __cplusplus
}
#endif

#endif  // ZIGPC_COMMAND_TRANSLATOR_H
