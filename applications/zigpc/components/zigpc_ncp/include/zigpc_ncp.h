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
#include "uic_typedefs.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef sl_status_t (*zigpc_ncp_endpoint_interview_callback_t)(
  const char *unid,
  dotdot_endpoint_id_t endpoint_id,
  bool supports_on_off,
  bool supports_level);

typedef struct {
  sl_status_t (*connect)(const char *cpc_instance);
  sl_status_t (*disconnect)(void);
  sl_status_t (*discover_network)(void);
  sl_status_t (*send_on_off)(const char *unid, uint8_t endpoint_id, bool on_off);
  sl_status_t (*move_to_level)(const char *unid, uint8_t endpoint_id, uint8_t level);
} zigpc_ncp_interface_t;

/**
 * @brief Set the active ZigPC NCP interface.
 *
 * Passing NULL clears the configured interface.
 *
 * @return SL_STATUS_OK on success, or the disconnect error when replacing or
 *         clearing an active interface fails.
 */
sl_status_t zigpc_ncp_set_interface(const zigpc_ncp_interface_t *interface);

/**
 * @brief Get the active ZigPC NCP interface.
 *
 * @return Pointer to the registered interface, or NULL when no interface is
 *         configured.
 */
const zigpc_ncp_interface_t *zigpc_ncp_get_interface(void);

/**
 * @brief Check whether the active ZigPC NCP backend is connected.
 *
 * @return true when the backend connection completed successfully, otherwise
 *         false.
 */
bool zigpc_ncp_is_connected(void);

/**
 * @brief Register a callback for discovered endpoint interview results.
 *
 * Passing NULL clears the registered callback.
 */
sl_status_t zigpc_ncp_register_endpoint_interview_callback(
  zigpc_ncp_endpoint_interview_callback_t callback);

/**
 * @brief Notify the registered callback about an interviewed endpoint.
 *
 * @return SL_STATUS_NOT_AVAILABLE if no callback is registered, otherwise the
 *         callback return status.
 */
sl_status_t zigpc_ncp_notify_endpoint_interviewed(
  const char *unid,
  dotdot_endpoint_id_t endpoint_id,
  bool supports_on_off,
  bool supports_level);

#ifdef __cplusplus
}
#endif

/** @} end of zigpc_ncp */
#endif  // ZIGPC_NCP_H
