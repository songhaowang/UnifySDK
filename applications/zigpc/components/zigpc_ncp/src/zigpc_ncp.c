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
#include <string.h>

static zigpc_ncp_interface_t zigpc_ncp_interface;

void zigpc_ncp_set_interface(const zigpc_ncp_interface_t *interface)
{
  if (interface == NULL) {
    memset(&zigpc_ncp_interface, 0, sizeof(zigpc_ncp_interface));
  } else {
    zigpc_ncp_interface = *interface;
  }
}

const zigpc_ncp_interface_t *zigpc_ncp_get_interface(void)
{
  return &zigpc_ncp_interface;
}

sl_status_t zigpc_ncp_fixt_setup(void)
{
  const zigpc_ncp_interface_t *interface = zigpc_ncp_get_interface();

  if ((interface == NULL) || (interface->connect == NULL)) {
    return SL_STATUS_INVALID_STATE;
  }

  return interface->connect(zigpc_get_config()->cpc_instance);
}
