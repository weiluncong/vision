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
#ifndef DDSRT_TYPES_H
#define DDSRT_TYPES_H

#include <stdbool.h>
#include <stdint.h>

#if _WIN32
# include "dds/ddsrt/types/windows.h"
#elif __VXWORKS__
# include "dds/ddsrt/types/vxworks.h"
#else
# include "dds/ddsrt/types/posix.h"
#endif

#define PRIdSIZE "zd"
#define PRIuSIZE "zu"
#define PRIxSIZE "zx"

#endif /* DDSRT_TYPES_H */


#ifndef DDSRT_CODE_PART
#define DDSRT_CODE_PART(X) 1
#endif


#if DDSRT_ENDIAN == DDSRT_LITTLE_ENDIAN
#define DDS_ENDIAN true
#else
#define DDS_ENDIAN false
#endif


typedef unsigned char byte;


#if DDS_ENDIAN
/* Host sequence & Network sequence swap */
#define DDS_HTON_16(x)                    ddsrt_bswap2(x)
#define DDS_NTOH_16(x)                    ddsrt_bswap2(x)
#define DDS_HTON_32(x)                    ddsrt_bswap4(x)
#define DDS_NTOH_32(x)                    ddsrt_bswap4(x)
#define DDS_HTON_64(x)                    ddsrt_bswap8(x)
#define DDS_NTOH_64(x)                    ddsrt_bswap8(x)

#define DDS_HTON_16U(x)                   ddsrt_bswap2u(x)
#define DDS_NTOH_16U(x)                   ddsrt_bswap2u(x)
#define DDS_HTON_32U(x)                   ddsrt_bswap4u(x)
#define DDS_NTOH_32U(x)                   ddsrt_bswap4u(x)
#define DDS_HTON_64U(x)                   ddsrt_bswap8u(x)
#define DDS_NTOH_64U(x)                   ddsrt_bswap8u(x)

#else
#define DDS_HTON_16(x)                    (x)                    
#define DDS_NTOH_16(x)                    (x)                    
#define DDS_HTON_32(x)                    (x)                    
#define DDS_NTOH_32(x)                    (x)                    
#define DDS_HTON_64(x)                    (x)                    
#define DDS_NTOH_64(x)                    (x)                    

#define DDS_HTON_16U(x)                   (x)                   
#define DDS_NTOH_16U(x)                   (x)                   
#define DDS_HTON_32U(x)                   (x)                   
#define DDS_NTOH_32U(x)                   (x)                     
#define DDS_HTON_64U(x)                   (x)                   
#define DDS_NTOH_64U(x)                   (x)                   

#endif



