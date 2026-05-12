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

#include "zigpc_attrmgmt_helper.h"
#include "zigpc_attrmgmt_attribute_types.h"

#include "attribute_store_helper.h"
#include "unify_dotdot_attribute_store.h"

#include <string.h>

attribute_store_node_t zigpc_attrmgmt_get_unid_node(const dotdot_unid_t unid)
{
  if (unid == NULL) {
    return ATTRIBUTE_STORE_INVALID_NODE;
  }

  return attribute_store_get_node_child_by_value(attribute_store_get_root(),
                                                 ATTRIBUTE_UNID,
                                                 REPORTED_ATTRIBUTE,
                                                 unid,
                                                 strlen(unid) + 1,
                                                 0);
}

attribute_store_node_t
  zigpc_attrmgmt_create_unid_node(const dotdot_unid_t unid)
{
  if (unid == NULL) {
    return ATTRIBUTE_STORE_INVALID_NODE;
  }

  attribute_store_node_t unid_node = zigpc_attrmgmt_get_unid_node(unid);

  if (unid_node != ATTRIBUTE_STORE_INVALID_NODE) {
    return unid_node;
  }

  unid_node = attribute_store_add_node(ATTRIBUTE_UNID, attribute_store_get_root());
  attribute_store_set_reported_string(unid_node, unid);
  return unid_node;
}

attribute_store_node_t zigpc_attrmgmt_get_endpoint_node(
  const dotdot_unid_t unid, dotdot_endpoint_id_t endpoint_id)
{
  attribute_store_node_t unid_node = zigpc_attrmgmt_get_unid_node(unid);

  if (unid_node == ATTRIBUTE_STORE_INVALID_NODE) {
    return ATTRIBUTE_STORE_INVALID_NODE;
  }

  return attribute_store_get_node_child_by_value(unid_node,
                                                 ATTRIBUTE_ENDPOINT_ID,
                                                 REPORTED_ATTRIBUTE,
                                                 &endpoint_id,
                                                 sizeof(endpoint_id),
                                                 0);
}

attribute_store_node_t zigpc_attrmgmt_create_endpoint_node(
  const dotdot_unid_t unid, dotdot_endpoint_id_t endpoint_id)
{
  attribute_store_node_t unid_node = zigpc_attrmgmt_create_unid_node(unid);

  if (unid_node == ATTRIBUTE_STORE_INVALID_NODE) {
    return ATTRIBUTE_STORE_INVALID_NODE;
  }

  return attribute_store_emplace(unid_node,
                                 ATTRIBUTE_ENDPOINT_ID,
                                 &endpoint_id,
                                 sizeof(endpoint_id));
}

sl_status_t zigpc_attrmgmt_get_unid_from_node(attribute_store_node_t node,
                                              char *unid)
{
  attribute_store_node_t unid_node = node;

  if (unid != NULL) {
    unid[0] = '\0';
  }

  if (node == ATTRIBUTE_STORE_INVALID_NODE) {
    return SL_STATUS_FAIL;
  }

  if (attribute_store_get_node_type(node) != ATTRIBUTE_UNID) {
    unid_node = attribute_store_get_first_parent_with_type(node, ATTRIBUTE_UNID);
  }

  if ((unid == NULL) || (unid_node == ATTRIBUTE_STORE_INVALID_NODE)) {
    return SL_STATUS_FAIL;
  }

  return attribute_store_get_reported_string(unid_node, unid, MAXIMUM_UNID_SIZE);
}

sl_status_t zigpc_attrmgmt_get_endpoint_from_node(
  attribute_store_node_t node,
  char *unid,
  dotdot_endpoint_id_t *endpoint_id)
{
  attribute_store_node_t endpoint_node = node;
  sl_status_t status                   = SL_STATUS_OK;

  if (unid != NULL) {
    unid[0] = '\0';
  }

  if ((endpoint_id == NULL) || (node == ATTRIBUTE_STORE_INVALID_NODE)) {
    return SL_STATUS_FAIL;
  }

  if (attribute_store_get_node_type(node) != ATTRIBUTE_ENDPOINT_ID) {
    endpoint_node
      = attribute_store_get_first_parent_with_type(node, ATTRIBUTE_ENDPOINT_ID);
  }

  if (endpoint_node == ATTRIBUTE_STORE_INVALID_NODE) {
    return SL_STATUS_FAIL;
  }

  status
    = attribute_store_get_reported(endpoint_node, endpoint_id, sizeof(*endpoint_id));
  status |= zigpc_attrmgmt_get_unid_from_node(endpoint_node, unid);
  return status;
}
