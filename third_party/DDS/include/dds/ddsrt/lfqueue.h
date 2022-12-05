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

#ifndef ddsrt_LFQUEUE_H
#define ddsrt_LFQUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dds/ddsrt/types.h"



/**
 *\brief
 */
typedef enum ddsrt_lfq_type_eu
{
    LFQ_TYPE_START = 250,    /*!<  */
    LFQ_TYPE_INVALID,
    LFQ_TYPE_DEFAULT,

    LFQ_TYPE_END = 255
}DDSRT_LFQ_TYPE;



#define DDSRT_LFQ_NOT_ENOUGH      (int32_t)-5    /** RingBuff空间不足 */
#define DDSRT_LFQ_NO_DATA         (int32_t)-4    /** RingBuff无数据 */
#define DDSRT_LFQ_SIZE_SHORT      (int32_t)-3    /** 业务buff空间不够 */
#define DDSRT_LFQ_INVALID_PARAMS  (int32_t)-2
#define DDSRT_LFQ_ERROR           (int32_t)-1
#define DDSRT_LFQ_OK              (int32_t)0

#define DDSRT_APP_SEND_PACKET_MAX_LEN       (9600+2688)   /* 12*1024 */



/**
 Ring buff, contain the two pointer: the header and tailer:
 1. the Ring buff is empty: when the header == tailer
 2. the Ring buff is full: when the next location of tailer == header
 3. First in first out
 4. Element space can be reused
*/
typedef struct ddsrt_lf_ring_buff_st{
    uint32_t   index;
    uint32_t   count_pop_size_short;
    uint64_t   count_pop_get_next;
    uint64_t   count_pop;
    uint64_t   count_push;
    uint64_t   count_push_large;
    uint64_t   count_push_drop;
    
    int32_t    count;                   /** 业务数据个数 */
    uint32_t   size;
    char     *head;                   /** read Index */
    char     *tail;                   /** write Index */
    char     buf[sizeof(void*)];  /** 地址值不变,一直指向RingBuff起始地址*/
}DDSRT_LF_BUFF;


/*****************************************************************************/
/**
* \author      lilin
* \date        2021/06/01
* \brief       create a ddsrt_LF_BUFF
* \param[in]   size    the buffer size user want to create
* \return
*   - the pointer of ddsrt_LF_BUFF
*   - NULL if create failure when there is no available memory
* \remarks     if the size is `0`, it will create a empty ddsrt_LF_BUFF
               with size is 0
******************************************************************************/
DDSRT_LF_BUFF *ddsrt_lfq_create(uint32_t size);


/*****************************************************************************/
/**
* \author      lilin
* \date        2021/06/01
* \brief       函数描述
* \param[out]  rbuf       pointer of ddsrt_LF_BUFF
* \return      返回值描述
* \remarks     其它信息
******************************************************************************/
void ddsrt_lfq_destroy(DDSRT_LF_BUFF *rbuf);


/*****************************************************************************/
/**
* \author      lilin
* \date        2021/06/01
* \brief       push data into rbuf
* \param[out]  rbuf    pointer of ddsrt_LF_BUFF
* \param[in]   data    the data user want to push to rbuf
* \param[in]   size    the size of data
* \return      success: data length;
               failure: < 0
* \remarks     其它信息
******************************************************************************/
int32_t ddsrt_lfq_data_push(DDSRT_LF_BUFF *rbuf, void *data, uint32_t size);


/*****************************************************************************/
/**
* \author      lilin
* \date        2021/06/01
* \brief       pop data from rbuf to data
* \param[in]   rbuf    pointer of ddsrt_LF_BUFF
* \param[out]  data    the data will be copied to the user *buffer*
* \param[in]   size    the size of the user buf
* \return      success: data length;
               failure: < 0
* \remarks
******************************************************************************/
int32_t ddsrt_lfq_data_pop(DDSRT_LF_BUFF *rbuf, void *data, uint32_t size);


/*****************************************************************************/
/**
* \author      lilin
* \date        2021/06/01
* \brief       强制向后移动ringBuff的head指针
* \param[out]  rbuf      pointer of rbuf
* \param[in]   offset    偏移值
* \return      返回值描述
* \remarks     其它信息
******************************************************************************/
int32_t ddsrt_lfq_head_seek(DDSRT_LF_BUFF *rbuf,uint32_t offset);


/*****************************************************************************/
/**
* \author      lilin
* \date        2021/06/01
* \brief       强制向后移动ringBuff的tail指针
* \param[out]  rbuf      pointer of rbuf
* \param[in]   offset    偏移值
* \return      返回值描述
* \remarks     其它信息
******************************************************************************/
int32_t ddsrt_lfq_tail_seek(DDSRT_LF_BUFF *rbuf,uint32_t offset);


/*****************************************************************************/
/**
* \author      lilin
* \date        2021/06/01
* \brief       函数描述
* \param[in]   rbuf    pointer of ddsrt_LF_BUFF
* \param[in]   size    用户需要的buff大小
* \return      NULL表示当前RingBuff缓存空间不足,无法写入size大小数据
* \remarks     其它信息
******************************************************************************/
void *ddsrt_lfq_tlv_remain_get(DDSRT_LF_BUFF *rbuf, uint32_t size);


/*****************************************************************************/
/**
* \author      lilin
* \date        2021/06/01
* \brief       函数描述
* \param[out]  rbuf      输入参数描述
* \param[in]   offset    输入参数描述
* \return      返回值描述
* \remarks     必须配合ddsrt_lfq_tlv_remain_get()使用
******************************************************************************/
int32_t ddsrt_lfq_tlv_tail_seek(DDSRT_LF_BUFF *rbuf,uint32_t offset);


/*****************************************************************************/
/**
* \author      lilin
* \date        2021/06/01
* \brief       函数描述
* \param[out]  rbuf    输入参数描述
* \param[in]   data    输入参数描述
* \param[in]   data_type    输入参数描述
* \param[in]   data_size    输入参数描述
* \return      success: data length;
               failure: < 0
* \remarks
******************************************************************************/
int32_t ddsrt_lfq_tlv_push(DDSRT_LF_BUFF *rbuf, byte data_type, void *data, uint32_t data_size);


/*****************************************************************************/
/**
* \author      lilin
* \date        2021/06/01
* \brief       pop data from rbuf to data
* \param[in]   rbuf         pointer of ddsrt_LF_BUFF
* \param[out]  data_type    the data type
* \param[out]  data         the data will be copied to the user *buffer*
* \param[in]   size         the size of the user buf
* \return      success: data length;
               failure: < 0
* \remarks
******************************************************************************/
int32_t ddsrt_lfq_tlv_pop(DDSRT_LF_BUFF *rbuf, byte *data_type,void *data, uint32_t size);

/*****************************************************************************/
/**
* \author      lilin
* \date        2021/06/01
* \brief       函数描述
* \param[in]   flag    输入参数描述
* \return      返回值描述
* \remarks     其它信息
******************************************************************************/
void ddsrt_lfq_trace_info_set(bool flag);


#ifdef __cplusplus
}
#endif


#endif

