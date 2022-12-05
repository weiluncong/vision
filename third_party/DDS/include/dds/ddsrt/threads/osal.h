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
 * Copyright(c) 2006 to 2018 ADLINK Technology Limited and others
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
 * v. 1.0 which is available at
 * http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
 */
#ifndef DDSRT_OSAL_THREAD_H
#define DDSRT_OSAL_THREAD_H

#include "osapi.h"

#if defined(__VXWORKS__)
#define DDSRT_HAVE_THREAD_SETNAME (0)
#else
#define DDSRT_HAVE_THREAD_SETNAME (1)
#endif
#if defined (__linux) || defined (__APPLE__)
#define DDSRT_HAVE_THREAD_LIST (1)
#else
#define DDSRT_HAVE_THREAD_LIST (0)
#endif

#if defined (__cplusplus)
extern "C" {
#endif


typedef osal_id_t ddsrt_tid_t;
#define PRIdTID "ld"
typedef long int ddsrt_thread_list_id_t;

/* Wrapped in a struct to force conformation to abstraction. */
typedef struct {
    osal_id_t v;
} ddsrt_thread_t;

#if defined (__cplusplus)
}
#endif

#endif /* DDSRT_POSIX_THREAD_H */
