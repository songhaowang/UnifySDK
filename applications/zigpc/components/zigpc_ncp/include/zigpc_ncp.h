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

/**
 * @defgroup zigpc_ncp ZigPC NCP Boundary
 * @ingroup zigpc_components
 *
 * @brief Boundary interface for ZigPC NCP operations.
 *
 * @{
 */

#ifndef ZIGPC_NCP_H
#define ZIGPC_NCP_H

#include "sl_status.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  sl_status_t (*connect)(const char *cpc_instance);
  sl_status_t (*discover_network)(void);
  sl_status_t (*send_on_off)(const char *unid, uint8_t endpoint_id, bool on_off);
  sl_status_t (*move_to_level)(const char *unid, uint8_t endpoint_id, uint8_t level);
} zigpc_ncp_interface_t;

/**
 * @brief Set the active ZigPC NCP interface.
 *
 * Passing NULL clears the configured interface.
 */
void zigpc_ncp_set_interface(const zigpc_ncp_interface_t *interface);

/**
 * @brief Get the active ZigPC NCP interface.
 */
const zigpc_ncp_interface_t *zigpc_ncp_get_interface(void);

#ifdef __cplusplus
}
#endif

/** @} end of zigpc_ncp */
#endif  // ZIGPC_NCP_H
