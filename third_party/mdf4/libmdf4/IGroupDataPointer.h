/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IGroupDataPointer
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
//! \brief Definition of IGroupDataPointer

///////////////////////////////////////////////////
//! \brief Access to signal data synchronized over several channel groups
//!
//! The IGroupDataPointer object gives access to signal data from several channel groups
//! synchronized by a synchronization domain (currently only time).
//! <br>
//! It can also be used to read interpolated values for equidistant time steps.
//! <br><br>
//! A group data pointer can be created by IFile::CreateGroupDataPointer.
interface IGroupDataPointer
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IGroupDataPointer* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //!
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Adds a channel group to the IGroupDataPointer
  //!
  //! Adds a channel group to the internal array of channel groups.
  //! \note Channel groups without signal data will be refused.
  //! The return value is the zero-based index of the channel group as to be used for other methods (e.g. GetRecordCount, GetTimeNs, ...).
  //! Adding a new channel group after reading has started requires a new call to SeekFirst!
  //! \note: It is not allowed to add a channel group from a different file, nor to add it a second time. In addition, channel groups without any samples will be rejected (return value = eNoDataAvailable).
  //! \param[in]  pChannelGroup channel group to be added. Note that the channel group will not be released before IGroupDataPointer::Release.
  //! \param[in]  timeOffsetNs a time offset in nanoseconds can be assigned for the channel group.
  //!             <br>The offset will be added to the time value of the channel group, e.g. to synchronize the time stamps for channel groups from different files.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return zero-based index of added channel group, or -1 on error
  virtual INT32 MDFLIBCALL AddChannelGroup(IChannelGroup* pChannelGroup, ErrorCode* pErrorCode = NULL, INT64 timeOffsetNs = 0) = 0;

  ///////////////////////////////////////////////////
  //! Get number of channel groups added successfully to the IGroupDataPointer
  //! \see AddChannelGroup, GetChannelGroup
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return number of channel groups (0 on error)
  virtual UINT64 MDFLIBCALL GetChannelGroupCount(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get channel group for given index
  //! \see AddChannelGroup, GetChannelGroupCount
  //! \param[in]  index zero-based index of channel group in IGroupDataPointer
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IChannelGroup object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannelGroup* MDFLIBCALL GetChannelGroup(UINT64 index, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Attach this object to the current thread
  //!
  //! This method can be used in case IFile was opened with allowMultiThreading = true
  //! <br>to improve performance when using the IGroupDataPointer object in current thread only.
  //! <br>After calling the method, the object cannot be used in any other thread any longer
  //! <br>(except when calling AttachToCurrentThread again within the new thread!).
  //! \see IFileManager::OpenFile
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL AttachToCurrentThread() = 0;

  ///////////////////////////////////////////////////
  //! Seek the first record
  //! \note must be called once before any Inc or Read
  //!
  //! Resets the pointer to the start
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SeekFirst() = 0;

  ///////////////////////////////////////////////////
  //! Increment the record position by given number of increments
  //! \note SeekFirst must have been called once before!
  //! IGroupDataPointer allows only forward increments.
  //! \param[in]  steps number of steps to advance the pointer (default = 1)
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL Inc(UINT64 steps = 1) = 0;

  ///////////////////////////////////////////////////
  //! Increment the current time by the specified time interval
  //! \note SeekFirst must have been called once before!
  //! IGroupDataPointer allows only forward increments. The current time will be incremented and all channel groups will be advanced accordingly.
  //! \param[in]  timeIntervalNs time interval in nanoseconds to advance the current time.
  //! \return error code of the operation, ::eReachedEndOfData when IGroupDataPointer already is at or beyond last data point
  virtual ErrorCode MDFLIBCALL IncTimeNs(UINT64 timeIntervalNs) = 0;

  ///////////////////////////////////////////////////
  //! Increment the current time by the specified time interval
  //! \note SeekFirst must have been called once before!
  //! IGroupDataPointer allows only forward increments. The current time will be incremented and all channel groups will be advanced accordingly.
  //! \param[in]  timeIntervalSec time interval in seconds to advance the current time.
  //! \return error code of the operation, ::eReachedEndOfData when IGroupDataPointer already is at or beyond last data point
  virtual ErrorCode MDFLIBCALL IncTimeSec(REAL timeIntervalSec) = 0;

  ///////////////////////////////////////////////////
  //! Get current record position for total number of records or current position for a specified channel group contained in the IGroupDataPointer array
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \param[in]  groupIndex      <  0 get record position for total number of records (zero-based number of finished records).
  //!                        <br> >= 0 get current position of contained channel group specified by zero-based index (see AddChannelGroup).
  //!                        <br> Default: -1
  //! \return zero-based index of current record or -1 on error or if not available
  virtual INT64 MDFLIBCALL GetPos(ErrorCode* pErrorCode = NULL, INT32 groupIndex = -1) = 0;

  ///////////////////////////////////////////////////
  //! Get the total number of records or the number of records for a specified channel group
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \param[in]  groupIndex      < 0 get total number of records (sum over all contained channel groups).
  //!                        <br> >= 0 get number of records of contained channel group specified by zero-based index (see AddChannelGroup).
  //!                        <br> Default: -1
  //! \return the number of records or 0 on error
  virtual UINT64 MDFLIBCALL GetRecordCount(ErrorCode* pErrorCode = NULL, INT32 groupIndex = -1) = 0;

  ///////////////////////////////////////////////////
  //! Get time stamp in nanoseconds for current position or for a specific channel group
  //!
  //! \note for virtual time master channels, the virtual time will be returned
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \param[in]  groupIndex       < 0: get current total time.
  //!                        <br> >= 0: get current time of contained channel group specified by zero-based index (see AddChannelGroup).
  //!                        <br> Default: -1
  //!                        <br> Note: the current time of a specific channel group may be less than the current total time.
  //! \return time stamp in nanoseconds or 0 on error or if not available
  virtual INT64 MDFLIBCALL GetTimeNs(ErrorCode* pErrorCode = NULL, INT32 groupIndex = -1) = 0;

  ///////////////////////////////////////////////////
  //! Get time stamp in seconds for current position or for a specific channel group
  //! \note for virtual time master channels, the virtual time will be returned
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \param[in]  groupIndex       < 0: get current time.
  //!                        <br> >= 0: get current time of contained channel group specified by zero-based index (see AddChannelGroup).
  //!                        <br> Default: -1
  //!                        <br> Note: the current time of a specific channel group may be less than the current total time.
  //! \return time stamp in seconds or 0 on error or if not available
  virtual REAL MDFLIBCALL GetTimeSec(ErrorCode* pErrorCode = NULL, INT32 groupIndex = -1) = 0;


  ///////////////////////////////////////////////////
  //! Get time stamp in nanoseconds for next position or for next position of a specific channel group
  //!
  //! \note for virtual time master channels, the virtual time will be returned
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \param[in]  groupIndex       < 0: get next time (time after Inc(1)).
  //!                        <br> >= 0: get next time of contained channel group specified by zero-based index (see AddChannelGroup).
  //!                        <br> Default: -1
  //!                        <br>Note: the next time of a specific channel group may be larger than the next total time.
  //! \return time stamp in nanoseconds or 0 on error or if not available
  virtual INT64 MDFLIBCALL GetNextTimeNs(ErrorCode* pErrorCode = NULL, INT32 groupIndex = -1) = 0;

  ///////////////////////////////////////////////////
  //! Get time stamp in seconds for next position or for next position of a specific channel group
  //!
  //! \note for virtual time master channels, the virtual time will be returned
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \param[in]  groupIndex       < 0: get next time (time after Inc(1)).
  //!                        <br> >= 0: get next time of contained channel group specified by zero-based index (see AddChannelGroup).
  //!                        <br> Default: -1
  //!                        <br>Note: the next time of a specific channel group may be larger than the next total time.
  //! \return time stamp in seconds or 0 on error or if not available
  virtual REAL MDFLIBCALL GetNextTimeSec(ErrorCode* pErrorCode = NULL, INT32 groupIndex = -1) = 0;

  ///////////////////////////////////////////////////
  //! Read the physical value for the given channel as double value
  //! \note in case there is no conversion rule, the raw value will be returned (1:1 conversion)
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pValue buffer to receive the result (must not be NULL)
  //! \param[out] pIsValid if not NULL, false will be assigned for MDF4 if all values are invalid or the invalidation bit is set, otherwise true
  //! \param[in]  accessMode the access mode controls whether to read the current, the next or an interpolated value, see ::ValueAccessModeEnum.
  //!             <br>Default: ::eGetCurrent.
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL ReadPhysValueDouble(IChannel* pChannel, REAL* pValue, bool* pIsValid = NULL, ValueAccessMode accessMode = eGetCurrent) = 0;

  ///////////////////////////////////////////////////
  //! Read the phys value for the given channel as string
  //!
  //! \note
  //! <ul>
  //!   <li>Only possible if conversion returns a string or 1:1 conversion for string data types.
  //!   <li>The string will be transformed to Unicode or MBCS depending on the library.
  //!   <li>The result string always is zero terminated.
  //!   <li>Please note that the bufferSize is in Bytes and not in characters.
  //! </ul>
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pStringBuffer buffer to receive the result (must not be NULL unless bufferSize is 0 for reading the data size)
  //! \param[in]  bufferSize size of the result buffer in Bytes (!) (can be 0 in order to only read the data size, in which case pDataSize must be specified)
  //!             <br>If the result string (including zero termination) does not fit into the result buffer,
  //!             <br>the string will be truncated and the return code will be ::eResultBufferTooSmall.
  //! \param[out] pDataSize if not NULL, the actual size of the data in Bytes will be returned
  //!             <br>Note: the size returned in pDataSize might not contain space for a zero termination required
  //!             <br>in case the string value stored in the record is not zero terminated yet and there is no conversion.
  //!             <br>When only reading data size, for performance reasons in some cases only the number of bytes used in MDF will be returned and not (yet) the actual string length.
  //! \param[out] pIsValid if not NULL, false will be assigned for MDF4 if all values are invalid or the invalidation bit is set, otherwise true
  //! \param[in]  accessMode the access mode controls whether to read the current or the the next value, see ::ValueAccessModeEnum.
  //!             <br>Default: ::eGetCurrent.
  //!             <br>Note: for this method, ::eGetInterpolated is equal to ::eGetCurrent.
  //! \return error code of the operation (note: if only data size is requested, the standard result is ::eResultBufferTooSmall)
  virtual ErrorCode MDFLIBCALL ReadPhysValueString(IChannel* pChannel, LPTSTR pStringBuffer, UINT32 bufferSize, UINT32* pDataSize = NULL, bool* pIsValid = NULL, ValueAccessMode accessMode = eGetCurrent) = 0;

  ///////////////////////////////////////////////////
  //! Read the raw value for the given channel as double value
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pValue buffer to receive the result (must not be NULL)
  //! \param[out] pIsValid if not NULL, false will be assigned for MDF4 if all values are invalid or the invalidation bit is set, otherwise true
  //! \param[in]  accessMode the access mode controls whether to read the current, the next or an interpolated value, see ::ValueAccessModeEnum.
  //!             <br>Default: ::eGetCurrent.
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL ReadRawValueDouble(IChannel* pChannel, REAL* pValue, bool* pIsValid = NULL, ValueAccessMode accessMode = eGetCurrent) = 0;

  ///////////////////////////////////////////////////
  //! Read the raw value for the given channel as Integer value
  //!
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pValue buffer to receive the result (must not be NULL)
  //! \param[out] pIsValid if not NULL, false will be assigned for MDF4 if all values are invalid or the invalidation bit is set, otherwise true
  //! \param[in]  accessMode the access mode controls whether to read the current, the next or an interpolated value, see ::ValueAccessModeEnum.
  //!             <br>Default: ::eGetCurrent.
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL ReadRawValueInt64(IChannel* pChannel, INT64* pValue, bool* pIsValid = NULL, ValueAccessMode accessMode = eGetCurrent) = 0;

  ///////////////////////////////////////////////////
  //! Read the raw value for the given channel as BYTE array
  //!
  //! \note for numeric values, the value bytes will be assigned!
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pByteArray buffer to receive the result (must not be NULL unless arraySize is 0 for reading the data size)
  //! \param[in]  arraySize size of the result buffer in Bytes (can be 0 in order to only read the data size, in which case pDataSize must be specified)
  //! \param[out] pDataSize if not NULL, the actual size of the data in Bytes will be returned:
  //! <ul>
  //!   <li>If data size is less than arraySize, only consider the respective number of bytes.
  //!   <li>If data size is larger than arraySize, not all Bytes have been copied
  //!       <br>and the return code will be ::eResultBufferTooSmall.
  //! </ul>
  //! \param[out] pIsValid if not NULL, false will be assigned for MDF4 if all values are invalid or the invalidation bit is set, otherwise true
  //! \param[in]  accessMode the access mode controls whether to read the current or the the next value, see ::ValueAccessModeEnum.
  //!             <br>Default: ::eGetCurrent.
  //!             <br>Note: for this method, ::eGetInterpolated is equal to ::eGetCurrent.
  //! \return error code of the operation (note: if only data size is requested, the standard result is ::eResultBufferTooSmall)
  virtual ErrorCode MDFLIBCALL ReadRawValueByteArray(IChannel* pChannel, BYTE* pByteArray, UINT32 arraySize, UINT32* pDataSize = NULL, bool* pIsValid = NULL, ValueAccessMode accessMode = eGetCurrent) = 0;

  ///////////////////////////////////////////////////
  //! Read the raw value for the given channel as string
  //!
  //! \note
  //! <ul>
  //!   <li>Only possible for string data types.
  //!   <li>The string will be transformed to Unicode or MBCS depending on the library.
  //!   <li>The result string always is zero terminated.
  //!   <li>Please note that the bufferSize is in Bytes and not in characters.
  //! </ul>
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pStringBuffer buffer to receive the result (must not be NULL unless bufferSize is 0 for reading the data size)
  //! \param[in]  bufferSize size of the result buffer in Bytes (!) (can be 0 in order to only read the data size, in which case pDataSize must be specified)
  //!             <br>If the result string (including zero termination) does not fit into the result buffer,
  //!             <br>the string will be truncated and the return code will be ::eResultBufferTooSmall.
  //! \param[out] pDataSize if not NULL, the actual size of the data in Bytes will be returned
  //!             <br>Note: the size returned in pDataSize might not contain space for a zero termination required
  //!             <br>in case the string value stored in the record is not zero terminated yet.
  //!             <br>When only reading data size, for performance reasons in some cases only the number of bytes used in MDF will be returned and not (yet) the actual string length.
  //! \param[out] pIsValid if not NULL, false will be assigned for MDF4 if all values are invalid or the invalidation bit is set, otherwise true
  //! \param[in]  accessMode the access mode controls whether to read the current or the the next value, see ::ValueAccessModeEnum.
  //!             <br>Default: ::eGetCurrent.
  //!             <br>Note: for this method, ::eGetInterpolated is equal to ::eGetCurrent.
  //! \return error code of the operation (note: if only data size is requested, the standard result is ::eResultBufferTooSmall)
  virtual ErrorCode MDFLIBCALL ReadRawValueString(IChannel* pChannel, LPTSTR pStringBuffer, UINT32 bufferSize, UINT32* pDataSize = NULL, bool* pIsValid = NULL, ValueAccessMode accessMode = eGetCurrent) = 0;

  ///////////////////////////////////////////////////
  //! Request the state of the invalidation bit for the given channel
  //! \note invalidation bits are not available for MDF3 and optional for MDF4
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \param[in]  accessMode the access mode controls whether to read the current or the the next value, see ::ValueAccessModeEnum.
  //!             <br>Default: ::eGetCurrent.
  //!             <br>Note: for this method, ::eGetInterpolated will return the logical OR of the bit states for ::eGetCurrent and ::eGetNext.
  //! \return true if invalidation bit is set for given channel, false otherwise or on error
  virtual bool MDFLIBCALL GetInvalBitState(IChannel* pChannel, ErrorCode* pErrorCode = NULL, ValueAccessMode accessMode = eGetCurrent) = 0;

  ///////////////////////////////////////////////////
  //! Check if values of the given channel can be read as PhysValueDouble in general
  //!
  //! This function may help to decide whether to use a MultiReadPhysValue method or iterate over all records and check each result individually.
  //! It considers the options to ignore some warnings specified at creating the IDataPointer (see IChannelGroup::CreateDataPointerEx).
  //! \param[in]  pChannel channel to use for this operation
  //! \param[in]  options bit combination of option flags, see ::CanReadChannelValueFlagsEnum. Note: Currently only 0 is allowed.
  //! \return error code of the operation:
  //! <br>::eOK if PhysValueDouble methods can safely be used
  //! <br>::eInvalidDataTypeForOperation if IChannel neither has not a numeric data type nor a conversion of type ::eText2Value
  //! <br>::eDifferentCoversionResultType if IChannel has a conversion with text output
  //! <br>::eStatusStringTableEntry if IChannel has a status string table conversion
  //! <br>::eFatalFormatError in case of invalid conversion
  //! <br>::eDivisionByZero or ::eLogarithmOfZero in conversion always delivers this result (still can happen for indvidualy channel values!)
  //! <br>::eNoDataAvailable if there are no channel values
  //! <br>... (other error codes possible)
  virtual ErrorCode MDFLIBCALL CanReadChannelValueAsPhysDouble(IChannel* pChannel, UINT32 options = 0) = 0;

};
