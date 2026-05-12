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
#include "zigpc_config.h"
#include "zigpc_config_fixt.h"

// Unify components
#include "config.h"
#include "sl_log.h"

// Generic includes
#include <string.h>

#define LOG_TAG "zigpc_config"

static zigpc_config_t config;

static config_status_t config_get_int_and_log(const char *key, int *value)
{
  config_status_t status = config_get_as_int(key, value);

  if (CONFIG_STATUS_OK != status) {
    sl_log_error(LOG_TAG, "Failed to get int for key: %s", key);
  }

  return status;
}

int zigpc_config_init(void)
{
  config_status_t status = CONFIG_STATUS_OK;

  status |= config_add_string(CONFIG_KEY_ZIGPC_DATASTORE_FILE,
                              "ZigPC datastore database file",
                              DEFAULT_ZIGPC_DATASTORE_FILE);
  status |= config_add_string(CONFIG_KEY_ZIGPC_CPC_INSTANCE,
                              "ZigPC CPC instance",
                              DEFAULT_ZIGPC_CPC_INSTANCE);

  return status != CONFIG_STATUS_OK;
}

sl_status_t zigpc_config_fixt_setup(void)
{
  memset(&config, 0, sizeof(config));
  config_status_t status = CONFIG_STATUS_OK;

  status |= config_get_as_string(CONFIG_KEY_ZIGPC_DATASTORE_FILE,
                                 &config.datastore_file);
  status |= config_get_as_string(CONFIG_KEY_ZIGPC_CPC_INSTANCE,
                                 &config.cpc_instance);
  status |= config_get_as_string(CONFIG_KEY_MQTT_HOST, &config.mqtt_host);
  status |= config_get_int_and_log(CONFIG_KEY_MQTT_PORT, &config.mqtt_port);

  return status == CONFIG_STATUS_OK ? SL_STATUS_OK : SL_STATUS_FAIL;
}

const zigpc_config_t *zigpc_get_config(void)
{
  return &config;
}
