/****************************************************************

  Generated by Eclipse Cyclone DDS IDL to C Translator
  File name: DDSData.c
  Source: DDSData.idl
  Cyclone DDS: V0.9.0

*****************************************************************/
#include "DDSData.h"

static const uint32_t DDSData_Msg_ops [] =
{
  /* DDSData_Msg */
  DDS_OP_ADR | DDS_OP_TYPE_STR, offsetof (DDSData_Msg, topic),
  DDS_OP_ADR | DDS_OP_TYPE_8BY | DDS_OP_FLAG_FP, offsetof (DDSData_Msg, timestamp),
  DDS_OP_ADR | DDS_OP_TYPE_SEQ | DDS_OP_SUBTYPE_1BY, offsetof (DDSData_Msg, payload),
  DDS_OP_RTS
};

const dds_topic_descriptor_t DDSData_Msg_desc =
{
  .m_size = sizeof (DDSData_Msg),
  .m_align = 8u,
  .m_flagset = DDS_TOPIC_NO_OPTIMIZE,
  .m_nkeys = 0u,
  .m_typename = "DDSData_Msg",
  .m_keys = NULL,
  .m_nops = 4,
  .m_ops = DDSData_Msg_ops,
  .m_meta = ""
};

