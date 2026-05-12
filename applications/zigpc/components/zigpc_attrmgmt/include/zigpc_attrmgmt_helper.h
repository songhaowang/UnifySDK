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

#ifndef ZIGPC_ATTRMGMT_HELPER_H
#define ZIGPC_ATTRMGMT_HELPER_H

#include "attribute_store.h"
#include "sl_status.h"
#include "uic_typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

attribute_store_node_t zigpc_attrmgmt_get_unid_node(const dotdot_unid_t unid);

attribute_store_node_t
  zigpc_attrmgmt_create_unid_node(const dotdot_unid_t unid);

attribute_store_node_t zigpc_attrmgmt_get_endpoint_node(
  const dotdot_unid_t unid, dotdot_endpoint_id_t endpoint_id);

attribute_store_node_t zigpc_attrmgmt_create_endpoint_node(
  const dotdot_unid_t unid, dotdot_endpoint_id_t endpoint_id);

sl_status_t zigpc_attrmgmt_get_unid_from_node(attribute_store_node_t node,
                                              char *unid);

sl_status_t zigpc_attrmgmt_get_endpoint_from_node(
  attribute_store_node_t node,
  char *unid,
  dotdot_endpoint_id_t *endpoint_id);

#ifdef __cplusplus
}
#endif

#endif  // ZIGPC_ATTRMGMT_HELPER_H
