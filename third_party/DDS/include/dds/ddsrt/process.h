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
#ifndef DDSRT_PROCESS_H
#define DDSRT_PROCESS_H

#include "dds/export.h"
#include "dds/ddsrt/time.h"
#include "dds/ddsrt/types.h"
#include "dds/ddsrt/retcode.h"

#if DDSRT_WITH_FREERTOS
#include <FreeRTOS.h>
#include <task.h>
typedef TaskHandle_t ddsrt_pid_t; /* typedef void *TaskHandle_t */
#define PRIdPID "p"
#define DDSRT_HAVE_MULTI_PROCESS 0
/* DDSRT_WITH_FREERTOS */
#elif defined(_WIN32)
typedef uint32_t ddsrt_pid_t;
#define PRIdPID PRIu32
#define DDSRT_HAVE_MULTI_PROCESS 1
/* _WIN32 */
#else
#include <unistd.h>
#if defined(_WRS_KERNEL)
typedef RTP_ID ddsrt_pid_t; /* typedef struct wind_rtp *RTP_ID */
#define PRIdPID PRIuPTR
#define DDSRT_HAVE_MULTI_PROCESS 0
#else
typedef pid_t ddsrt_pid_t;
#define PRIdPID "d"
#define DDSRT_HAVE_MULTI_PROCESS 1
#endif
#endif


#if defined (__cplusplus)
extern "C" {
#endif

#define DDSRT_PROC_NAME_LEN     32     /* process name length */

#define DDSRT_PROC_PATH_LEN     128    /* process all-path length */

#define DDSRT_PROC_PID_PATH_LEN 32     /* the lenght of path '/proc/pid' */


/**
 * @brief Return process ID (PID) of the calling process.
 *
 * @returns The process ID of the calling process.
 */
DDS_EXPORT ddsrt_pid_t
ddsrt_getpid(void);

#if DDSRT_HAVE_MULTI_PROCESS



/***************************************************************************
* author      lilin
* date        2020/11/26
* brief       ddsrt get process execute path
* remarks     pid <=0:self process; pid:specified process;
****************************************************************************/
DDS_EXPORT void 
ddsrt_exe_path_get(
  char exe_path[DDSRT_PROC_PATH_LEN], 
  ddsrt_pid_t pid);

/***************************************************************************
* author      lilin
* date        2020/11/26
* brief       ddsrt get current process name
* remarks     
****************************************************************************/
DDS_EXPORT void 
ddsrt_proc_name_get(
  char proc_name[DDSRT_PROC_NAME_LEN]);

/***************************************************************************
* author      lilin
* date        2020/11/26
* brief       ddsrt get process name by pid
* remarks     pid <=0:self process; pid:specified process;
****************************************************************************/
DDS_EXPORT void 
ddsrt_proc_name_get_by_pid(
  char proc_name[DDSRT_PROC_NAME_LEN], 
  ddsrt_pid_t pid);



#endif /* DDSRT_HAVE_MULTI_PROCESS */

#if defined (__cplusplus)
}
#endif

#endif /* DDSRT_PROCESS_H */
