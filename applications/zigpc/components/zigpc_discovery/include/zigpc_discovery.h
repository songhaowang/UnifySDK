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

#ifndef ZIGPC_DISCOVERY_H
#define ZIGPC_DISCOVERY_H

#include "sl_status.h"
#include "uic_typedefs.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

sl_status_t zigpc_discovery_setup(void);

sl_status_t zigpc_discovery_init(void);

int zigpc_discovery_teardown(void);

sl_status_t zigpc_discovery_run_full_interview(void);

sl_status_t zigpc_discovery_handle_endpoint(const char *unid,
                                            dotdot_endpoint_id_t endpoint_id,
                                            bool supports_on_off,
                                            bool supports_level);

#ifdef __cplusplus
}
#endif

#endif  // ZIGPC_DISCOVERY_H
