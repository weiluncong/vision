/*
 *  Copyright(c) 2021 to 2023 AutoCore Technology (Nanjing) Co., Ltd. All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 */

/*
 * Copyright(c) 2006 to 2019 ADLINK Technology Limited and others
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
 * v. 1.0 which is available at
 * http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
 */
#ifndef DDSI_TYPEWRAP_H
#define DDSI_TYPEWRAP_H

#include "dds/features.h"

#include <stdbool.h>
#include <stdint.h>
#include "dds/ddsrt/static_assert.h"
#include "dds/ddsi/ddsi_xt_typeinfo.h"
#include "dds/ddsi/ddsi_xt_typemap.h"
#include "dds/ddsi/ddsi_typelib.h"

#if defined (__cplusplus)
extern "C" {
#endif

#ifdef DDS_HAS_TYPE_DISCOVERY

#define XT_FLAG_EXTENSIBILITY_MASK  0x7

struct xt_type;

typedef struct ddsi_typeid ddsi_typeid_t;
typedef struct ddsi_typeinfo ddsi_typeinfo_t;
typedef struct ddsi_typeobj ddsi_typeobj_t;
typedef struct ddsi_typemap ddsi_typemap_t;

typedef enum ddsi_typeid_kind {
  DDSI_TYPEID_KIND_MINIMAL,
  DDSI_TYPEID_KIND_COMPLETE,
  DDSI_TYPEID_KIND_FULLY_DESCRIPTIVE
} ddsi_typeid_kind_t;

DDS_EXPORT const char * ddsi_typekind_descr (unsigned char disc);

DDS_EXPORT void ddsi_typeid_copy (ddsi_typeid_t *dst, const ddsi_typeid_t *src);
DDS_EXPORT ddsi_typeid_t * ddsi_typeid_dup (const ddsi_typeid_t *src);
DDS_EXPORT int ddsi_typeid_compare (const ddsi_typeid_t *a, const ddsi_typeid_t *b);
DDS_EXPORT void ddsi_typeid_ser (const ddsi_typeid_t *type_id, unsigned char **buf, uint32_t *sz);
DDS_EXPORT void ddsi_typeid_fini (ddsi_typeid_t *type_id);
DDS_EXPORT bool ddsi_typeid_is_none (const ddsi_typeid_t *type_id);
DDS_EXPORT bool ddsi_typeid_is_hash (const ddsi_typeid_t *type_id);
DDS_EXPORT bool ddsi_typeid_is_minimal (const ddsi_typeid_t *type_id);
DDS_EXPORT bool ddsi_typeid_is_complete (const ddsi_typeid_t *type_id);
DDS_EXPORT void ddsi_typeid_get_equivalence_hash (const ddsi_typeid_t *type_id, DDS_XTypes_EquivalenceHash *hash);
DDS_EXPORT bool ddsi_type_id_with_deps_equal (const struct DDS_XTypes_TypeIdentifierWithDependencies *a, const struct DDS_XTypes_TypeIdentifierWithDependencies *b);
DDS_EXPORT ddsi_typeid_kind_t ddsi_typeid_kind (const ddsi_typeid_t *type);

DDS_EXPORT void ddsi_typeobj_fini (ddsi_typeobj_t *typeobj);

DDS_EXPORT dds_return_t ddsi_xt_type_init (struct ddsi_domaingv *gv, struct xt_type *xt, const ddsi_typeid_t *ti, const ddsi_typeobj_t *to);
DDS_EXPORT dds_return_t ddsi_xt_type_add_typeobj (struct ddsi_domaingv *gv, struct xt_type *xt, const struct DDS_XTypes_TypeObject *to);
DDS_EXPORT void ddsi_xt_get_typeobject (const struct xt_type *xt, ddsi_typeobj_t *to);
DDS_EXPORT void ddsi_xt_type_fini (struct ddsi_domaingv *gv, struct xt_type *xt);
DDS_EXPORT bool ddsi_xt_is_assignable_from (struct ddsi_domaingv *gv, const struct xt_type *t1, const struct xt_type *t2);


#else /* DDS_HAS_TYPE_DISCOVERY */

typedef void ddsi_typeid_t;
typedef int ddsi_typeid_kind_t;
typedef void ddsi_typemap_t;
typedef void ddsi_typeinfo_t;

#endif /* DDS_HAS_TYPE_DISCOVERY */

#if defined (__cplusplus)
}
#endif

#endif /* DDSI_TYPEWRAP_H */
