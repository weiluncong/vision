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
 * Copyright(c) 2020 ADLINK Technology Limited and others
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
 * v. 1.0 which is available at
 * http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
 */
#ifndef DDSRT_IOVEC_H
#define DDSRT_IOVEC_H

#if _WIN32
typedef unsigned ddsrt_iov_len_t;
typedef struct ddsrt_iovec {
  ddsrt_iov_len_t iov_len;
  void *iov_base;
} ddsrt_iovec_t;

// Equivalent to a DWORD
typedef unsigned long ddsrt_msg_iovlen_t;

#else // _WIN32

#if DDSRT_WITH_LWIP
#include <lwip/sockets.h>
#else
#include <stddef.h>
#include <sys/socket.h>
#endif

typedef struct iovec ddsrt_iovec_t;
typedef size_t ddsrt_iov_len_t;

#if defined(__linux) && !LWIP_SOCKET
typedef size_t ddsrt_msg_iovlen_t;
#else /* POSIX says int (which macOS, FreeBSD, Solaris do) */
typedef int ddsrt_msg_iovlen_t;
#endif

#endif // _WIN32

#endif
