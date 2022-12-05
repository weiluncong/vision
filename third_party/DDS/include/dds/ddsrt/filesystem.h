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

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stddef.h>

#include "dds/export.h"
#include "dds/ddsrt/retcode.h"
#include "dds/ddsrt/time.h"

#if !DDSRT_WITH_FREERTOS
#define DDSRT_HAVE_FILESYSTEM (1)
#else
#define DDSRT_HAVE_FILESYSTEM (0)
#endif

#if DDSRT_HAVE_FILESYSTEM

#if _WIN32
#include "dds/ddsrt/filesystem/windows.h"
#elif DDSRT_WITH_OSAL
#include "dds/ddsrt/filesystem/osal.h"
#else
#include "dds/ddsrt/filesystem/posix.h"
#endif

#if defined (__cplusplus)
extern "C" {
#endif


struct ddsrt_stat {
  ddsrt_mode_t stat_mode;
  size_t stat_size;
  dds_time_t  stat_mtime;
};


struct ddsrt_dirent {
    char d_name[DDSRT_PATH_MAX + 1];
};


/**
 * @brief create directory
 *
 * @param muldir
 * @return dds_return_t
 */
DDS_EXPORT dds_return_t ddsrt_mkdirs(char *muldir);

/** \brief opendir wrapper
 *
 * Open the directory conform opendir
 *
 * Precondition:
 *   none
 *
 * Possible results:
 * - return DDS_RETCODE_OK if directory 'name' is opened
 * - DDS_RETCODE_ERROR if 'name' could not
 *     be found or is not a directory.
 */
DDS_EXPORT dds_return_t ddsrt_opendir(const char *name, ddsrt_dir_handle_t *dir);

/** \brief closedir wrapper
 *
 * Close the directory conform closdir
 *
 * Precondition:
 *   none
 *
 * Possible results:
 * - return DDS_RETCODE_OK if directory identified by the handle
 *     is succesfully closed
 * - return DDS_RETCODE_ERROR if the handle is invalid.
 */
DDS_EXPORT dds_return_t ddsrt_closedir(ddsrt_dir_handle_t d);

/** \brief readdir wrapper
 *
 * Read the directory conform readdir.
 *
 * Precondition:
 *   none
 *
 * Possible results:
 * - return DDS_RETCODE_OK if next directory is found
 * - return DDS_RETCODE_ERROR if no more directories are found.
 */
DDS_EXPORT dds_return_t ddsrt_readdir(ddsrt_dir_handle_t d, struct ddsrt_dirent *direntp);

/** \brief stat wrapper
 *
 * Gets directory status conform stat.
 *
 * Precondition:
 *   none
 *
 * Possible results:
 * - return DDS_RETCODE_OK if stat is successful
 * - return DDS_RETCODE_ERROR if stat fails.
 */
DDS_EXPORT dds_return_t ddsrt_stat(const char *path, struct ddsrt_stat *buf);

/** \brief Transforms the given filepath into a platform specific filepath.
 *
 * This translation function will replace any platform file seperator into
 * the fileseperator of the current platform. Doulbe quotes are removed
 * as well.
 *
 * Precondition:
 *   none
 *
 * Possible results:
 * - returns normalized filepath conform current platform
 * - return NULL if out of memory.
 */
DDS_EXPORT char* ddsrt_file_normalize(const char *filepath);

/** \brief Get file seperator
 *
 * Possible Results:
 * - "<file-seperator-string>"
 */
DDS_EXPORT const char* ddsrt_file_sep(void);

#if defined (__cplusplus)
}
#endif

#endif // DDRT_HAVE_FILESYSTEM

#endif // FILESYSTEM_H
