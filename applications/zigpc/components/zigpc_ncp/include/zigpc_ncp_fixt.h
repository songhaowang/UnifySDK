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
 * This requires an NCP connect handler to be configured before setup.
 *
 * @return SL_STATUS_OK for success, SL_STATUS_INVALID_STATE when no connect
 *         handler is configured, or the connect handler status otherwise.
 */
sl_status_t zigpc_ncp_fixt_setup(void);

#ifdef __cplusplus
}
#endif

/** @} end of zigpc_ncp */
#endif  // ZIGPC_NCP_FIXT_H
