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

// Includes from this component
#include "zigpc_ncp.h"
#include "zigpc_ncp_fixt.h"

// ZigPC component includes
#include "zigpc_config.h"

// Generic includes
#include <stdbool.h>
#include <string.h>

static zigpc_ncp_interface_t zigpc_ncp_interface;
static bool zigpc_ncp_interface_registered = false;
static bool zigpc_ncp_connected            = false;
static zigpc_ncp_endpoint_interview_callback_t endpoint_interview_callback = NULL;

sl_status_t zigpc_ncp_set_interface(const zigpc_ncp_interface_t *interface)
{
  if (zigpc_ncp_connected && zigpc_ncp_interface_registered) {
    if (zigpc_ncp_interface.disconnect == NULL) {
      return SL_STATUS_INVALID_STATE;
    }

    sl_status_t status = zigpc_ncp_interface.disconnect();
    if (status != SL_STATUS_OK) {
      return status;
    }
    zigpc_ncp_connected = false;
  }

  if (interface == NULL) {
    memset(&zigpc_ncp_interface, 0, sizeof(zigpc_ncp_interface));
    zigpc_ncp_interface_registered = false;
  } else {
    zigpc_ncp_interface = *interface;
    zigpc_ncp_interface_registered = true;
  }

  return SL_STATUS_OK;
}

const zigpc_ncp_interface_t *zigpc_ncp_get_interface(void)
{
  return zigpc_ncp_interface_registered ? &zigpc_ncp_interface : NULL;
}

bool zigpc_ncp_is_connected(void)
{
  return zigpc_ncp_connected;
}

sl_status_t zigpc_ncp_register_endpoint_interview_callback(
  zigpc_ncp_endpoint_interview_callback_t callback)
{
  endpoint_interview_callback = callback;
  return SL_STATUS_OK;
}

sl_status_t zigpc_ncp_notify_endpoint_interviewed(
  const char *unid,
  dotdot_endpoint_id_t endpoint_id,
  bool supports_on_off,
  bool supports_level)
{
  if (endpoint_interview_callback == NULL) {
    return SL_STATUS_NOT_AVAILABLE;
  }

  return endpoint_interview_callback(unid,
                                     endpoint_id,
                                     supports_on_off,
                                     supports_level);
}

sl_status_t zigpc_ncp_fixt_setup(void)
{
  const zigpc_ncp_interface_t *interface = zigpc_ncp_get_interface();

  if (interface == NULL) {
    return SL_STATUS_OK;
  }

  if ((interface->connect == NULL) || (interface->disconnect == NULL)) {
    return SL_STATUS_INVALID_STATE;
  }

  sl_status_t status = interface->connect(zigpc_get_config()->cpc_instance);

  zigpc_ncp_connected = (status == SL_STATUS_OK);

  return status;
}

int zigpc_ncp_fixt_teardown(void)
{
  const zigpc_ncp_interface_t *interface = zigpc_ncp_get_interface();

  if ((interface == NULL) || (zigpc_ncp_connected == false)) {
    return 0;
  }

  if (interface->disconnect == NULL) {
    return -1;
  }

  sl_status_t status = interface->disconnect();

  if (status == SL_STATUS_OK) {
    zigpc_ncp_connected = false;
    return 0;
  }

  return -1;
}
