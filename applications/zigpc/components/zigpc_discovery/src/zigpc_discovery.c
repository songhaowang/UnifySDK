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

#include "zigpc_discovery.h"

#include "zigpc_attrmgmt.h"
#include "zigpc_ncp.h"
#include "zigpc_ncp_fixt.h"

sl_status_t zigpc_discovery_setup(void)
{
  return zigpc_ncp_register_endpoint_interview_callback(
    zigpc_discovery_handle_endpoint);
}

sl_status_t zigpc_discovery_init(void)
{
  const zigpc_ncp_interface_t *interface = zigpc_ncp_get_interface();
  sl_status_t status = SL_STATUS_OK;

  if (interface == NULL) {
    return SL_STATUS_OK;
  }

  if (zigpc_ncp_is_connected() == false) {
    return SL_STATUS_NOT_AVAILABLE;
  }

  if (interface->discover_network == NULL) {
    status = SL_STATUS_INVALID_STATE;
  } else {
    status = zigpc_discovery_run_full_interview();
  }

  if ((status != SL_STATUS_OK) && (status != SL_STATUS_NOT_AVAILABLE)) {
    zigpc_ncp_register_endpoint_interview_callback(NULL);
    if (zigpc_ncp_fixt_teardown() != 0) {
      return SL_STATUS_FAIL;
    }
  }

  return status;
}

int zigpc_discovery_teardown(void)
{
  return (zigpc_ncp_register_endpoint_interview_callback(NULL) == SL_STATUS_OK)
           ? 0
           : -1;
}

sl_status_t zigpc_discovery_run_full_interview(void)
{
  const zigpc_ncp_interface_t *ncp_interface = zigpc_ncp_get_interface();

  if ((ncp_interface == NULL) || (ncp_interface->discover_network == NULL)
      || (zigpc_ncp_is_connected() == false)) {
    return SL_STATUS_NOT_AVAILABLE;
  }

  return ncp_interface->discover_network();
}

sl_status_t zigpc_discovery_handle_endpoint(const char *unid,
                                            dotdot_endpoint_id_t endpoint_id,
                                            bool supports_on_off,
                                            bool supports_level)
{
  return zigpc_attrmgmt_publish_endpoint(unid,
                                         endpoint_id,
                                         supports_on_off,
                                         supports_level);
}
