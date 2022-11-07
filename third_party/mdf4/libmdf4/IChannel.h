/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IChannel
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
//! \brief Definition of IChannel

///////////////////////////////////////////////////
//! \brief Description of a channel
//!
//! The IChannel object describes a channel, i.e. it gives information about the recorded signal (name, unit, data type, ...)
//! and how its signal values are stored in the MDF file.
//! <br><br>
//! A channel can have child channels if it is a composition, i.e. a structure with members or an array with elements.
//! \note For an array channel (CompositionType ::eIsArray), some of its child channels may <b>NOT</b> be part of
//! the parent channel group! This may happen for MDF3 in case of a distributed CD block (old way to store 2D maps)
//! or for MDF4 in case of a CA block with CG/DG template (not supported yet!).
//! <br>IChannel::GetChannelGroup always returns the actual parent channel group.
//! <br>You can use IChannel::IsEncodedInChannelGroup to check if the channel is really part of a given channel group.
interface IChannel
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //!
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  /////////////////////////////////////////////////////
  //! Get name of channel
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with channel name, NULL on error
  virtual LPCTSTR MDFLIBCALL GetName(ErrorCode* pErrorCode = NULL) = 0;

  /////////////////////////////////////////////////////
  //! Get message name for channel
  //! \note the message name is stored in CE block for MDF3 and in SI block of parent CG for MDF4
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \param[out] pID if not NULL the ID of the message will be returned, -1 if no available
  //! \param[out] pDLC if not NULL the DLC of the message will be returned, -1 if no available
  //! \return pointer to string with message name, NULL on error or if not available
  virtual LPCTSTR MDFLIBCALL GetMessageName(ErrorCode* pErrorCode = NULL, INT64* pID = NULL, INT64* pDLC = NULL) = 0;

  /////////////////////////////////////////////////////
  //! Get prefix for extended name
  //! \note in case of a channel from a bus message, this is the message name, otherwise the device name
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with result, NULL on error or if not available
  virtual LPCTSTR MDFLIBCALL GetExtendedNamePrefix(ErrorCode* pErrorCode = NULL) = 0;

  /////////////////////////////////////////////////////
  //! Get display name of channel
  //! \note the display name is optional. If for MDF4 more than one display name is specified, the first will be returned.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with display name, NULL on error or if not available
  virtual LPCTSTR MDFLIBCALL GetDisplayName(ErrorCode* pErrorCode = NULL) = 0;

  /////////////////////////////////////////////////////
  //! Get description text for channel
  //! \note in MDF4 the description is an optional XML tag. If for MDF4 more than one description is specified, the first will be returned.
  //! If no description is specified in MDF4, the first line of the plain comment text will be returned.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with description text, NULL on error or if not available
  virtual LPCTSTR MDFLIBCALL GetDescription(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get comment of channel
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IComment object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IComment* MDFLIBCALL GetComment(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get source information of channel
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to ISourceInfo object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ISourceInfo* MDFLIBCALL GetSourceInfo(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get type of the channel
  //! \return channel type, see ::ChannelTypeEnum, ::eChannelTypeUnspec on error
  virtual ChannelType MDFLIBCALL GetType() = 0;

  ///////////////////////////////////////////////////
  //! Get synchronization type of the channel
  //! \return sync type, see ::SyncTypeEnum, ::eSyncTypeUnspec on error
  virtual SyncType MDFLIBCALL GetSyncType() = 0;

  ///////////////////////////////////////////////////
  //! Get data type of the channel
  //! \return data type, see ::ChannelDataTypeEnum, ::eChannelDataTypeUnspec on error
  virtual ChannelDataType MDFLIBCALL GetDataType() = 0;

  ///////////////////////////////////////////////////
  //! Get composition type of channel
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return composition type, see ::CompositionTypeEnum, ::eCompositionTypeUnspec on error
  virtual CompositionType MDFLIBCALL GetCompositionType(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get component type of channel
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return component type, see ::ComponentTypeEnum, ::eComponentTypeUnspec on error
  virtual ComponentType MDFLIBCALL GetComponentType(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Request if data type is numeric
  //! \return true if data type is numeric (some floating-point or Integer data type)
  virtual bool MDFLIBCALL HasNumericDataType() = 0;

  ///////////////////////////////////////////////////
  //! Request if data type is Integer
  //! \return true if data type is Integer, false otherwise or on error
  virtual bool MDFLIBCALL HasIntegerDataType() = 0;

  ///////////////////////////////////////////////////
  //! Get number of bits for signal value in record
  //! \return bit count, 0 on error or for virtual master channels
  virtual UINT32 MDFLIBCALL GetBitCount() = 0;

  ///////////////////////////////////////////////////
  //! Get offset to first Byte in the data record that contains bits of the signal value.
  //! \note The offset is applied to the plain record data, i.e. skipping the record ID.
  //! \return byte offset, 0 on error (check error code!)
  virtual UINT32 MDFLIBCALL GetByteOffset() = 0;

  ///////////////////////////////////////////////////
  //! Get bit offset for signal value in record
  //! \note The bit offset specifies the first bit (=LSB) of the signal value after Byte offset has been applied
  //! (0 <= bit offset <= 7)
  //! \return bit offset, 0 on error (check error code!)
  virtual UINT8 MDFLIBCALL GetBitOffset() = 0;

  /////////////////////////////////////////////////////
  //! Get position of invalidation bit
  //! \note optional for MDF4, not available for MDF3
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return bit position of invalidation bit, 0 on error or if not available (check error code!)
  virtual UINT32 MDFLIBCALL GetInvalBitPos(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get conversion rule of channel
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL GetConversion(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get unit of channel
  //! \note for MDF4 a unit can be specified directly for the channel. This overrules a unit provided via conversion rule.
  //!       For MDF3, a unit only can be specified via conversion rule.
  //!       Thus, for MDF3 parameter channelOnly = false can be used to get the unit from the conversion rule instead.
  //! \note for MDF4 channel data types ::eMimeSample and ::eMimeStream, the IUnit of the channel contains the MIME type text (given by IUnit::GetName)
  //! \param[in]  channelOnly if false and if the channel has no unit, the unit of the conversion rule will be returned as fall-back.<br>
  //!        Otherwise only the unit for channel will be considered.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IUnit object, NULL on error or if not available
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IUnit* MDFLIBCALL GetUnit(bool channelOnly = true, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Request the precision to use for display of floating point values
  //! \note for MDF4 the precision can be specified directly for the channel. This overrules a precision provided via conversion rule.
  //!       For MDF3, the precision is stored in the channel comment (Vector proprietary extension).
  //! \note the precision information is optional
  //! \param[in]  channelOnly if false and if the channel has no precision information, the precision information of the conversion rule will be returned as fall-back.<br>
  //!        Otherwise only the precision information of the channel will be considered.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return number of decimal places to use for display of floating point values or 255 (0xFF) in case of error or unrestricted precision
  virtual UINT8 MDFLIBCALL GetPrecision(bool channelOnly = true, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Read value range of channel
  //!
  //! The minimum and maximum value that occurred for a signal can be stored, e.g.
  //! for quick determination of the display range or statistics.
  //! For arrays, the minimum and maximum value is calculated over all elements.
  //! \note the range may not be valid
  //! \param[out] pMin if not NULL the minimum raw value of the range will be assigned
  //! \param[out] pMax if not NULL the maximum raw value of the range will be assigned
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL ReadValueRange(REAL* pMin = NULL, REAL* pMax = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Read limit range of channel
  //! \note physical value for numeric conversion rule, otherwise raw value.
  //! \note the limit range may not be valid
  //! \param[out] pMin if not NULL the minimum limit value of the limit range will be assigned
  //! \param[out] pMax if not NULL the maximum limit value of the limit range will be assigned
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL ReadLimitRange(REAL* pMin = NULL, REAL* pMax = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Read extended limit range of channel
  //! \note physical value for numeric conversion rule, otherwise raw value.
  //! \note the extended limit range may not be valid
  //! \param[out] pMin if not NULL the minimum value of the extended limit range will be assigned
  //! \param[out] pMax if not NULL the maximum value of the extended limit range will be assigned
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL ReadExtLimitRange(REAL* pMin = NULL, REAL* pMax = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Request if channel has discrete values and should not be interpolated
  //! \note this method not only considers the flag state for ::eFlagDiscrete,
  //! but also bit signals (GetBitCount() = 1) and non-numeric data types (HasNumericDataType() = false).
  //! \return true if channel has discrete values, false otherwise
  virtual bool MDFLIBCALL IsDiscrete() = 0;

  ///////////////////////////////////////////////////
  //! Request a flag state
  //! \note only available for MDF4
  //! \param[in]  flag see ::ChannelFlagsEnum for flag values<br>
  //! may even be a bit combination, e.g. GetFlagState(::eFlagCalibration | ::eFlagCalculated)
  //! \return true if flag is set (high), false otherwise
  virtual bool MDFLIBCALL GetFlagState(ChannelFlags flag) = 0;

  /////////////////////////////////////////////////////
  //! Request if the values for this channel are encoded in the record of given channel group
  //! \note for array elements it can happen that a channel is part of the channel set retrieved for a channel group,
  //! although its values are not encoded in the record of this channel group
  //! (e.g. CG/DG template for MDF4 or CD block elements distributed over several CGs for MDF3).
  //! In this case use the IDataPointer returned by IChannel::CreateDataPointer of the channel instead of the one returned by IChannelGroup::CreateDataPointer
  //! \see CreateDataPointer
  //! \param[in]  pChannelGroup the channel group to check
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return true if channel value is in the record of given channel group, false otherwise or on error
  virtual bool MDFLIBCALL IsEncodedInChannelGroup(IChannelGroup* pChannelGroup, ErrorCode* pErrorCode = NULL) = 0;

  /////////////////////////////////////////////////////
  //! Get reference to parent channel group
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IChannelGroup object, NULL on error or if not available
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannelGroup* MDFLIBCALL GetChannelGroup(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get child channel set of channel
  //! \param[in]  filterFlags flags for filtering, value is a bit combination of ::ChannelFilterFlagsEnum
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IChannelSet object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannelSet* MDFLIBCALL GetChildChannelSet(UINT32 filterFlags = 0, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get number of attachments related to this channel
  //! \note there can be attachments for channels for MDF version >= 4.1.
  //! \see GetAttachment
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return number of attachments (0 on error or if not available, e.g. for MDF3)
  virtual UINT64 MDFLIBCALL GetAttachmentCount(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get attachment related to this channel for given index
  //! \note attachments for channels are only available for MDF version >= 4.1.
  //! For retrieving the attachment of a synchronization channel use GetSyncStreamAttachment.
  //! \see GetAttachmentCount
  //! \param[in]  index index of attachment.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IAttachment object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IAttachment* MDFLIBCALL GetAttachment(UINT64 index, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get attachment for stream data related to this channel
  //! \note only available for MDF4 and channel type ::eSynchronization
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IAttachment object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IAttachment* MDFLIBCALL GetSyncStreamAttachment(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create a data pointer object to navigate and read signal data for this channel
  //! \note the extended method IChannel::CreateDataPointerEx allows to specifiy options for the created IDataPointer
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IDataPointer object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IDataPointer* MDFLIBCALL CreateDataPointer(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create a fixed length data channel as structure child member of this channel
  //!
  //! A sub channel can only be created for a parent channel with fixed length data type ::eByteArray
  //! that is not an array (composition type != ::eIsArray) or an array member (component type != ::eIsArrayElement).
  //! In case, the composition type of the parent channel will be changed to ::eIsStruct.
  //! Note that the structure child member will automatically have the same source information as the parent channel.
  //! \note only available for newly created or writable file.
  //! \param[in]  name name for the sub channel.
  //! \param[in]  relativeByteOffset byte offset of the channel value in the data bytes of the parent channel.
  //!             <br>Note: the byte offset within the record of the channel group will be calculated internally!
  //! \param[in]  bitCount number of bits used for the channel value in the record data bytes.
  //! \param[in]  dataType data type of the channel, see ::ChannelDataTypeEnum.
  //!             <br>If number of bits does not match the data type, the method will fail with error code ::eInvalidDataTypeSize.
  //! \param[in]  pComment pointer to IComment object to set for the channel.
  //!             <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (cn_comment.XSD).
  //! \param[in]  pUnit pointer to IUnit object to set for the channel.
  //!             <br>Note: object must be in same file and if unit is XML for MDF4, it should match the respective XML schema (cn_unit.XSD).
  //! \param[in]  pConversion pointer to IConversion object to set for the channel.
  //!             <br>Note: object must be in same file and the conversion type must match the channel data type.
  //! \param[in]  flags bit flags to set for the channel, see ::ChannelFlagsEnum.
  //!             <br>Note: flags ::eFlagPrecisionValid, ::eFlagValueRangeValid, ::eFlagLimitRangeValid, ::eFlagLimitExtRangeValid, ::eFlagDefaultX and ::eFlagVlsdDataStream will be ignored (reset)
  //!             use the respective Set methods instead to set the information.
  //! \param[in]  invalBitPos position of invalidation bit. Will only be considered for MDF4 and if ::eFlagInvalBitUsed flag is set in flags.
  //! \param[in]  bitOffset bit offset of the channel value in the data bytes of the parent channel. 0-7, must be 0 for all data types >= eReal_LE.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     on success the IChannel interface of the created master channel, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL CreateChildDataChannel(LPCTSTR name,
                                                      UINT32 relativeByteOffset,
                                                      UINT32 bitCount = 64,
                                                      ChannelDataType dataType = eReal_LE,
                                                      IComment* pComment = NULL,
                                                      IUnit* pUnit = NULL,
                                                      IConversion* pConversion = NULL,
                                                      UINT32 flags = 0,
                                                      UINT32 invalBitPos = 0,
                                                      UINT8 bitOffset = 0,
                                                      ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create a maximum length data channel as structure child member of this channel
  //!
  //! A sub channel can only be created for a parent channel with fixed length data type ::eByteArray
  //! that is not an array (composition type != ::eIsArray) or an array member (component type != ::eIsArrayElement).
  //! In case, the composition type of the parent channel will be changed to ::eIsStruct.
  //! Note that the structure child member will automatically have the same source information as the parent channel.
  //! \note only available for newly created or writable file, not available for MDF3
  //! \param[in]  name name for the sub channel.
  //! \param[in]  relativeByteOffset byte offset of the channel value in the data bytes of the parent channel.
  //!             <br>Note: the byte offset within the record of the channel group will be calculated internally!
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
  //! \param[in]  flags bit flags to set for the channel, see ::ChannelFlagsEnum.
  //!             <br>Note: flags ::eFlagPrecisionValid, ::eFlagValueRangeValid, ::eFlagLimitRangeValid, ::eFlagLimitExtRangeValid, ::eFlagDefaultX and ::eFlagVlsdDataStream will be ignored (reset)
  //!             use the respective Set methods instead to set the information.
  //! \param[in]  invalBitPos position of invalidation bit. Will only be considered for MDF4 and if ::eFlagInvalBitUsed flag is set in flags.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     on success the IChannel interface of the created master channel, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL CreateChildMLSDChannel(LPCTSTR name,
                                                      UINT32 relativeByteOffset,
                                                      IChannel* pSizeChannel, UINT32 maxByteCount,
                                                      ChannelDataType dataType = eByteArray,
                                                      IComment* pComment = NULL,
                                                      IUnit* pUnit = NULL,
                                                      IConversion* pConversion = NULL,
                                                      UINT32 flags = 0,
                                                      UINT32 invalBitPos = 0,
                                                      ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create a variable length data channel as structure child member of this channel
  //!
  //! A sub channel can only be created for a parent channel with fixed length data type ::eByteArray
  //! that is not an array (composition type != ::eIsArray) or an array member (component type != ::eIsArrayElement).
  //! In case, the composition type of the parent channel will be changed to ::eIsStruct.
  //! Note that the structure child member will automatically have the same source information as the parent channel.
  //! \note only available for newly created or writable file, not available for MDF3
  //! \param[in]  name name for the channel.
  //! \param[in]  relativeByteOffset byte offset of the channel value in the data bytes of the parent channel.
  //! \param[in]  vlsdDataType data type of the channel, see ::ChannelDataTypeEnum.
  //!             <br>For VLSD channels, the data type specifies the type of the VLSD values. Typically only string or complex data types are used for VLSD channels.
  //! \param[in]  pComment pointer to IComment object to set for the channel.
  //!             <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (cn_comment.XSD).
  //! \param[in]  pUnit pointer to IUnit object to set for the channel.
  //!             <br>Note: object must be in same file and if unit is XML for MDF4, it should match the respective XML schema (cn_unit.XSD).
  //! \param[in]  pConversion pointer to IConversion object to set for the channel.
  //!             <br>Note: object must be in same file and the conversion type must match the channel data type.
  //! \param[in]  flags bit flags to set for the channel, see ::ChannelFlagsEnum.
  //!             <br>Note: flags ::eFlagPrecisionValid, ::eFlagValueRangeValid, ::eFlagLimitRangeValid, ::eFlagLimitExtRangeValid, ::eFlagDefaultX and ::eFlagVlsdDataStream will be ignored (reset)
  //!             use the respective Set methods instead to set the information.
  //! \param[in]  invalBitPos position of invalidation bit. Will only be considered for MDF4 and if ::eFlagInvalBitUsed flag is set in flags.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     on success the IChannel interface of the created master channel, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL CreateChildVLSDChannel(LPCTSTR name,
                                                      UINT32 relativeByteOffset,
                                                      ChannelDataType vlsdDataType = eByteArray,
                                                      IComment* pComment = NULL,
                                                      IUnit* pUnit = NULL,
                                                      IConversion* pConversion = NULL,
                                                      UINT32 flags = 0,
                                                      UINT32 invalBitPos = 0,
                                                      ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set value range of channel
  //!
  //! \note only available for newly created or writable file, not available for array child channels in MDF4 files
  //! Set the minimum and maximum raw value that occurred for the signal.
  //! For arrays, the minimum and maximum value is calculated over all elements.
  //! \param[in] min minimum raw value
  //! \param[in] max maximum raw value
  //! \param[in] isValid false if value range is invalid, true if valid (default)
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetValueRange(REAL min, REAL max, bool isValid = true) = 0;

  ///////////////////////////////////////////////////
  //! Set limit range of channel
  //! \note physical value for numeric conversion rule, otherwise raw value.
  //! \note only available for newly created or writable file, not available for MDF3 or for array child channels
  //! \param[in] min minimum limit value
  //! \param[in] max maximum limit value
  //! \param[in] isValid false if limit range is invalid, true if valid (default)
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetLimitRange(REAL min, REAL max, bool isValid = true) = 0;

  ///////////////////////////////////////////////////
  //! Set extended limit range of channel
  //! \note physical value for numeric conversion rule, otherwise raw value.
  //! \note only available for newly created or writable file, not available for MDF3 or for array child channels
  //! \param[in] min minimum extended limit value
  //! \param[in] max maximum extended limit value
  //! \param[in] isValid false if extended limit range is invalid, true if valid (default)
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetExtLimitRange(REAL min, REAL max, bool isValid = true) = 0;

  ///////////////////////////////////////////////////
  //! Set the precision to use for display of floating point values
  //! \note only available for newly created or writable file, not available for MDF3 or for array child channels
  //! \param[in] precision number of decimal places to use for display of floating point values or 255 (0xFF) in case of unrestricted precision
  //! \param[in] isValid false if precision is invalid, true if valid (default)
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetPrecision(UINT8 precision, bool isValid = true) = 0;

  ///////////////////////////////////////////////////
  //! Set a flag state
  //! \note only available for newly created or writable file, not available for MDF3 or for array child channels
  //! \param[in]  flag flag to be set, see ::ChannelFlagsEnum for flag values.<br>
  //!                  May even be a bit combination, e.g. SetFlagState(::eFlagCalibration | ::eFlagCalculated).<br>
  //!                  Note: the following flags cannot be changed with this method: ::eFlagInvalBitUsed, ::eFlagDefaultX and ::eFlagVlsdDataStream.<br>
  //!                  The following flags cannot bet set, but only be reset with this method: ::eFlagPrecisionValid, ::eFlagValueRangeValid, ::eFlagLimitRangeValid, ::eFlagLimitExtRangeValid.<br>
  //! \param[in]  state true if the flag(s) should be set, false if they should be reset<br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetFlagState(ChannelFlags flag, bool state) = 0;

  ///////////////////////////////////////////////////
  //! Add attachment reference to this channel
  //! \note only available for newly created or writable file, not available for MDF < 4.1 or for array child channels
  //! For setting the attachment of a synchronization channel use SetSyncStreamAttachment.
  //! \param[in] pAttachment pointer to IAttachment object (must not be NULL).
  //!            <br>Note: attachment object must be in same file and in global list of attachments.
  //!            <br>It is not possible to add an attachment a second time for a channel.
  //! \param[out] pIndex if not NULL the zero-based index of the added attachment in the list of channel attachments will be returned.
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL AddAttachment(IAttachment* pAttachment, UINT64* pIndex = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set attachment reference for stream data related to this channel
  //! \note only available for MDF4 and channel type ::eSynchronization in newly created or writable file, not available for array child channels
  //! \param[in] pAttachment pointer to IAttachment object (must not be NULL).
  //!            <br>Note: attachment object must be in same file and in global list of attachments.
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetSyncStreamAttachment(IAttachment* pAttachment) = 0;

  ///////////////////////////////////////////////////
  //! Set comment of channel
  //! \note only available for newly created or writable file, not available for array elements in MDF4 file
  //! \param[in] pComment pointer to IComment object, NULL to remove reference to a comment object.
  //!            <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (cn_comment.XSD).
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetComment(IComment* pComment) = 0;

  ///////////////////////////////////////////////////
  //! Set source information of channel
  //! \note only available for newly created or writable file, not available for structure or array child channels
  //! \param[in] pSourceInfo pointer to ISourceInfo object, NULL to remove reference to a source info object.
  //!            <br>Note: object must be in same file.
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetSourceInfo(ISourceInfo* pSourceInfo) = 0;

  ///////////////////////////////////////////////////
  //! Set conversion rule of channel
  //! \note only available for newly created or writable file, not available for array child channels in MDF4 file
  //! \param[in] pConversion pointer to IConversion object, NULL to remove reference for conversion.
  //!            <br>Note: conversion object must be in same file and match the data type of the channel.
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetConversion(IConversion* pConversion) = 0;

  ///////////////////////////////////////////////////
  //! Set unit of channel
  //! \note only available for newly created or writable file, not available for MDF3 or for array child channels.
  //!       In MDF3, a unit can only be set for a conversion. In case create a new conversion (e.g. IFactory::CreateIdentityConversion)
  //!       and set the unit for the conversion.
  //! \param[in] pUnit pointer to IUnit object, NULL to remove reference for unit.
  //!            <br>Note: object must be in same file and if unit is XML for MDF4, it should match the respective XML schema (cn_unit.XSD).
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetUnit(IUnit* pUnit) = 0;

  ///////////////////////////////////////////////////
  //! Get the assigned channel to be preferably used as X axis
  //!
  //! \note the default X channel is optional and not available for MDF < 4.1.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     on success the IChannel interface of the assigned default X channel is returned, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL GetDefaultXChannel(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set the channel to be preferably used as X axis
  //!
  //! \note only available for newly created or writable file, not available for MDF < 4.1 or for array child channels.
  //! \param[in] pChannel pointer to IChannel object, NULL to remove reference to default X channel.
  //!            <br>Note: object must be in same file.
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetDefaultXChannel(IChannel* pChannel) = 0;

  ///////////////////////////////////////////////////
  //! Set physical value range of channel
  //!
  //! \note only available for newly created or writable file, not available for array child channels in MDF4 files
  //!
  //! Set the minimum and maximum physical value that occurred for the signal.
  //! For arrays, the minimum and maximum value is calculated over all elements.
  //! \note range should only be set if conversion rule is used exclusively for this channel.
  //! \param[in] min minimum physical value
  //! \param[in] max maximum physical value
  //! \param[in] isValid false if physical range is invalid, true if valid (default)
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetPhysRange(REAL min, REAL max, bool isValid = true) = 0;

  ///////////////////////////////////////////////////
  //! Read physical value range of channel
  //!
  //! Usually the physical value range stored in the conversion rule assigned to the channel will be returned.
  //! In case the physical value range of the conversion rule is not available, but the raw value range of the channel is valid:
  //! <ul>
  //!   <li>If the channel has no conversion rule, the raw value range will be returned.
  //!   <li>If the conversion rule does not return a numeric type, the raw value range will be returned and the return value will be eInvalidConversionType.
  //!   <li>In all other cases, the raw value range will be converted using the conversion rule:
  //!   <ul>
  //!     <li>On a conversion error, the return value will be the error code of the conversion operation.
  //!     <li>On success it will be either ::eOK or ::eConversionNotMonotone to indicate that the phys value range may not be correct because the conversion may not be monotone.
  //!   </ul>
  //! </ul>
  //! \param[out] pMin if not NULL the minimum physical value of the range will be assigned
  //! \param[out] pMax if not NULL the maximum physical value of the range will be assigned
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL ReadPhysRange(REAL* pMin = NULL, REAL* pMax = NULL) = 0;

  /////////////////////////////////////////////////////
  //! Set display name of channel
  //! \note only available for newly created or writable file, not available for array child channels in MDF4 files and for MDF < 3.0.
  //! For MDF4, the display name is encoded in the comment, therefore setting the IComment after setting of the display name will overwrite it.
  //! \param[in] displayName display name to be set, NULL or empty string if display name should be deleted.
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetDisplayName(LPCTSTR displayName) = 0;

  ///////////////////////////////////////////////////
  //! Create a data pointer object to navigate and read signal data for this channel
  //! \param[in]  options bit combination of option flags, see ::DataPointerOptionsFlagsEnum
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IDataPointer object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IDataPointer* MDFLIBCALL CreateDataPointerEx(UINT32 options, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Remove attachment reference of given index
  //! \note Only available for newly created or writable file, not available for MDF < 4.1 or for array child channels
  //! This does not affect the attachment reference of a channel with type ::eSynchronization
  //! Keep in mind that the indices of other channel attachments may change.
  //! \param[in]  index index of attachment in channel attachments
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL RemoveAttachmentRef(UINT64 index) = 0;

  ///////////////////////////////////////////////////
  //! Remove all references to the given attachment
  //! \note Only available for newly created or writable file, not available for MDF < 4.1 or for array child channels
  //! This also affects the attachment reference of a channel with type ::eSynchronization
  //! Keep in mind that the indices of other channel attachments may change.
  //! \param[in] pAttachment pointer to IAttachment object (must not be NULL).
  //!            <br>Note: attachment object must be in same file and in global list of attachments.
  //! \return error code of the operation, eNotFound if the attachment was not referenced by this channel
  virtual ErrorCode MDFLIBCALL RemoveAllAttachmentRefsTo(IAttachment* pAttachment) = 0;

  ///////////////////////////////////////////////////
  //! Convert this channel to an array of channels
  //! \note only available for newly created or writable file and for fixed-length value channels (channel type ::eFixedLength). Currently not available for MDF 3.x and for array or structure child channels or structures or arrays.
  //! The current channel will be used as "template" for creating an array of channels where each element channel
  //! has the same properties except that the byte offset and (if used) the invalidation bit position varies.
  //! The composition type of the parent channel will be changed to ::eIsArray.
  //! \param[in] dimCount         number of dimensions for the array (must be > 0)
  //! \param[in] pDimSizeArray    array with size (number of elements) for each dimension.
  //!                             <br>pDimSizeArray must not be NULL and must have at least dimCount elements. Each size value must be > 0.
  //! \param[in] type             type of the array, see ::ArrayTypeEnum.
  //!                             <br>Note: if an array of arrays is created, the main array must have the same type as the sub array.
  //! \param[in] flags            bit flags for the array properties, see ::ArrayPropertyFlagsEnum.
  //!                             <br>Note: flags ::eHasDynamicSize, ::eHasInputQuantity, ::eHasOutputQuantity, ::eHasComparisonQuantity and ::eHasAxis only prepare the array that these properties are available.
  //!                             However, each property (e.g. axis) must be set individually by getting the IArrayInfo objects with GetArrayInfo and using the respective Set methods.
  //! \param[in] byteOffsetBase   Base factor for calculation of byte offsets for each element channel from the template channel (this channel).
  //!                             If byteOffsetBase is 0, then the byte offset is used from the size in bytes of the element channels,
  //!                             i.e. component values are stored contiguously (next to each other without gaps).
  //!                             If byteOffsetBase is not 0, then its absolute value should be larger than or equal to the size in bytes of the element channels.
  //!                             If byteOffsetBase is negative, the component values are stored with decreasing index.
  //! \param[in] invalBitPosBase  Base factor for calculation of invalidation bit positions for each element channel from the template channel (this channel).
  //!                             Note: only used if template channel uses invalidation bits (see ::eFlagInvalBitUsed).
  //!                             If invalBitPosBase is 0, then all element channels use the same invalidation bit in the record, i.e. either all are valid or all are invalid.
  //!                             If invalBitPosBase > 0, each element channel will have its own invalidation bit.
  //!                             If invalBitPosBase is 1, then the individual invalidation bits are stored without gaps.
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL ConvertToArray(UINT16 dimCount,
                                              UINT64* pDimSizeArray,
                                              ArrayType type = eIsValueArray,
                                              UINT32 flags = 0,
                                              INT32 byteOffsetBase = 0,
                                              UINT32 invalBitPosBase = 0) = 0;

  ///////////////////////////////////////////////////
  //! Get information about an array channel
  //! \note Only available for array channels
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     on success an IArrayInfo interface with information about the array is returned, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IArrayInfo* MDFLIBCALL GetArrayInfo(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get event set for events for this channel
  //! \note The resulting IEventSet object offers further filtering of the events.
  //! \param[in]  filterFlags flags for filtering, value is a bit combination of ::EventFilterFlagsEnum
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IEventSet object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IEventSet* MDFLIBCALL GetEventSet(UINT32 filterFlags = 0, ErrorCode* pErrorCode = NULL) = 0;

  /////////////////////////////////////////////////////
  //! Set/change the name of the channel.
  //!
  //! \note only available for newly created or writable file, not available for array child channels in MDF4 files.
  //!
  //! Typically the name of a channel is fixed.
  //! For a possible use case for changing the name consider that the channel contains wrong signal
  //! values that should be corrected. If this cannot be achieved simply by assigning a new conversion rule
  //! (e.g. linear or value2value conversion), then a possible solution might be to rename the channel and
  //! to create a new channel (in a new channel group) with the original channel name and with corrected values.
  //! Currently this requires duplication of all master channels in the new channel group, in a future MDF version
  //! there might be an alternative solution by only referencing the original channel group.
  //! \param[in] name string to be used as name (must not be empty!).
  //! \return error code for the operation.
  virtual ErrorCode MDFLIBCALL SetName(LPCTSTR name) = 0;

  ///////////////////////////////////////////////////
  //! Request a flag state
  //! \note only available for MDF4
  //! \param[in]  flag see ::ChannelFlagsEnum for flag values<br>
  //! may even be a bit combination, e.g. GetFlagState(::eFlagCalibration | ::eFlagCalculated)
  //! \return true if flag is set (high), false otherwise
  virtual bool MDFLIBCALL GetFlagStateEx(UINT32 flag) = 0;

  ///////////////////////////////////////////////////
  //! Get all bit flags
  //! \note see ::ChannelFlagsEnum for flag values
  //! \return bit combination of flags
  virtual UINT32 MDFLIBCALL GetFlags() = 0;

  ///////////////////////////////////////////////////
  //! Get start addresses of CN block
  //! \return returns start address of CN block for this channel (for arrays the template CN), -1 on error.
  virtual INT64 MDFLIBCALL GetBlockStartAddress() = 0;

};

