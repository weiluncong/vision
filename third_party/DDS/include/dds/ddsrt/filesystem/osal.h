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

#ifndef DDSRT_FILESYSTEM_OSAL_H
#define DDSRT_FILESYSTEM_OSAL_H
#include <sys/stat.h>
#include <osapi.h>

typedef osal_id_t ddsrt_dir_handle_t;

#ifdef _WIN32
typedef unsigned short ddsrt_mode_t;
#else
typedef mode_t ddsrt_mode_t;
#endif

#define DDSRT_PATH_MAX OS_MAX_FILE_NAME - 1

#ifdef _WIN32
#define DDSRT_FILESEPCHAR '\\'
#else
#define DDSRT_FILESEPCHAR '/'
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif /* DDSRT_FILESYSTEM_OSAL_H */