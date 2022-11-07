/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces:
-------------------------------------------------------------------------------
MDF4Lib public unmanaged C++ interface: type definitions
See API documentation for details
-------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
-----------------------------------------------------------------------------*/

#pragma once

#include "MDF4LibEnums.h"
#include "MDF4LibStatusCodes.h"

///////////////////////////////////////////////////
//! \file
//! \brief type definitions for Enumerations

#ifdef _USE_ENUM_DEFS_

// Attention: this must only be used for help compiler

typedef XmlDataTypeEnum              XmlDataType;
typedef ChannelDataTypeEnum          ChannelDataType;
typedef ChannelTypeEnum              ChannelType;
typedef SyncTypeEnum                 SyncType;
typedef ArrayTypeEnum                ArrayType;
typedef ArrayPropertyFlagsEnum       ArrayPropertyFlags;
typedef CompositionTypeEnum          CompositionType;
typedef ComponentTypeEnum            ComponentType;
typedef SourceTypeEnum               SourceType;
typedef SourceBusTypeEnum            SourceBusType;
typedef EventTypeEnum                EventType;
typedef EventFilterFlagsEnum         EventFilterFlags;
typedef CommentFieldFilterFlagsEnum  CommentFieldFilterFlags;
typedef ScopeTypeEnum                ScopeType;
typedef HierarchyTypeEnum            HierarchyType;
typedef EventCauseEnum               EventCause;
typedef RangeTypeEnum                RangeType;
typedef ConversionTypeEnum           ConversionType;
typedef ConversionResultTypeEnum     ConversionResultType;
typedef AttachmentTypeEnum           AttachmentType;
typedef ZipTypeEnum                  ZipType;
typedef ZipTypeAndLevelEnum          ZipTypeAndLevel;
typedef BusEventTypeEnum             BusEventType;
typedef TimeStampTypeEnum            TimeStampType;
typedef ChannelFlagsEnum             ChannelFlags;
typedef ChannelFilterFlagsEnum       ChannelFilterFlags;
typedef ChannelSubSetFilterFlagsEnum ChannelSubSetFilterFlags;
typedef AttachmentFlagsEnum          AttachmentFlags;
typedef DeleteAttachmentFlagsEnum    DeleteAttachmentFlags;
typedef TimeFlagsEnum                TimeFlags;
typedef OpenFileModeEnum             OpenFileMode;
typedef DataReadModeEnum             DataReadMode;
typedef DataWriteModeEnum            DataWriteMode;
typedef TimeCheckModeEnum            TimeCheckMode;
typedef FlushCheckModeEnum           FlushCheckMode;
typedef SeekTimeModeEnum             SeekTimeMode;
typedef SeekPosModeEnum              SeekPosMode;
typedef ValueAccessModeEnum          ValueAccessMode;
typedef LicenseLevelEnum             LicenseLevel;
typedef LogSinkLevelEnum             LogSinkLevel;
typedef SortFileOptionFlagsEnum      SortFileOptionFlags;
typedef CreateFileOptionFlagsEnum    CreateFileOptionFlags;
typedef FinalizeFileOptionFlagsEnum  FinalizeFileOptionFlags;
typedef ValidateFileOptionFlagsEnum  ValidateFileOptionFlags;
typedef RewriteFileOptionFlagsEnum   RewriteFileOptionFlags;
typedef DataPointerOptionsFlagsEnum  DataPointerOptionsFlags;
typedef CanReadChannelValueFlagsEnum CanReadChannelValueFlags;
typedef LimitTypeEnum                LimitType;
typedef SyntaxVersionEnum            SyntaxVersion;
typedef FieldDataTypeEnum            FieldDataType;
typedef StandardFieldTypeEnum        StandardFieldType;
typedef CommentNodeTypeEnum          CommentNodeType;
typedef CommentFieldTypeEnum         CommentFieldType;
typedef CommentParentTypeEnum        CommentParentType;
typedef CommentMergeFlagsEnum        CommentMergeFlags;
typedef TimeSrcClassEnum             TimeSrcClass;
typedef GetFirstAndLastTimeStampOptionsFlagsEnum GetFirstAndLastTimeStampOptionsFlags;
typedef ErrorCodeEnum                ErrorCode;

#elif defined _USE_ENUM_

// Attention: this must only be used for help compiler

#define XmlDataType              XmlDataTypeEnum
#define ChannelDataType          ChannelDataTypeEnum
#define ChannelType              ChannelTypeEnum
#define SyncType                 SyncTypeEnum
#define ArrayType                ArrayTypeEnum
#define ArrayPropertyFlags       ArrayPropertyFlagsEnum
#define CompositionType          CompositionTypeEnum
#define ComponentType            ComponentTypeEnum
#define SourceType               SourceTypeEnum
#define SourceBusType            SourceBusTypeEnum
#define EventType                EventTypeEnum
#define EventFilterFlags         EventFilterFlagsEnum
#define CommentFieldFilterFlags  CommentFieldFilterFlagsEnum
#define ScopeType                ScopeTypeEnum
#define HierarchyType            HierarchyTypeEnum
#define EventCause               EventCauseEnum
#define RangeType                RangeTypeEnum
#define ConversionType           ConversionTypeEnum
#define ConversionResultType     ConversionResultTypeEnum
#define AttachmentType           AttachmentTypeEnum
#define ZipType                  ZipTypeEnum
#define ZipTypeAndLevel          ZipTypeAndLevelEnum
#define BusEventType             BusEventTypeEnum
#define TimeStampType            TimeStampTypeEnum
#define ChannelFlags             ChannelFlagsEnum
#define ChannelFilterFlags       ChannelFilterFlagsEnum
#define ChannelSubSetFilterFlags ChannelSubSetFilterFlagsEnum;
#define AttachmentFlags          AttachmentFlagsEnum
#define DeleteAttachmentFlags    DeleteAttachmentFlagsEnum
#define TimeFlags                TimeFlagsEnum
#define OpenFileMode             OpenFileModeEnum
#define DataReadMode             DataReadModeEnum
#define DataWriteMode            DataWriteModeEnum
#define TimeCheckMode            TimeCheckModeEnum
#define FlushCheckMode           FlushCheckModeEnum
#define SeekTimeMode             SeekTimeModeEnum
#define SeekPosMode              SeekPosModeEnum
#define ValueAccessMode          ValueAccessModeEnum
#define LicenseLevel             LicenseLevelEnum
#define LogSinkLevel             LogSinkLevelEnum
#define SortFileOptionFlags      SortFileOptionFlagsEnum
#define CreateFileOptionFlags    CreateFileOptionFlagsEnum
#define FinalizeFileOptionFlags  FinalizeFileOptionFlagsEnum
#define ValidateFileOptionFlags  ValidateFileOptionFlagsEnum
#define RewriteFileOptionFlags   RewriteFileOptionFlagsEnum
#define DataPointerOptionsFlags  DataPointerOptionsFlagsEnum
#define CanReadChannelValueFlags CanReadChannelValueFlagsEnum
#define LimitType                LimitTypeEnum
#define SyntaxVersion            SyntaxVersionEnum
#define FieldDataType            FieldDataTypeEnum
#define StandardFieldType        StandardFieldTypeEnum
#define CommentNodeType          CommentNodeTypeEnum;
#define CommentFieldType         CommentFieldTypeEnum;
#define CommentParentType        CommentParentTypeEnum;
#define CommentMergeFlags        CommentMergeFlagsEnum;
#define TimeSrcClass             TimeSrcClassEnum;
#define GetFirstAndLastTimeStampOptionsFlags GetFirstAndLastTimeStampOptionsFlagsEnum;
#define ErrorCode                ErrorCodeEnum

#else

typedef INT16 XmlDataType;
typedef INT16 ChannelDataType;
typedef INT16 ChannelType;
typedef INT16 SyncType;
typedef INT16 ArrayType;
typedef INT32 ArrayPropertyFlags;
typedef INT16 CompositionType;
typedef INT16 ComponentType;
typedef INT16 SourceType;
typedef INT16 SourceBusType;
typedef INT16 EventType;
typedef INT32 EventFilterFlags;
typedef INT32 CommentFieldFilterFlags;
typedef INT16 ScopeType;
typedef INT16 HierarchyType;
typedef INT16 EventCause;
typedef INT16 RangeType;
typedef INT16 ConversionType;
typedef INT16 ConversionResultType;
typedef INT16 AttachmentType;
typedef INT16 ZipType;
typedef INT32 ZipTypeAndLevel;
typedef INT16 BusEventType;
typedef INT16 TimeStampType;
typedef INT16 ChannelFlags;
typedef INT32 ChannelFilterFlags;
typedef INT32 ChannelSubSetFilterFlags;
typedef INT32 AttachmentFlags;
typedef INT32 DeleteAttachmentFlags;
typedef INT16 TimeFlags;
typedef INT16 OpenFileMode;
typedef INT16 DataReadMode;
typedef INT16 DataWriteMode;
typedef INT16 TimeCheckMode;
typedef INT16 FlushCheckMode;
typedef INT16 SeekTimeMode;
typedef INT16 SeekPosMode;
typedef INT16 ValueAccessMode;
typedef INT16 LicenseLevel;
typedef INT16 LogSinkLevel;
typedef INT32 SortFileOptionFlags;
typedef INT32 CreateFileOptionFlags;
typedef INT32 FinalizeFileOptionFlags;
typedef INT32 ValidateFileOptionFlags;
typedef INT32 RewriteFileOptionFlags;
typedef INT32 DataPointerOptionsFlags;
typedef INT32 CanReadChannelValueFlags;
typedef INT16 LimitType;
typedef INT16 SyntaxVersion;
typedef INT16 FieldDataType;
typedef INT16 StandardFieldType;
typedef INT16 CommentNodeType;
typedef INT16 CommentFieldType;
typedef INT16 CommentParentType;
typedef INT16 CommentMergeFlags;
typedef INT16 TimeSrcClass;
typedef INT32 GetFirstAndLastTimeStampOptionsFlags;
typedef INT32 ErrorCode;

#endif

