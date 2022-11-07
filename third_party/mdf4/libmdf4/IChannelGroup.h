/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IChannelGroup
-------------------------------------------------------------------------------
MDF4Lib public unmanaged C++ interface
See API documentation for details
-------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
-----------------------------------------------------------------------------*/
#pragma once

#include "MDF4LibDataTypes.h"

///////////////////////////////////////////////////
//! \file
//! \brief Definition of IChannelGroup

///////////////////////////////////////////////////
//! \brief Description of a channel group
//!
//! The IChannelGroup object describes a channel group (CG block) and also holds some information of the data group (DG block).
//! A channel group contains a collection of channels which are stored in one record, i.e. which have equal sampling.
interface IChannelGroup
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannelGroup* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //!
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<br>
  //! > 0 if the object still is referenced by some other object<br>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Get reference to parent file
  //! \return pointer to IFile object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IFile* MDFLIBCALL GetFile() = 0;

  ///////////////////////////////////////////////////
  //! Get number of samples
  //! \return number of samples, 0 on error
  virtual UINT64 MDFLIBCALL GetSampleCount() = 0;

  ///////////////////////////////////////////////////
  //! Get number of data bytes in record
  //! \note this does not contain the bytes for the record ID nor invalidation bytes
  //! \see GetInvalByteCount
  //! \return number of data bytes, 0 on error
  virtual UINT32 MDFLIBCALL GetDataByteCount() = 0;

  ///////////////////////////////////////////////////
  //! Get number of invalidation bytes in record
  //! \note invalidation bytes are optional and are not available for MDF3
  //! \return number of invalidation bytes, 0 on error or if not available
  virtual UINT32 MDFLIBCALL GetInvalByteCount() = 0;

  ///////////////////////////////////////////////////
  //! Get comment of channel group
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IComment object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IComment* MDFLIBCALL GetComment(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get comment of parent data group
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IComment object, NULL on error or for MDF3
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IComment* MDFLIBCALL GetDataGroupComment(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get acquisition name
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with acquisition name, NULL on error or for MDF3
  virtual LPCTSTR MDFLIBCALL GetAcquisitionName(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get source information of channel group
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to ISourceInfo object, NULL on error or for MDF3
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ISourceInfo* MDFLIBCALL GetSourceInfo(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get master channel of channel group for specified sync type
  //! \param[in] masterSyncType sync type of master channel (usually ::eTimeSync)
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IChannel object, NULL on error or if requested master channel type is not available
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL GetMasterChannel(SyncType masterSyncType = eTimeSync, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get channel set of channel group
  //! \param[in] filterFlags flags for filtering, value is a bit combination of ::ChannelFilterFlagsEnum
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IChannelSet object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannelSet* MDFLIBCALL GetChannelSet(UINT32 filterFlags = 0, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Check if parent data group is sorted, i.e. data block cannot contain records of other channel groups
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return true if sorted, false otherwise or on error
  virtual bool MDFLIBCALL IsSorted(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create a data pointer object to navigate and read signal data
  //! \note the extended method IChannelGroup::CreateDataPointerEx allows to specifiy options for the created IDataPointer
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IDataPointer object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IDataPointer* MDFLIBCALL CreateDataPointer(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Check if channel group contains a bus event (bus logging for MDF 4.1)
  //! \note for MDF version >= 4.1 only a flag is checked, for earlier versions this is not supported
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return true if the channel group contains a bus event, false otherwise or on error
  virtual bool MDFLIBCALL ContainsBusEvent(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get the type of bus event contained in the channel group
  //! \note for MDF version >= 4.1 only flags are checked, for earlier versions this is not supported yet.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return type of the contained bus event (see ::BusEventTypeEnum), or ::eBusEventUnspec in case of an error or if not supported.
  virtual BusEventType MDFLIBCALL GetBusEventType(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set comment of channel group
  //! \note only available for newly created or writable file
  //! \param[in] pComment pointer to IComment object, NULL to remove reference to a comment object.
  //!            <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (cg_comment.XSD).
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetComment(IComment* pComment) = 0;

  ///////////////////////////////////////////////////
  //! Set comment of parent data group
  //! \note only available for newly created or writable file, not available for MDF3
  //! \param[in] pComment pointer to IComment object, NULL to remove reference to a comment object.
  //!            <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (dg_comment.XSD).
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetDataGroupComment(IComment* pComment) = 0;

  ///////////////////////////////////////////////////
  //! Set (acquisition) source information of channel group
  //! \note only available for newly created or writable file, not available for MDF3
  //! \param[in] pSourceInfo pointer to ISourceInfo object, NULL to remove reference to a source info object.
  //!            <br>Note: object must be in same file.
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetSourceInfo(ISourceInfo* pSourceInfo) = 0;

  ///////////////////////////////////////////////////
  //! Create a master channel in this channel group
  //!
  //! The given channel will be appended to the channel list of this channel group.
  //! \note There can be at most one master channel (real or virtual) per sync type per channel group.
  //! Adding a second master channel with the same sync type will fail with error code ::eNoUniqueMasterChannel.
  //! For the created master channel automatically a IUnit will be added with name "s", "m" or "rad", depending on the sync type.
  //!
  //! Please pay attention that the added channel can be identified uniquely within the channel group, otherwise the method might fail with ::eNoUniqueIdentification.
  //! For MDF4 the combination of channel name (cn), source name (cs) and source path (cp) (see GetSourceInfo) must be unique.
  //!
  //! Adding a channel, the range of data Bytes and possibly invalidation Bytes (MDF4) will be extended considering the range of Bytes used by the added channel.
  //! Hence, adding a channel that exceeds the current record length of the channel group is only possible as long as the channel group has no records (GetSampleCount() must be 0) and
  //! no IDataWriter for the channel group is open.
  //! \note only available for newly created or writable file.
  //! \param[in]  name name for the master channel.
  //!             <br>If NULL, depending on the sync type automatically the name "time", "angle" or "distance" will be set.
  //! \param[in]  byteOffset byte offset of the channel value in the record data bytes.
  //! \param[in]  conversionFactor factor for a linear conversion (offset 0) of the master channel. Must be >= 0.
  //!             <br>If conversionFactor is different to 1, the method automatically creates or assigns a respective IConversion.
  //! \param[in]  bitCount number of bits used for the channel value in the record data bytes.
  //!             <br>if 0, a virtual master channel will be created.
  //! \param[in]  dataType data type of the channel, see ::ChannelDataTypeEnum. For this channel type, only numeric data types are allowed.
  //!             <br>If number of bits does not match the data type, the method will fail with error code ::eInvalidDataTypeSize.
  //! \param[in]  syncType synchronization type of the master channel, see ::SyncTypeEnum. Only values ::eTimeSync, ::eAngleSync, and ::eDistanceSync are allowed.
  //! \param[in]  pComment pointer to IComment object to set for the channel.
  //!             <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (cn_comment.XSD).
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     on success the IChannel interface of the created master channel, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL CreateMasterChannel(LPCTSTR name = NULL,
                                                   UINT32 byteOffset = 0,
                                                   REAL conversionFactor = 1.0,
                                                   UINT32 bitCount = 64,
                                                   ChannelDataType dataType = eReal_LE,
                                                   SyncType syncType = eTimeSync,
                                                   IComment* pComment = NULL,
                                                   ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create a master channel in this channel group
  //!
  //! The given channel will be appended to the channel list of this channel group.
  //! \note There can be at most one master channel (real or virtual) per sync type per channel group.
  //! Adding a second master channel with the same sync type will fail with error code ::eNoUniqueMasterChannel.
  //! For the created master channel automatically a IUnit will be added with name "s", "m" or "rad", depending on the sync type.
  //!
  //! Please pay attention that the added channel can be identified uniquely within the channel group, otherwise the method might fail with ::eNoUniqueIdentification.
  //! For MDF4 the combination of channel name (cn), source name (cs) and source path (cp) (see GetSourceInfo) must be unique.
  //!
  //! Adding a channel, the range of data Bytes and possibly invalidation Bytes (MDF4) will be extended considering the range of Bytes used by the added channel.
  //! Hence, adding a channel that exceeds the current record length of the channel group is only possible as long as the channel group has no records (GetSampleCount() must be 0) and
  //! no IDataWriter for the channel group is open.
  //! \note only available for newly created or writable file.
  //! \param[in]  name name for the master channel.
  //!             <br>If NULL, depending on the sync type automatically the name "time", "angle" or "distance" will be set.
  //! \param[in]  byteOffset byte offset of the channel value in the record data bytes.
  //! \param[in]  bitCount number of bits used for the channel value in the record data bytes.
  //!             <br>if 0, a virtual master channel will be created.
  //! \param[in]  dataType data type of the channel, see ::ChannelDataTypeEnum. For this channel type, only numeric data types are allowed.
  //!             <br>If number of bits does not match the data type, the method will fail with error code ::eInvalidDataTypeSize.
  //! \param[in]  syncType synchronization type of the master channel, see ::SyncTypeEnum. Only values ::eTimeSync, ::eAngleSync, and ::eDistanceSync are allowed.
  //! \param[in]  pComment pointer to IComment object to set for the channel.
  //!             <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (cn_comment.XSD).
  //! \param[in]  pUnit pointer to IUnit object to set for the channel.
  //!             <br>Note: object must be in same file and if unit is XML for MDF4, it should match the respective XML schema (cn_unit.XSD).
  //! \param[in]  pConversion pointer to IConversion object to set for the channel.
  //!             <br>Note: object must be in same file and the conversion type must match the channel data type.
  //! \param[in]  pChannelSource pointer to ISourceInfo object to set for the channel.
  //!             <br>Note: object must be in same file.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     on success the IChannel interface of the created master channel, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL CreateMasterChannelEx(LPCTSTR name,
                                                   UINT32 byteOffset,
                                                   UINT32 bitCount,
                                                   ChannelDataType dataType,
                                                   SyncType syncType = eTimeSync,
                                                   IComment* pComment = NULL,
                                                   IUnit* pUnit = NULL,
                                                   IConversion* pConversion = NULL,
                                                   ISourceInfo* pChannelSource = NULL,
                                                   ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create a fixed length data channel in this channel group
  //!
  //! The given channel will be appended to the channel list of this channel group.
  //!
  //! Please pay attention that the added channel can be identified uniquely within the channel group, otherwise the method might fail with ::eNoUniqueIdentification.
  //! For MDF4 the combination of channel name (cn), source name (cs) and source path (cp) (see GetSourceInfo) must be unique.
  //!
  //! Adding a channel, the range of data Bytes and possibly invalidation Bytes (MDF4) will be extended considering the range of Bytes used by the added channel.
  //! Hence, adding a channel that exceeds the current record length of the channel group is only possible as long as the channel group has no records (GetSampleCount() must be 0) and
  //! no IDataWriter for the channel group is open.
  //! \note only available for newly created or writable file.
  //! \param[in]  name name for the channel.
  //! \param[in]  byteOffset byte offset of the channel value in the record data bytes.
  //! \param[in]  bitCount number of bits used for the channel value in the record data bytes.
  //              <br>if 0, a virtual data channel will be created (MDF 4.1)
  //! \param[in]  dataType data type of the channel, see ::ChannelDataTypeEnum.
  //!             <br>If number of bits does not match the data type, the method will fail with error code ::eInvalidDataTypeSize.
  //! \param[in]  pComment pointer to IComment object to set for the channel.
  //!             <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (cn_comment.XSD).
  //! \param[in]  pUnit pointer to IUnit object to set for the channel.
  //!             <br>Note: object must be in same file and if unit is XML for MDF4, it should match the respective XML schema (cn_unit.XSD).
  //! \param[in]  pConversion pointer to IConversion object to set for the channel.
  //!             <br>Note: object must be in same file and the conversion type must match the channel data type.
  //! \param[in]  pChannelSource pointer to ISourceInfo object to set for the channel.
  //!             <br>Note: object must be in same file.
  //! \param[in]  flags bit flags to set for the channel, see ::ChannelFlagsEnum.
  //!             <br>Note: flags ::eFlagPrecisionValid, ::eFlagValueRangeValid, ::eFlagLimitRangeValid, ::eFlagLimitExtRangeValid and ::eFlagDefaultX will be ignored (reset)
  //!             use the respective Set methods instead to set the information.
  //! \param[in]  invalBitPos position of invalidation bit. Will only be considered for MDF4 and if ::eFlagInvalBitUsed flag is set in flags.
  //! \param[in]  bitOffset bit offset of the channel value in the data bytes of the parent channel. 0-7, must be 0 for all data types >= eReal_LE.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     on success the IChannel interface of the created master channel, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL CreateDataChannel(LPCTSTR name,
                                                 UINT32 byteOffset,
                                                 UINT32 bitCount = 64,
                                                 ChannelDataType dataType = eReal_LE,
                                                 IComment* pComment = NULL,
                                                 IUnit* pUnit = NULL,
                                                 IConversion* pConversion = NULL,
                                                 ISourceInfo* pChannelSource = NULL,
                                                 UINT32 flags = 0,
                                                 UINT32 invalBitPos = 0,
                                                 UINT8 bitOffset = 0,
                                                 ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create a variable length data channel in this channel group
  //!
  //! The given channel will be appended to the channel list of this channel group.
  //!
  //! Please pay attention that the added channel can be identified uniquely within the channel group, otherwise the method might fail with ::eNoUniqueIdentification.
  //! For MDF4 the combination of channel name (cn), source name (cs) and source path (cp) (see GetSourceInfo) must be unique.
  //!
  //! Adding a channel, the range of data Bytes and possibly invalidation Bytes (MDF4) will be extended considering the range of Bytes used by the added channel (here 8 Bytes always).
  //! Hence, adding a channel that exceeds the current record length of the channel group is only possible as long as the channel group has no records (GetSampleCount() must be 0) and
  //! no IDataWriter for the channel group is open.
  //! \note only available for newly created or writable file, not available for MDF3
  //! \param[in]  name name for the channel.
  //! \param[in]  byteOffset byte offset of the channel value in the record data bytes.
  //! \param[in]  vlsdDataType data type of the channel, see ::ChannelDataTypeEnum.
  //!             <br>For VLSD channels, the data type specifies the type of the VLSD values. Typically only string or complex data types are used for VLSD channels.
  //! \param[in]  pComment pointer to IComment object to set for the channel.
  //!             <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (cn_comment.XSD).
  //! \param[in]  pUnit pointer to IUnit object to set for the channel.
  //!             <br>Note: object must be in same file and if unit is XML for MDF4, it should match the respective XML schema (cn_unit.XSD).
  //! \param[in]  pConversion pointer to IConversion object to set for the channel.
  //!             <br>Note: object must be in same file and the conversion type must match the channel data type.
  //! \param[in]  pChannelSource pointer to ISourceInfo object to set for the channel.
  //!             <br>Note: object must be in same file.
  //! \param[in]  flags bit flags to set for the channel, see ::ChannelFlagsEnum.
  //!             <br>Note: flags ::eFlagPrecisionValid, ::eFlagValueRangeValid, ::eFlagLimitRangeValid, ::eFlagLimitExtRangeValid and ::eFlagDefaultX will be ignored (reset)
  //!             use the respective Set methods instead to set the information.
  //! \param[in]  invalBitPos position of invalidation bit. Will only be considered for MDF4 and if ::eFlagInvalBitUsed flag is set in flags.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     on success the IChannel interface of the created channel, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL CreateVLSDChannel(LPCTSTR name,
                                                 UINT32 byteOffset,
                                                 ChannelDataType vlsdDataType = eByteArray,
                                                 IComment* pComment = NULL,
                                                 IUnit* pUnit = NULL,
                                                 IConversion* pConversion = NULL,
                                                 ISourceInfo* pChannelSource = NULL,
                                                 UINT32 flags = 0,
                                                 UINT32 invalBitPos = 0,
                                                 ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create a maximum length data channel in this channel group
  //!
  //! The given channel will be appended to the channel list of this channel group.
  //!
  //! Please pay attention that the added channel can be identified uniquely within the channel group, otherwise the method might fail with ::eNoUniqueIdentification.
  //! For MDF4 the combination of channel name (cn), source name (cs) and source path (cp) (see GetSourceInfo) must be unique.
  //!
  //! Adding a channel, the range of data Bytes and possibly invalidation Bytes (MDF4) will be extended considering the range of Bytes used by the added channel.
  //! Hence, adding a channel that exceeds the current record length of the channel group is only possible as long as the channel group has no records (GetSampleCount() must be 0) and
  //! no IDataWriter for the channel group is open.
  //! \note only available for newly created or writable file, not available for MDF3
  //! \param[in]  name name for the channel.
  //! \param[in]  byteOffset byte offset of the channel value in the record data bytes.
  //! \param[in]  pSizeChannel pointer to IChannel object for the size signal of the MLSD channel.
  //!             <br>Note: object must be in same file and channel group and should return as physical values only integer values <= maxByteCount.
  //! \param[in]  maxByteCount maximum number of Bytes for MLSD channel value, i.e. number of Bytes used for the channel value in the record data bytes.
  //! \param[in]  dataType data type of the channel, see ::ChannelDataTypeEnum.
  //!             <br>Typically only string or complex data types are used for MLSD channels.
  //!             If number of bits does not match the data type, the method will fail with error code ::eInvalidDataTypeSize.
  //! \param[in]  pComment pointer to IComment object to set for the channel.
  //!             <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (cn_comment.XSD).
  //! \param[in]  pUnit pointer to IUnit object to set for the channel.
  //!             <br>Note: object must be in same file and if unit is XML for MDF4, it should match the respective XML schema (cn_unit.XSD).
  //! \param[in]  pConversion pointer to IConversion object to set for the channel.
  //!             <br>Note: object must be in same file and the conversion type must match the channel data type.
  //! \param[in]  pChannelSource pointer to ISourceInfo object to set for the channel.
  //!             <br>Note: object must be in same file.
  //! \param[in]  flags bit flags to set for the channel, see ::ChannelFlagsEnum.
  //!             <br>Note: flags ::eFlagPrecisionValid, ::eFlagValueRangeValid, ::eFlagLimitRangeValid, ::eFlagLimitExtRangeValid and ::eFlagDefaultX will be ignored (reset)
  //!             use the respective Set methods instead to set the information.
  //! \param[in]  invalBitPos position of invalidation bit. Will only be considered for MDF4 and if ::eFlagInvalBitUsed flag is set in flags.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     on success the IChannel interface of the created channel, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL CreateMLSDChannel(LPCTSTR name,
                                                 UINT32 byteOffset,
                                                 IChannel* pSizeChannel,
                                                 UINT32 maxByteCount,
                                                 ChannelDataType dataType = eByteArray,
                                                 IComment* pComment = NULL,
                                                 IUnit* pUnit = NULL,
                                                 IConversion* pConversion = NULL,
                                                 ISourceInfo* pChannelSource = NULL,
                                                 UINT32 flags = 0,
                                                 UINT32 invalBitPos = 0,
                                                 ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create a stream synchronization channel in this channel group
  //!
  //! The given channel will be appended to the channel list of this channel group.
  //!
  //! Please pay attention that the added channel can be identified uniquely within the channel group, otherwise the method might fail with ::eNoUniqueIdentification.
  //! For MDF4 the combination of channel name (cn), source name (cs) and source path (cp) (see GetSourceInfo) must be unique.
  //!
  //! Adding a channel, the range of data Bytes and possibly invalidation Bytes (MDF4) will be extended considering the range of Bytes used by the added channel.
  //! Hence, adding a channel that exceeds the current record length of the channel group is only possible as long as the channel group has no records (GetSampleCount() must be 0) and
  //! no IDataWriter for the channel group is open.
  //! \note only available for newly created or writable file.
  //! \param[in]  name name for the channel.
  //! \param[in]  byteOffset byte offset of the channel value in the record data bytes.
  //! \param[in]  pStreamAttachment pointer to IAttachment object with stream to be synchronized (must not be NULL).
  //!             <br>Note: attachment object must be in same file and in global list of attachments.
  //! \param[in]  syncType type of synchronization with the stream in attachment, see ::SyncTypeEnum. Only values ::eTimeSync, ::eAngleSync, ::eDistanceSync, and ::eIndexSync are allowed.
  //!             <br>Note:  Physical values of this channel must return the unit of the respective synchronization domain,
  //!            i.e. seconds for syncType = ::eTimeSync, or an index value for syncType = ::eIndexSync.
  //!            These values are used for synchronization with the stream.
  //! \param[in]  bitCount number of bits used for the channel value in the record data bytes.
  //! \param[in]  dataType data type of the channel, see ::ChannelDataTypeEnum. For this channel type, only numeric data types are allowed.
  //!             <br>If number of bits does not match the data type, the method will fail with error code ::eInvalidDataTypeSize.
  //! \param[in]  pComment pointer to IComment object to set for the channel.
  //!             <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (cn_comment.XSD).
  //! \param[in]  pUnit pointer to IUnit object to set for the channel.
  //!             <br>Note: object must be in same file and if unit is XML for MDF4, it should match the respective XML schema (cn_unit.XSD).
  //! \param[in]  pConversion pointer to IConversion object to set for the channel.
  //!             <br>Note: object must be in same file and the conversion type must match the channel data type.
  //! \param[in]  pChannelSource pointer to ISourceInfo object to set for the channel.
  //!             <br>Note: object must be in same file.
  //! \param[in]  flags bit flags to set for the channel, see ::ChannelFlagsEnum.
  //!             <br>Note: flags ::eFlagPrecisionValid, ::eFlagValueRangeValid, ::eFlagLimitRangeValid, ::eFlagLimitExtRangeValid and ::eFlagDefaultX will be ignored (reset)
  //!             use the respective Set methods instead to set the information.
  //! \param[in]  invalBitPos position of invalidation bit. Will only be considered for MDF4 and if ::eFlagInvalBitUsed flag is set in flags.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     on success the IChannel interface of the created master channel, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL CreateStreamSyncChannel(LPCTSTR name,
                                                       UINT32 byteOffset,
                                                       IAttachment* pStreamAttachment,
                                                       SyncType syncType = eIndexSync,
                                                       UINT32 bitCount = 32,
                                                       ChannelDataType dataType = eInteger_Unsigned_LE,
                                                       IComment* pComment = NULL,
                                                       IUnit* pUnit = NULL,
                                                       IConversion* pConversion = NULL,
                                                       ISourceInfo* pChannelSource = NULL,
                                                       UINT32 flags = 0,
                                                       UINT32 invalBitPos = 0,
                                                       ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create a data writer object to write signal values for this channel group
  //!
  //! \note for MDF3, any other operation that creates a new object and writes it to file (e.g. IFacotry::CreateComment or IFile::CreateChannelGroup) is only allowed after the IDataWriter has been released!
  //! Only one IDataWriter can be open per file, i.e. creating a new IDataWriter will fail as long as another one is open and has not been released yet.
  //! It is also not allowed to create an IDataWriter as long as there is an open data pointer (IDataPointer/IGroupDataPointer) using this channel group.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation. Default: NULL
  //! \param[in]  recordCountForWriteBuffer number of records to store in internal write buffer before flushing to file (MDF4 only, will be ignored for MDF3).
  //!             <br>If 0, the number of records to buffer will be determined internally (default)
  //!             <br>If 1, each single record will be written to file immediately (bad performance, should only be used for large or VLSD records).
  //! \param[in]  zipTypeAndLevel type and level of compression algorithm to use, see ::ZipTypeAndLevelEnum. Must be ::eNoCompression for MDF < 4.1.
  //! \return     on success the IDataWriter interface of the created data writer, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IDataWriter* MDFLIBCALL CreateDataWriter(ErrorCode* pErrorCode = NULL, UINT32 recordCountForWriteBuffer = 0, ZipTypeAndLevel zipTypeAndLevel = eNoCompression) = 0;

  ///////////////////////////////////////////////////
  //! Get the path separator specified for this channel group as character
  //! \note optional feature available for MDF >= 4.1
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     on success the character to be used as path separator, 0 on error or if not available/not defined
  virtual TCHAR MDFLIBCALL GetPathSeparatorChar(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set the path separator for this channel group
  //! \note optional feature, only available for newly created or writable file, not available for MDF < 4.1.
  //! \param[in]  pathSeparatorChar character to be used as path separator, 0 if path separator is not defined
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetPathSeparatorChar(TCHAR pathSeparatorChar) = 0;

  ///////////////////////////////////////////////////
  //! Set the acquisition name for this channel group
  //! \note only available for newly created or writable file, not available for MDF3
  //! \param[in] acquisitionName string to be used as acquisition name, 0 or empty to delete the name
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetAcquisitionName(LPCTSTR acquisitionName) = 0;

  ///////////////////////////////////////////////////
  //! Change the type of bus event contained in the channel group
  //! Usually the bus event type is set automatically when creating the respective channels in the channel group.
  //! For rare cases, however, it may be necessary to manually change the bus event type of the channel group.
  //! \note only available for newly created or writable file, not available for MDF < 4.1.
  //! \param[in] busEventType of the contained bus event (see ::BusEventTypeEnum).
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetBusEventType(BusEventType busEventType) = 0;

  ///////////////////////////////////////////////////
  //! Create a data pointer object to navigate and read signal data
  //! \param[in]  options bit combination of option flags, see ::DataPointerOptionsFlagsEnum
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IDataPointer object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IDataPointer* MDFLIBCALL CreateDataPointerEx(UINT32 options, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get event set for events for this channel group
  //! \note The resulting IEventSet object offers further filtering of the events.
  //! \param[in]  filterFlags flags for filtering, value is a bit combination of ::EventFilterFlagsEnum
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IEventSet object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IEventSet* MDFLIBCALL GetEventSet(UINT32 filterFlags = 0, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get start addresses of CG block
  //! \return returns start address of CG block for this channel group, -1 on error or for virtual channel groups.
  virtual INT64 MDFLIBCALL GetBlockStartAddress() = 0;

  ///////////////////////////////////////////////////
  //! Get start addresses of parent DG block
  //! \return returns start address of parent DG block for this channel group, -1 on error or for virtual channel groups.
  virtual INT64 MDFLIBCALL GetParentDataGroupStartAddress() = 0;

};

