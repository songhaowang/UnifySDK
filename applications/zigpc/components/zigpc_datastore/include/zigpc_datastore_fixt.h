/******************************************************************************
 * # License
 * <b>Copyright 2026 Silicon Laboratories Inc. www.silabs.com</b>
 ******************************************************************************
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at www.silabs.com/about-us/legal/master-software-license-agreement.
 * This software is distributed to you in Source Code format and is governed by
 * the sections of the MSLA applicable to Source Code.
 *
 *****************************************************************************/

/**
 * @defgroup zigpc_datastore_fixture ZigPC Datastore Fixture
 * @ingroup zigpc_components
 * @brief ZigPC specific functions using the @ref unify_datastore.
 *
 * Fixture to initialize the zigpc_datastore from uic_main,
 * to be used in zigpc_datastore
 *
 * @{
 */

#ifndef ZIGPC_DATASTORE_FIXT_H
#define ZIGPC_DATASTORE_FIXT_H

#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Current datastore version used by ZigPC.
 */
#define ZIGPC_DATASTORE_VERSION 1

/**
 * @brief Fixture for setting up the zigpc_datastore component.
 *
 * @return SL_STATUS_OK for success, SL_STATUS_FAIL if an error occurred
 */
sl_status_t zigpc_datastore_fixt_setup(void);

#ifdef __cplusplus
}
#endif

/** @} end of zigpc_datastore */
#endif  // ZIGPC_DATASTORE_FIXT_H
