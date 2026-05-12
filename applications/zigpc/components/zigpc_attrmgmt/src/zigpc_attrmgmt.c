/******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
 ******************************************************************************
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 *****************************************************************************/

#include "zigpc_attrmgmt.h"
#include "zigpc_attrmgmt_attribute_types.h"
#include "zigpc_attrmgmt_helper.h"

#include "attribute_store_configuration.h"
#include "attribute_store_helper.h"
#include "attribute_store_type_registration.h"
#include "unify_dotdot_attribute_store.h"

static const unify_dotdot_attribute_store_configuration_t zigpc_configuration = {
  .get_endpoint_node_function                  = &zigpc_attrmgmt_get_endpoint_node,
  .get_unid_endpoint_function                  = &zigpc_attrmgmt_get_endpoint_from_node,
  .get_unid_function                           = &zigpc_attrmgmt_get_unid_from_node,
  .update_attribute_desired_values_on_commands = true,
  .clear_reported_on_desired_updates           = false,
  .automatic_deduction_of_supported_commands   = true,
  .force_read_attributes_enabled               = true,
  .write_attributes_enabled                    = true,
  .publish_desired_attribute_values_to_mqtt    = true,
  .publish_reported_attribute_values_to_mqtt   = true,
  .node_type                                   = ATTRIBUTE_UNID,
  .endpoint_type                               = ATTRIBUTE_ENDPOINT_ID,
};

static sl_status_t zigpc_attrmgmt_register_attribute_types(void)
{
  sl_status_t status = SL_STATUS_OK;

  status |= attribute_store_register_type(ATTRIBUTE_UNID,
                                          "UNID",
                                          ATTRIBUTE_TREE_ROOT,
                                          C_STRING_STORAGE_TYPE);
  status |= attribute_store_register_type(ATTRIBUTE_ENDPOINT_ID,
                                          "Endpoint ID",
                                          ATTRIBUTE_UNID,
                                          U8_STORAGE_TYPE);

  return status;
}

sl_status_t zigpc_attrmgmt_init(void)
{
  sl_status_t status = zigpc_attrmgmt_register_attribute_types();

  attribute_store_configuration_set_auto_save_safety_interval(10 * 60);
  attribute_store_configuration_set_auto_save_cooldown_interval(10);
  attribute_store_configuration_set_type_validation(true);

  unify_dotdot_attribute_store_set_configuration(&zigpc_configuration);
  status |= attribute_store_refresh_node_and_children_callbacks(
    attribute_store_get_root());

  return status;
}

sl_status_t zigpc_attrmgmt_publish_endpoint(const char *unid,
                                            dotdot_endpoint_id_t endpoint_id,
                                            bool supports_on_off,
                                            bool supports_level)
{
  if (unid == NULL) {
    return SL_STATUS_FAIL;
  }

  attribute_store_node_t endpoint_node
    = zigpc_attrmgmt_create_endpoint_node(unid, endpoint_id);

  if (endpoint_node == ATTRIBUTE_STORE_INVALID_NODE) {
    return SL_STATUS_FAIL;
  }

  if (supports_on_off) {
    attribute_store_node_t on_off_node
      = attribute_store_create_child_if_missing(endpoint_node,
                                                DOTDOT_ATTRIBUTE_ID_ON_OFF_ON_OFF);
    if (on_off_node == ATTRIBUTE_STORE_INVALID_NODE) {
      return SL_STATUS_FAIL;
    }
  }

  if (supports_level) {
    attribute_store_node_t level_node = attribute_store_create_child_if_missing(
      endpoint_node,
      DOTDOT_ATTRIBUTE_ID_LEVEL_CURRENT_LEVEL);
    if (level_node == ATTRIBUTE_STORE_INVALID_NODE) {
      return SL_STATUS_FAIL;
    }
  }

  return SL_STATUS_OK;
}
