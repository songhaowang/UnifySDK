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

#ifndef ZIGPC_ATTRMGMT_ATTRIBUTE_TYPES_H
#define ZIGPC_ATTRMGMT_ATTRIBUTE_TYPES_H

#include "attribute_store.h"
#include "uic_attribute_definitions.h"
#include "unify_dotdot_defined_attribute_types.h"

///< ZigPC UNID. C-string storage.
DEFINE_ATTRIBUTE(ATTRIBUTE_UNID, 0x0002)
///< ZigPC endpoint ID. dotdot_endpoint_id_t storage.
DEFINE_ATTRIBUTE(ATTRIBUTE_ENDPOINT_ID, 0x0003)

#endif  // ZIGPC_ATTRMGMT_ATTRIBUTE_TYPES_H
