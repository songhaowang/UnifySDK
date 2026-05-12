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
 * @defgroup zigpc_ncp_fixture ZigPC NCP Fixture
 * @ingroup zigpc_ncp
 * @brief ZigPC NCP fixture.
 *
 * Fixture to initialize the zigpc_ncp boundary from uic_main,
 * to be used in @ref zigpc_ncp
 *
 * @{
 */

#ifndef ZIGPC_NCP_FIXT_H
#define ZIGPC_NCP_FIXT_H

#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Fixture for setting up the zigpc_ncp component.
 *
 * If no NCP interface is registered yet, setup is a no-op so ZigPC can start
 * before a production backend is added. When an interface is registered it
 * must provide both connect and disconnect lifecycle handlers.
 *
 * @return SL_STATUS_OK for success, SL_STATUS_INVALID_STATE when a registered
 *         interface is missing connect/disconnect handlers, or the connect
 *         handler status otherwise.
 */
sl_status_t zigpc_ncp_fixt_setup(void);

/**
 * @brief Fixture for tearing down the zigpc_ncp component.
 *
 * Teardown is a no-op when no NCP interface is registered or setup never
 * completed a connection.
 *
 * @return int 0 for success, -1 for disconnect failure
 */
int zigpc_ncp_fixt_teardown(void);

#ifdef __cplusplus
}
#endif

/** @} end of zigpc_ncp */
#endif  // ZIGPC_NCP_FIXT_H
