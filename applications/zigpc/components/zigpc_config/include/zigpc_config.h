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
 * @defgroup zigpc_config ZigPC Configuration Extension
 * @ingroup zigpc_components
 *
 * @brief Add the ZigPC-specific fixtures to the Unify \ref config system.
 *
 * This module is the source of all ZigPC-specific configuration parameters.
 * The source of the configuration parameters are command line arguments
 * and the configuration file.
 *
 * The after initialization the configurations parameters are constant.
 *
 * @{
 */

#ifndef ZIGPC_CONFIG_H
#define ZIGPC_CONFIG_H

// Unify includes
#include "uic_version.h"

// Default setting for the zigpc.datastore_file.
#define DEFAULT_ZIGPC_DATASTORE_FILE UIC_VAR_DIR "/zigpc.db"

// Default setting for the zigpc.cpc_instance.
#define DEFAULT_ZIGPC_CPC_INSTANCE "cpcd_0"

// Config key for the ZigPC datastore file
#define CONFIG_KEY_ZIGPC_DATASTORE_FILE "zigpc.datastore_file"

// Config key for the ZigPC CPC instance
#define CONFIG_KEY_ZIGPC_CPC_INSTANCE "zigpc.cpc_instance"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  /// File name for datastore/persistent storage
  const char *datastore_file;
  /// CPC instance to connect ZigPC against
  const char *cpc_instance;
  /// Hostname of the MQTT broker
  const char *mqtt_host;
  /// Port of the MQTT broker
  int mqtt_port;
} zigpc_config_t;

/**
 * @brief Get the current configuration. This must only be called after
 * zigpc_config_init.
 */
const zigpc_config_t *zigpc_get_config(void);

/**
 * @brief Register ZigPC configurations in \ref config.
 *
 * This must be called before \ref uic_main.
 *
 * @returns 0 on success.
 */
int zigpc_config_init(void);

#ifdef __cplusplus
}
#endif

/** @} end zigpc_config */

#endif  // ZIGPC_CONFIG_H
