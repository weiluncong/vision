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
 * Copyright(c) 2019 ADLINK Technology Limited and others
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
 * v. 1.0 which is available at
 * http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
 */
#ifndef DDSRT_STATIC_ASSERT_H
#define DDSRT_STATIC_ASSERT_H

/* There are many tricks to use a constant expression to yield an
   illegal type or expression at compile time, such as zero-sized
   arrays and duplicate case or enum labels. So this is but one of the
   many tricks. */

#define DDSRT_STATIC_ASSERT2(line, pred)        \
  struct static_assert_##line {                 \
    char cond[(pred) ? 1 : -1];                 \
  }
#define DDSRT_STATIC_ASSERT1(line, pred)        \
  DDSRT_STATIC_ASSERT2 (line, pred)
#define DDSRT_STATIC_ASSERT(pred)               \
  DDSRT_STATIC_ASSERT1 (__LINE__, pred)

#ifndef _MSC_VER
#define DDSRT_STATIC_ASSERT_CODE(pred) do { switch(0) { case 0: case (pred): ; } } while (0)
#else
/* Temporarily disabling warning C6326: Potential comparison of a
   constant with another constant. */
#define DDSRT_STATIC_ASSERT_CODE(pred) do {     \
    __pragma (warning (push))                   \
      __pragma (warning (disable : 6326))       \
      switch(0) { case 0: case (pred): ; }      \
    __pragma (warning (pop))                    \
  } while (0)
#endif

#endif
