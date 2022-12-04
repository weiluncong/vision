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

#ifndef DDSRT_SOCKETS_OSAL_H
#define DDSRT_SOCKETS_OSAL_H
#include <osapi.h>

#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/select.h>
#include "dds/ddsrt/iovec.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef osal_id_t ddsrt_socket_t;

typedef struct msghdr ddsrt_msghdr_t;

#define DDSRT_INVALID_SOCKET (-1)
#define PRIdSOCK "d"

# define DDSRT_HAVE_SSM         1
# define DDSRT_HAVE_INET_NTOP   1
# define DDSRT_HAVE_INET_PTON   1
# define DDSRT_MSGHDR_FLAGS 1

#if defined(__cplusplus)
}
#endif

#endif /* DDSRT_SOCKETS_OSAL_H */