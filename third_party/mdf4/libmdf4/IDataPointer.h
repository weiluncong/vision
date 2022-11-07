/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IDataPointer
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
//! \brief Definition of IDataPointer

///////////////////////////////////////////////////
//! \brief Access to signal data
//!
//! The IDataPointer object gives access to the signal data.
//! <br><br>
//! Signal values are always stored in records.
//! The data pointer allows to navigate in the record stream:
//! <ul>
//!   <li>for sorted data a random access is possible,
//!       i.e. either by seeking a position (i.e. zero-based index)
//!       or by seeking a time value (other master channel types are not supported yet).
//!   <li>for unsorted data only a forward increment is possible,
//!       e.g. using the Inc method or the AutoIncrement read mode.
//! </ul>
//! <br>
//! Using the Read methods, the signal value(s) of a specific channel within the data record(s)
//! can be obtained.
//! <br><br>
//! A data pointer can be created either for a channel group or for a channel
//! (IChannelGroup::CreateDataPointer and IChannel::CreateDataPointer).
//! \note for faster reading use the extended methods IChannelGroup::CreateDataPointerEx resp. IChannel::CreateDataPointerEx and
//! specify using a read buffer. Especially in combination with the MultiRead methods, this will be the fastest way to read a larger number of signal values.
//! <br><br>
//! Created for a channel group, the data pointer gives access to the signal data of all channels
//! in the channel group.<br>
//! Created for a channel, only the values of this channel and its time stamps can be read
//! (possibly better performance if only signal values of this channel are of interest).
//! \note For an array channel (CompositionType ::eIsArray), some of its child channels may <b>NOT</b> be part of
//! the parent channel group! This may happen for MDF3 in case of a distributed CD block (old way to store 2D maps)
//! or for MDF4 in case of a CA block with CG/DG template (not supported yet!)
interface IDataPointer
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IDataPointer* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //!
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Attach this object to the current thread
  //!
  //! This method can be used in case IFile was opened with allowMultiThreading = true
  //! <br>to improve performance when using the IDataPointer object in current thread only.
  //! <br>After calling the method, the object cannot be used in any other thread any longer
  //! <br>(except when calling AttachToCurrentThread again within the new thread!).
  //! \see IFileManager::OpenFile
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL AttachToCurrentThread() = 0;

  ///////////////////////////////////////////////////
  //! Check if data pointer works on sorted data
  //!
  //! Sorted data allows indexed access. This may also be the case for
  //! an unsorted data group if only one of the channel groups has records.
  //! \note In case of unsorted data, the methods SeekPos, SeekTimeNs and
  //! SeekTimeSc are not allowed due to possible low performance.
  //! <br>Consider sorting the file before with IFileManager::SortFile or
  //! using methods SeekFirst and Inc.
  //! \see SeekPos, SeekTimeNs, SeekTimeSec, SeekFirst, Inc
  //!
  //! A data pointer may contain sorted data even if IChannelGroup::IsSorted of
  //! its channel group returns false.
  //! This may be the case if the parent data group contains
  //! several channel groups, but only one of them has samples.
  //! \return true if data is sorted, false otherwise or on error
  virtual bool MDFLIBCALL IsDataSorted() = 0;

  ///////////////////////////////////////////////////
  //! Get the number of records in data
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return the number of records or 0 on error
  virtual UINT64 MDFLIBCALL GetRecordCount(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get the current read mode
  //! \see SetReadMode
  //! \return current read mode, see ::DataReadModeEnum
  virtual DataReadMode MDFLIBCALL GetReadMode() = 0;

  ///////////////////////////////////////////////////
  //! Set the current read mode to use
  //! \note the read mode influences all Read methods
  //! \param[in]  mode read mode to set, see ::DataReadModeEnum
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetReadMode(DataReadMode mode) = 0;

  ///////////////////////////////////////////////////
  //! Get current record position (i.e. zero-based index)
  //! \see SeekPos
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return zero-based index of current record or -1 on error or if not available
  virtual INT64 MDFLIBCALL GetPos(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Seek the record position (i.e. zero-based index)
  //! \note currently only available for sorted data!
  //! \see GetPos, IsDataSorted
  //! \param[in]  pos position to seek, depending on mode (zero-based index)
  //! \param[in]  mode mode for seek, see ::SeekPosModeEnum
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SeekPos(INT64 pos, SeekPosMode mode = eSeekAbsolute) = 0;

  ///////////////////////////////////////////////////
  //! Seek record position with specific time stamp in nanoseconds
  //! \note currently only available for sorted data!
  //! \see GetTimeNs
  //! \param[in]  time time stamp in nanoseconds to seek
  //! \param[in]  mode mode for seek, see ::SeekTimeModeEnum
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SeekTimeNs(INT64 time, SeekTimeMode mode) = 0;

  ///////////////////////////////////////////////////
  //! Seek record position with specific time stamp in seconds
  //! \note currently only available for sorted data!
  //! \see GetTimeNs
  //! \param[in]  time time stamp in seconds to seek
  //! \param[in]  mode mode for seek, see ::SeekTimeModeEnum
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SeekTimeSec(REAL time, SeekTimeMode mode) = 0;

  ///////////////////////////////////////////////////
  //! Get time stamp in nanoseconds for current record position
  //! \note for virtual time master channels, the virtual time will be returned
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return time stamp in nanoseconds or 0 on error or if not available
  virtual INT64 MDFLIBCALL GetTimeNs(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get time stamp in seconds for current record position
  //! \note for virtual time master channels, the virtual time will be returned
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return time stamp in seconds or 0 on error or if not available
  virtual REAL MDFLIBCALL GetTimeSec(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Read the physical value for the given channel as double value
  //! \note in case there is no conversion rule, the raw value will be returned (1:1 conversion)
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pValue buffer to receive the result (must not be NULL)
  //! \param[out] pIsValid if not NULL, false will be assigned for MDF4 if all values are invalid or the invalidation bit is set, otherwise true
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL ReadPhysValueDouble(IChannel* pChannel, REAL* pValue, bool* pIsValid = NULL) = 0;

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
  //! \return error code of the operation (note: if only data size is requested, the standard result is ::eResultBufferTooSmall)
  virtual ErrorCode MDFLIBCALL ReadPhysValueString(IChannel* pChannel, LPTSTR pStringBuffer, UINT32 bufferSize, UINT32* pDataSize = NULL, bool* pIsValid = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Read the raw value for the given channel as double value
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pValue buffer to receive the result (must not be NULL)
  //! \param[out] pIsValid if not NULL, false will be assigned for MDF4 if all values are invalid or the invalidation bit is set, otherwise true
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL ReadRawValueDouble(IChannel* pChannel, REAL* pValue, bool* pIsValid = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Read the raw value for the given channel as Integer value
  //!
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pValue buffer to receive the result (must not be NULL)
  //! \param[out] pIsValid if not NULL, false will be assigned for MDF4 if all values are invalid or the invalidation bit is set, otherwise true
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL ReadRawValueInt64(IChannel* pChannel, INT64* pValue, bool* pIsValid = NULL) = 0;

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
  //! \return error code of the operation (note: if only data size is requested, the standard result is ::eResultBufferTooSmall)
  virtual ErrorCode MDFLIBCALL ReadRawValueByteArray(IChannel* pChannel, BYTE* pByteArray, UINT32 arraySize, UINT32* pDataSize = NULL, bool* pIsValid = NULL) = 0;

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
  //! \return error code of the operation (note: if only data size is requested, the standard result is ::eResultBufferTooSmall)
  virtual ErrorCode MDFLIBCALL ReadRawValueString(IChannel* pChannel, LPTSTR pStringBuffer, UINT32 bufferSize, UINT32* pDataSize = NULL, bool* pIsValid = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of physical values for the given channel as double values in one go
  //!
  //! This function allows to read a whole number of signal values into a value buffer.
  //! <br>Starting from the current position of IDataPointer, the next N values will be stored
  //! in the value buffer.
  //! <br>By default (interval = 1) the values will be stored consecutively.
  //! <br>
  //! <br>Alternatively, using parameter interval > 1, the values can be stored with intervals.
  //! <br>In this case, the i-th value will be stored in pValueArray[(i-1)*interval] (i > 0).
  //! <br>
  //! <br>Values will be stored until the end of pValueArray is reached, i.e. 1 + (int)((arrayElements-1)/interval).
  //! <br>In case there are less values present for the channel than can be stored, the method returns ::eReachedEndOfData.
  //! <br>In case there is some error during reading or converting a value, the loop will be aborted and the error code will be returned.
  //! <br>You should examine the contents of pElementsRead to find out how many values have been actually stored.
  //! \note
  //! <ul>
  //!   <li>Not supported for unsorted data (see IsDataSorted()).
  //!   <li>In case there is no conversion rule, the raw value will be returned (1:1 conversion)
  //!   <li>By default, after reading the values, the original record position will be restored.
  //!       <br>This can be disabled with option ::eDisableAutoRestorePos for CreateDataPointerEx.
  //! </ul>
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pValueArray array buffer to receive the result values (must not be NULL)
  //! \param[in]  arrayElements length of array, i.e. maximum number of elements (values) that can be stored (must not be 0)
  //! \param[out] pElementsRead if not NULL this argument returns the number of elements actually read (<= (arrayElements+interval-1)/interval)
  //! \param[in]  interval this parameter can be used to store values from different channels alternating in the value buffer (must not be 0).
  //!             <br>For interval > 1 thus only each <em>x</em>-th element in the array given by pValueArray will be filled:
  //!             <br><br>\image html MultiReadInterval.png <br><br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL MultiReadPhysValueDouble(IChannel* pChannel, REAL* pValueArray, UINT32 arrayElements, UINT32* pElementsRead = NULL, UINT32 interval = 1) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of phys values for the given channel as string values in one go
  //!
  //! This function allows to read a whole number of signal values into a value buffer.
  //! <br>Starting from the current position of IDataPointer, its next N values will be stored
  //! in the value buffer.
  //! <br>By default (interval = 1) the values will be stored consecutively.
  //! <br>
  //! <br>Alternatively, using parameter interval > 1, the values can be stored with intervals.
  //! <br>In this case, the i-th value will be stored in ppStringArray[(i-1)*interval] (i > 0).
  //! <br>
  //! <br>Values will be stored until the end of ppStringArray is reached, i.e. 1 + (int)((arrayElements-1)/interval).
  //! <br>In case there is some error during reading a value, the loop will be aborted and the error code will be returned.
  //! <br>You should examine the contents of pElementsRead to find out how many values have been actually stored.
  //! \note
  //! <ul>
  //!   <li>Only possible if conversion returns a string or 1:1 conversion for string data types.
  //!   <li>Not supported for unsorted data (see IsDataSorted()).
  //!   <li>Each string will be transformed to Unicode or MBCS depending on the library.
  //!   <li>Each result string always is zero terminated.
  //!   <li>Please note that the stringBufferSize is in Bytes and not in characters.
  //!   <li>By default, after reading the values, the original record position will be restored.
  //!       <br>This can be disabled with option ::eDisableAutoRestorePos for CreateDataPointerEx.
  //! </ul>
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] ppStringArray string array buffer to receive the result strings (must not be NULL)
  //! \param[in]  arrayElements length of array, i.e. maximum number of elements (strings) that can be stored (both ppStringArray and all elements must not be NULL unless stringBufferSize is 0 for reading the data sizes)
  //! \param[in]  stringBufferSize size of the result buffer ppStringArray[i] in Bytes (!) (can be 0 in order to only read the data sizes!)
  //!             <br>If a result string (including zero termination) does not fit into the result buffer,
  //!             <br>the string will be truncated and, if everything else was OK, the final return code will be ::eResultBufferTooSmall.
  //! \param[out] pDataSizeArray if not NULL, the actual size of the data in Bytes will be returned for each read value.
  //!             <br>The length of pDataSizeArray (i.e. the number of elements) must be at least arrayElements.
  //!             <br>For interval > 1, the values will be stored with intervals like for ppStringArray.
  //!             <br>Note: the size returned might not contain space for a zero termination required
  //!             <br>in case the string value stored in the record is not zero terminated yet and there is no conversion.
  //!             <br>When only reading data size, for performance reasons in some cases only the number of bytes used in MDF will be returned and not (yet) the actual string length.
  //! \param[out] pElementsRead if not NULL this argument returns the number of elements actually read (<= (arrayElements+interval-1)/interval)
  //! \param[in]  interval this parameter can be used to store values from different channels alternating in the value buffer (must not be 0).
  //!             <br>For interval > 1 thus only each <em>x</em>-th element in the array given by ppStringArray will be filled:
  //!             <br><br>\image html MultiReadInterval.png <br><br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL MultiReadPhysValueString(IChannel* pChannel, LPTSTR* ppStringArray, UINT32 arrayElements, UINT32 stringBufferSize, UINT32* pDataSizeArray = NULL, UINT32* pElementsRead = NULL, UINT32 interval = 1) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of raw values for the given channel as double values in one go
  //!
  //! This function allows to read a whole number of signal values into a value buffer.
  //! <br>Starting from the current position of IDataPointer, its next N values will be stored
  //! in the value buffer.
  //! <br>By default (interval = 1) the values will be stored consecutively.
  //! <br>
  //! <br>Alternatively, using parameter interval > 1, the values can be stored with intervals.
  //! <br>In this case, the i-th value will be stored in pValueArray[(i-1)*interval] (i > 0).
  //! <br>
  //! <br>Values will be stored until the end of pValueArray is reached, i.e. 1 + (int)((arrayElements-1)/interval).
  //! <br>In case there are less values present for the channel than can be stored, the method returns ::eReachedEndOfData.
  //! <br>In case there is some error during reading or converting a value, the loop will be aborted and the error code will be returned.
  //! <br>You should examine the contents of pElementsRead to find out how many values have been actually stored.
  //! \note
  //! <ul>
  //!   <li>Not supported for unsorted data (see IsDataSorted()).
  //!   <li>By default, after reading the values, the original record position will be restored.
  //!       <br>This can be disabled with option ::eDisableAutoRestorePos for CreateDataPointerEx.
  //! </ul>
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pValueArray array buffer to receive the result values (must not be NULL)
  //! \param[in]  arrayElements length of array, i.e. maximum number of elements (values) that can be stored (must not be 0)
  //! \param[out] pElementsRead if not NULL this argument returns the number of elements actually read (<= (arrayElements+interval-1)/interval)
  //! \param[in]  interval this parameter can be used to store values from different channels alternating in the value buffer (must not be 0).
  //!             <br>For interval > 1 thus only each <em>x</em>-th element in the array given by pValueArray will be filled:
  //!             <br><br>\image html MultiReadInterval.png <br><br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL MultiReadRawValueDouble(IChannel* pChannel, REAL* pValueArray, UINT32 arrayElements, UINT32* pElementsRead = NULL, UINT32 interval = 1) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of raw values for the given channel as Integer values in one go
  //!
  //! This function allows to read a whole number of signal values into a value buffer.
  //! <br>Starting from the current position of IDataPointer, its next N values will be stored
  //! in the value buffer.
  //! <br>By default (interval = 1) the values will be stored consecutively.
  //! <br>
  //! <br>Alternatively, using parameter interval > 1, the values can be stored with intervals.
  //! <br>In this case, the i-th value will be stored in pValueArray[(i-1)*interval] (i > 0).
  //! <br>
  //! <br>Values will be stored until the end of pValueArray is reached, i.e. 1 + (int)((arrayElements-1)/interval).
  //! <br>In case there is some error during reading a value, the loop will be aborted and the error code will be returned.
  //! <br>You should examine the contents of pElementsRead to find out how many values have been actually stored.
  //! \note
  //! <ul>
  //!   <li>Not supported for unsorted data (see IsDataSorted()).
  //!   <li>By default, after reading the values, the original record position will be restored.
  //!       <br>This can be disabled with option ::eDisableAutoRestorePos for CreateDataPointerEx.
  //! </ul>
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pValueArray array buffer to receive the result values (must not be NULL)
  //! \param[in]  arrayElements length of array, i.e. maximum number of elements (values) that can be stored (must not be 0)
  //! \param[out] pElementsRead if not NULL this argument returns the number of elements actually read (<= (arrayElements+interval-1)/interval)
  //! \param[in]  interval this parameter can be used to store values from different channels alternating in the value buffer (must not be 0).
  //!             <br>For interval > 1 thus only each <em>x</em>-th element in the array given by pValueArray will be filled:
  //!             <br><br>\image html MultiReadInterval.png <br><br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL MultiReadRawValueInt64(IChannel* pChannel, INT64* pValueArray, UINT32 arrayElements, UINT32* pElementsRead = NULL, UINT32 interval = 1) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of raw values for the given channel as string values in one go
  //!
  //! This function allows to read a whole number of signal values into a value buffer.
  //! <br>Starting from the current position of IDataPointer, its next N values will be stored
  //! in the value buffer.
  //! <br>By default (interval = 1) the values will be stored consecutively.
  //! <br>
  //! <br>Alternatively, using parameter interval > 1, the values can be stored with intervals.
  //! <br>In this case, the i-th value will be stored in ppStringArray[(i-1)*interval] (i > 0).
  //! <br>
  //! <br>Values will be stored until the end of ppStringArray is reached, i.e. 1 + (int)((arrayElements-1)/interval).
  //! <br>In case there is some error during reading a value, the loop will be aborted and the error code will be returned.
  //! <br>You should examine the contents of pElementsRead to find out how many values have been actually stored.
  //! \note
  //! <ul>
  //!   <li>Only possible for string data types.
  //!   <li>Not supported for unsorted data (see IsDataSorted())
  //!   <li>Each string will be transformed to Unicode or MBCS depending on the library.
  //!   <li>Each result string always is zero terminated.
  //!   <li>Please note that the stringBufferSize is in Bytes and not in characters.
  //!   <li>By default, after reading the values, the original record position will be restored.
  //!       <br>This can be disabled with option ::eDisableAutoRestorePos for CreateDataPointerEx.
  //! </ul>
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] ppStringArray string array buffer to receive the result strings (both ppStringArray and all elements must not be NULL unless stringBufferSize is 0 for reading the data sizes)
  //! \param[in]  arrayElements length of array, i.e. maximum number of elements (strings) that can be stored (must not be 0)
  //! \param[in]  stringBufferSize size of the result buffer ppStringArray[i] in Bytes (!) (can be 0 in order to only read the data sizes!)
  //!             <br>If a result string (including zero termination) does not fit into the result buffer,
  //!             <br>the string will be truncated and, if everything else was OK, the final return code will be ::eResultBufferTooSmall.
  //! \param[out] pDataSizeArray if not NULL, the actual size of the data in Bytes will be returned for each read value:
  //!             <br>The length of pDataSizeArray (i.e. the number of elements) must be at least arrayElements.
  //!             <br>For interval > 1, the values will be stored with intervals like for ppStringArray.
  //!             <br>Note: the size returned might not contain space for a zero termination required
  //!             <br>in case the string value stored in the record is not zero terminated yet.
  //!             <br>When only reading data size, for performance reasons in some cases only the number of bytes used in MDF will be returned and not (yet) the actual string length.
  //! \param[out] pElementsRead if not NULL this argument returns the number of elements actually read (<= (arrayElements+interval-1)/interval)
  //! \param[in]  interval this parameter can be used to store values from different channels alternating in the value buffer (must not be 0).
  //!             <br>For interval > 1 thus only each <em>x</em>-th element in the array given by ppStringArray will be filled:
  //!             <br><br>\image html MultiReadInterval.png <br><br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL MultiReadRawValueString(IChannel* pChannel, LPTSTR* ppStringArray, UINT32 arrayElements, UINT32 stringBufferSize, UINT32* pDataSizeArray = NULL, UINT32* pElementsRead = NULL, UINT32 interval = 1) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of raw values for the given channel as byte arrays in one go
  //!
  //! This function allows to read a whole number of signal values into a value buffer.
  //! <br>Starting from the current position of IDataPointer, its next N values will be stored
  //! in the value buffer.
  //! <br>By default (interval = 1) the values will be stored consecutively.
  //! <br>
  //! <br>Alternatively, using parameter interval > 1, the values can be stored with intervals.
  //! <br>In this case, the i-th value will be stored in ppBufferArray[(i-1)*interval] (i > 0).
  //! <br>
  //! <br>Values will be stored until the end of ppBufferArray is reached, i.e. 1 + (int)((arrayElements-1)/interval).
  //! <br>In case there is some error during reading a value, the loop will be aborted and the error code will be returned.
  //! <br>You should examine the contents of pElementsRead to find out how many values have been actually stored.
  //! \note
  //! <ul>
  //!   <li>Not supported for unsorted data (see IsDataSorted()).
  //!   <li>By default, after reading the values, the original record position will be restored.
  //!       <br>This can be disabled with option ::eDisableAutoRestorePos for CreateDataPointerEx.
  //! </ul>
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] ppBufferArray array buffer to receive the results, i.e. the values as byte arrays (both ppBufferArray and all elements must not be NULL unless elementBufferSize is 0 for reading the data sizes)
  //! \param[in]  arrayElements length of array, i.e. maximum number of elements (byte arrays) that can be stored (must not be 0)
  //! \param[in]  elementBufferSize size of the result buffer ppBufferArray[i] in Bytes (can be 0 in order to only read the data sizes!)
  //! \param[out] pDataSizeArray if not NULL, the actual size of the data in Bytes will be returned for each read value:
  //! <ul>
  //!   <li>If data size is less than elementBufferSize, only consider the respective number of bytes.
  //!   <li>If data size is larger than elementBufferSize, not all bytes have been copied
  //!       <br>and the total return code (if everything else was OK) will be ::eResultBufferTooSmall.
  //! </ul>
  //! The length of pDataSizeArray (i.e. the number of elements) must be at least arrayElements.
  //! <br>For interval > 1, the values will be stored with intervals like for ppBufferArray.
  //! \param[out] pElementsRead if not NULL this argument returns the number of elements actually read (<= (arrayElements+interval-1)/interval)
  //! \param[in]  interval this parameter can be used to store values from different channels alternating in the value buffer (must not be 0).
  //! <br>For interval > 1 thus only each <em>x</em>-th element in the array given by ppBufferArray will be filled:
  //! <br><br>\image html MultiReadInterval.png <br><br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL MultiReadRawValueByteArray(IChannel* pChannel, BYTE** ppBufferArray, UINT32 arrayElements, UINT32 elementBufferSize, UINT32* pDataSizeArray = NULL, UINT32* pElementsRead = NULL, UINT32 interval = 1) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of raw values for the given channel as Byte arrays into a Byte stream
  //!
  //! This function allows to read a whole number of signal values into a stream buffer.
  //! \note In contrast to MultiReadRawValueByteArray the size of a single value is not restricted!
  //! <br>Starting from the current position of IDataPointer, its next N values will be stored
  //! in the stream buffer.
  //! <br>By default (offset = 0) the values will be stored consecutively.
  //! <br>
  //! <br>Alternatively, using parameter offset > 0, a gap of offset Bytes will be left between the last Byte of one value and the first Byte of the next value (see Example below).
  //! <br>
  //! <br>At most maxElementsToRead values will be stored. The routine also stops
  //! <ul>
  //!     <li>if the end of pByteStream is reached (return value ::eResultBufferTooSmall), or
  //!     <li>if there are no more values (return value ::eReachedEndOfData).
  //! </ul>
  //! In case there is some error during reading a value, the loop will be aborted and the error code will be returned.
  //! <br>You should examine the contents of pElementsRead to find out how many values have been actually stored.
  //! \note
  //! <ul>
  //!   <li>Not supported for unsorted data (see IsDataSorted()).
  //!   <li>By default, after reading the values, the original record position will be restored.
  //!       <br>This can be disabled with option ::eDisableAutoRestorePos for CreateDataPointerEx.
  //! </ul>
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pByteStream stream buffer to store the raw values (must not be NULL)
  //! \param[in]  streamLength length of stream in Bytes (must not be 0)
  //! \param[in]  maxElementsToRead maximum number of elements to read
  //! \param[out] pDataSizeArray if not NULL, the actual size of the data in Bytes will be returned for each read value
  //! <br>the length of pDataSizeArray (i.e. the number of elements) must be at least maxElementsToRead.
  //! \param[out] pElementsRead if not NULL this argument returns the number of elements actually read (<= maxElementsToRead)
  //! \param[in]  offset offset in Bytes, i.e. number of Bytes to skip after end of one value and begin of next value. Example:
  //! <br><br> \image html OffsetInStream.png <br><br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL MultiReadRawValueByteArrayIntoStream(IChannel* pChannel, BYTE* pByteStream, UINT32 streamLength, UINT32 maxElementsToRead, UINT32* pDataSizeArray = NULL, UINT32* pElementsRead = NULL, UINT32 offset = 0) = 0;

  ///////////////////////////////////////////////////
  //! Request the state of the invalidation bit for the given channel
  //! \note invalidation bits are not available for MDF3 and optional for MDF4
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return true if invalidation bit is set for given channel, false otherwise or on error
  virtual bool MDFLIBCALL GetInvalBitState(IChannel* pChannel, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of invalidation bit states for the given channel as Boolean values in one go
  //!
  //! This function allows to read a whole number of invalidation bit state values into an array of Boolean values.
  //! <br>Starting from the current position of IDataPointer, the next N bit state values will be stored
  //! in the array.
  //! <br>By default (interval = 1) the values will be stored consecutively.
  //! <br>
  //! <br>Alternatively, using parameter interval > 1, the values can be stored with intervals.
  //! <br>In this case, the i-th state value will be stored in pStateArray[(i-1)*interval] (i > 0).
  //! <br>
  //! <br>State values will be stored until the end of pStateArray is reached, i.e. 1 + (int)((arrayElements-1)/interval).
  //! <br>In case there are less state values present for the channel than can be stored, the method returns ::eReachedEndOfData.
  //! <br>In case there is some error during reading or converting a value, the loop will be aborted and the error code will be returned.
  //! <br>You should examine the contents of pElementsRead to find out how many values have been actually stored.
  //! \note
  //! <ul>
  //!   <li>Not supported for unsorted data (see IsDataSorted()).
  //!   <li>In case the channel has no invalidation bit, eNotAvailable will be returned.
  //!   <li>The respective element in the array will be set to true if invalidation bit is set for given channel, or to false otherwise or on error.
  //! </ul>
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pStateArray array buffer to receive the state values (must not be NULL)
  //! \param[in]  arrayElements length of array, i.e. maximum number of elements (state values) that can be stored (must not be 0)
  //! \param[out] pElementsRead if not NULL this argument returns the number of elements actually read (<= (arrayElements+interval-1)/interval)
  //! \param[in]  interval this parameter can be used to store state values from different channels alternating in the value buffer (must not be 0).
  //!             <br>For interval > 1 thus only each <em>x</em>-th element in the array given by pStateArray will be filled:
  //!             <br><br>\image html MultiReadInterval.png <br><br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL MultiReadInvalBitState(IChannel* pChannel, bool* pStateArray, UINT32 arrayElements, UINT32* pElementsRead = NULL, UINT32 interval = 1) = 0;

  ///////////////////////////////////////////////////
  //! Seek the first record
  //! \note same as SeekPos(0), except that this also works for unsorted data
  //!
  //! Resets the pointer to the start
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SeekFirst() = 0;

  ///////////////////////////////////////////////////
  //! Increment record position
  //! \note same as SeekPos(steps, eSeekRelative), except that this also works for unsorted data
  //! \param steps number of steps to increment (forward only!)
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL Inc(UINT64 steps = 1) = 0;

  ///////////////////////////////////////////////////
  //! Get the compression ("zip") type
  //! \note compression is only available for MDF 4.1 or higher. Older versions never have a compression.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return compression type used for the (main) data block (see ::ZipTypeEnum), or ::eZipTypeUnspec in case of error or if no compression is used
  virtual ZipType MDFLIBCALL GetZipType(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of physical values for the given channel as double values and the respective time stamps in ns in one go
  //!
  //! This function is similar to MultiReadPhysValueDouble, but also reads the time stamps in ns. For details see description there.
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pTimeStampArrayNs optional array buffer to receive the time stamps in ns (can be NULL)
  //!             <br>Note that the time values are always stored consecutively, independent of parameter interval.
  //!             <br>The length of the array must be large enough to store as many time stamps as values can be stored in the value array<br>, i.e. at least (arrayElements+interval-1)/interval.
  //! \param[out] pValueArray array buffer to receive the result values (must not be NULL)
  //! \param[in]  arrayElements length of array, i.e. maximum number of elements (values/time stamps) that can be stored (must not be 0)
  //! \param[out] pElementsRead if not NULL this argument returns the number of elements actually read (<= (arrayElements+interval-1)/interval)
  //! \param[in]  interval this parameter can be used to store values from different channels alternating in the value buffer (must not be 0).
  //!             <br>For interval > 1 thus only each <em>x</em>-th element in the array given by pValueArray will be filled:
  //!             <br><br>\image html MultiReadInterval.png <br><br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL MultiReadTimeNsAndPhysValueDouble(IChannel* pChannel, INT64* pTimeStampArrayNs, REAL* pValueArray, UINT32 arrayElements, UINT32* pElementsRead = NULL, UINT32 interval = 1) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of phys values for the given channel as string values and the respective time stamps in ns in one go
  //!
  //! This function is similar to MultiReadPhysValueString, but also reads the time stamps in ns. For details see description there.
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pTimeStampArrayNs optional array buffer to receive the time stamps in ns (can be NULL)
  //!             <br>Note that the time values are always stored consecutively, independent of parameter interval.
  //!             <br>The length of the array must be large enough to store as many time stamps as values can be stored in the value array,
  //!             <br>i.e. at least (arrayElements+interval-1)/interval.
  //! \param[out] ppStringArray string array buffer to receive the result strings (both ppStringArray and all elements must not be NULL unless stringBufferSize is 0 for reading the data sizes)
  //! \param[in]  arrayElements length of value array, i.e. maximum number of elements (strings) that can be stored (must not be 0)
  //! \param[in]  stringBufferSize size of the result buffer ppStringArray[i] in Bytes (!) (can be 0 in order to only read the data sizes!)
  //!             <br>If a result string (including zero termination) does not fit into the result buffer,
  //!             <br>the string will be truncated and, if everything else was OK, the final return code will be ::eResultBufferTooSmall.
  //! \param[out] pDataSizeArray if not NULL, the actual size of the data in Bytes will be returned for each read value.
  //!             <br>The length of pDataSizeArray (i.e. the number of elements) must be at least arrayElements.
  //!             <br>For interval > 1, the values will be stored with intervals like for ppStringArray.
  //!             <br>Note: the size returned might not contain space for a zero termination required
  //!             <br>in case the string value stored in the record is not zero terminated yet and there is no conversion.
  //!             <br>When only reading data size, for performance reasons in some cases only the number of bytes used in MDF will be returned and not (yet) the actual string length.
  //! \param[out] pElementsRead if not NULL this argument returns the number of elements actually read (<= (arrayElements+interval-1)/interval)
  //! \param[in]  interval this parameter can be used to store values from different channels alternating in the value buffer (must not be 0).
  //!             <br>For interval > 1 thus only each <em>x</em>-th element in the array given by ppStringArray will be filled:
  //!             <br><br>\image html MultiReadInterval.png <br><br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL MultiReadTimeNsAndPhysValueString(IChannel* pChannel, INT64* pTimeStampArrayNs, LPTSTR* ppStringArray, UINT32 arrayElements, UINT32 stringBufferSize, UINT32* pDataSizeArray = NULL, UINT32* pElementsRead = NULL, UINT32 interval = 1) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of raw values for the given channel as double values and the respective time stamps in ns in one go
  //!
  //! This function is similar to MultiReadRawValueDouble, but also reads the time stamps in ns. For details see description there.
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pTimeStampArrayNs optional array buffer to receive the time stamps in ns (can be NULL)
  //!             <br>Note that the time values are always stored consecutively, independent of parameter interval.
  //!             <br>The length of the array must be large enough to store as many time stamps as values can be stored in the value array,
  //!             <br>i.e. at least (arrayElements+interval-1)/interval.
  //! \param[out] pValueArray array buffer to receive the result values (must not be NULL)
  //! \param[in]  arrayElements length of array, i.e. maximum number of elements (values/time stamps) that can be stored (must not be 0)
  //! \param[out] pElementsRead if not NULL this argument returns the number of elements actually read (<= (arrayElements+interval-1)/interval)
  //! \param[in]  interval this parameter can be used to store values from different channels alternating in the value buffer (must not be 0).
  //!             <br>For interval > 1 thus only each <em>x</em>-th element in the array given by pValueArray will be filled:
  //!             <br><br>\image html MultiReadInterval.png <br><br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL MultiReadTimeNsAndRawValueDouble(IChannel* pChannel, INT64* pTimeStampArrayNs, REAL* pValueArray, UINT32 arrayElements, UINT32* pElementsRead = NULL, UINT32 interval = 1) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of raw values for the given channel as Integer values and the respective time stamps in ns in one go
  //!
  //! This function is similar to MultiReadRawValueInt64, but also reads the time stamps in ns. For details see description there.
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pTimeStampArrayNs optional array buffer to receive the time stamps in ns (can be NULL)
  //!             <br>Note that the time values are always stored consecutively, independent of parameter interval.
  //!             <br>The length of the array must be large enough to store as many time stamps as values can be stored in the value array,
  //!             <br>i.e. at least (arrayElements+interval-1)/interval.
  //! \param[out] pValueArray array buffer to receive the result values (must not be NULL)
  //! \param[in]  arrayElements length of array, i.e. maximum number of elements (values/time stamps) that can be stored (must not be 0)
  //! \param[out] pElementsRead if not NULL this argument returns the number of elements actually read (<= (arrayElements+interval-1)/interval)
  //! \param[in]  interval this parameter can be used to store values from different channels alternating in the value buffer (must not be 0).
  //!             <br>For interval > 1 thus only each <em>x</em>-th element in the array given by pValueArray will be filled:
  //!             <br><br>\image html MultiReadInterval.png <br><br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL MultiReadTimeNsAndRawValueInt64(IChannel* pChannel, INT64* pTimeStampArrayNs, INT64* pValueArray, UINT32 arrayElements, UINT32* pElementsRead = NULL, UINT32 interval = 1) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of raw values for the given channel as string values and the respective time stamps in ns in one go
  //!
  //! This function is similar to MultiReadRawValueString, but also reads the time stamps in ns. For details see description there.
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pTimeStampArrayNs optional array buffer to receive the time stamps in ns (can be NULL)
  //!             <br>Note that the time values are always stored consecutively, independent of parameter interval.
  //!             <br>The length of the array must be large enough to store as many time stamps as values can be stored in the value array,
  //!             <br>i.e. at least (arrayElements+interval-1)/interval.
  //! \param[out] ppStringArray string array buffer to receive the result strings (both ppStringArray and all elements must not be NULL unless stringBufferSize is 0 for reading the data sizes)
  //! \param[in]  arrayElements length of value array, i.e. maximum number of elements (strings) that can be stored (must not be 0)
  //! \param[in]  stringBufferSize size of the result buffer ppStringArray[i] in Bytes (!) (can be 0 in order to only read the data sizes!)
  //!             <br>If a result string (including zero termination) does not fit into the result buffer,
  //!             <br>the string will be truncated and, if everything else was OK, the final return code will be ::eResultBufferTooSmall.
  //! \param[out] pDataSizeArray if not NULL, the actual size of the data in Bytes will be returned for each read value:
  //!             <br>The length of pDataSizeArray (i.e. the number of elements) must be at least arrayElements.
  //!             <br>For interval > 1, the values will be stored with intervals like for ppStringArray.
  //!             <br>Note: the size returned might not contain space for a zero termination required
  //!             <br>in case the string value stored in the record is not zero terminated yet.
  //!             <br>When only reading data size, for performance reasons in some cases only the number of bytes used in MDF will be returned and not (yet) the actual string length.
  //! \param[out] pElementsRead if not NULL this argument returns the number of elements actually read (<= (arrayElements+interval-1)/interval)
  //! \param[in]  interval this parameter can be used to store values from different channels alternating in the value buffer (must not be 0).
  //!             <br>For interval > 1 thus only each <em>x</em>-th element in the array given by ppStringArray will be filled:
  //!             <br><br>\image html MultiReadInterval.png <br><br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL MultiReadTimeNsAndRawValueString(IChannel* pChannel, INT64* pTimeStampArrayNs, LPTSTR* ppStringArray, UINT32 arrayElements, UINT32 stringBufferSize, UINT32* pDataSizeArray = NULL, UINT32* pElementsRead = NULL, UINT32 interval = 1) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of raw values for the given channel as byte arrays and the respective time stamps in ns in one go
  //!
  //! This function is similar to MultiReadRawValueByteArray, but also reads the time stamps in ns. For details see description there.
  //! \param[in]  pChannel channel to use for this operation
  //! \param[out] pTimeStampArrayNs optional array buffer to receive the time stamps in ns (can be NULL)
  //!             <br>Note that the time values are always stored consecutively, independent of parameter interval.
  //!             <br>The length of the array must be large enough to store as many time stamps as values can be stored in the value array,
  //!             <br>i.e. at least (arrayElements+interval-1)/interval.
  //! \param[out] ppBufferArray array buffer to receive the results, i.e. the values as byte arrays (both ppBufferArray and all elements must not be NULL unless elementBufferSize is 0 for reading the data sizes)
  //! \param[in]  arrayElements length of array, i.e. maximum number of elements (byte arrays) that can be stored (must not be 0)
  //! \param[in]  elementBufferSize size of the result buffer ppBufferArray[i] in Bytes (can be 0 in order to only read the data sizes!)
  //! \param[out] pDataSizeArray if not NULL, the actual size of the data in Bytes will be returned for each read value:
  //!             <ul>
  //!               <li>If data size is less than elementBufferSize, only consider the respective number of bytes.
  //!               <li>If data size is larger than elementBufferSize, not all bytes have been copied
  //!                   <br>and the total return code (if everything else was OK) will be ::eResultBufferTooSmall.
  //!             </ul>
  //!             The length of pDataSizeArray (i.e. the number of elements) must be at least arrayElements.
  //!             <br>For interval > 1, the values will be stored with intervals like for ppBufferArray.
  //! \param[out] pElementsRead if not NULL this argument returns the number of elements actually read (<= (arrayElements+interval-1)/interval)
  //! \param[in]  interval this parameter can be used to store values from different channels alternating in the value buffer (must not be 0).
  //!             <br>For interval > 1 thus only each <em>x</em>-th element in the array given by ppBufferArray will be filled:
  //!             <br><br>\image html MultiReadInterval.png <br><br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL MultiReadTimeNsAndRawValueByteArray(IChannel* pChannel, INT64* pTimeStampArrayNs, BYTE** ppBufferArray, UINT32 arrayElements, UINT32 elementBufferSize, UINT32* pDataSizeArray = NULL, UINT32* pElementsRead = NULL, UINT32 interval = 1) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of physical values for each of the given channels as double values and the respective time stamps in ns in one go
  //!
  //! This function is similar to MultiReadPhysValueDouble, but can reads the values of a number of channels and (optionally) the time stamps in ns.
  //! For details see description there.
  //! \param[in]  ppChannels channels to use for this operation (must not be NULL)
  //! \param[in]  channelCount number of channels, i.e. size of array ppChannels
  //! \param[out] pTimeStampArrayNs optional array buffer to receive the time stamps in ns (can be NULL)
  //!             <br>Note that the time values are always stored consecutively, independent of parameter interval.
  //!             <br>The length of the array must be large enough to store as many time stamps as values can be stored in the value arrays,
  //!             <br>i.e. at least (arrayElements+interval-1)/interval.
  //! \param[out] ppValueArrays array of array buffers to receive the result values for the respective channel in ppChannels (must not be NULL)
  //!             <br>the array must contain an array buffer for each channel in ppChannels.
  //! \param[in]  arrayElements length of each array buffer in ppValueArrays (must not be 0), i.e. maximum number of elements (values) that can be stored in case of interval = 1
  //! \param[out] pElementsRead if not NULL this argument returns the number of elements actually read (<= (arrayElements+interval-1)/interval)
  //! \param[in]  interval this parameter can be used to store values from different channels alternating in the value buffer (must not be 0).
  //!             <br>For interval > 1 thus only each <em>x</em>-th element in each array given by ppValueArrays[i] will be filled:
  //!             <br><br>\image html MultiReadInterval.png <br><br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL MultiReadPhysValueDoubleOfMultipleChannels(IChannel** ppChannels, UINT32 channelCount, INT64* pTimeStampArrayNs, REAL** ppValueArrays, UINT32 arrayElements, UINT32* pElementsRead = NULL, UINT32 interval = 1) = 0;

  ///////////////////////////////////////////////////
  //! Check if values of the given channel can be read as PhysValueDouble in general
  //!
  //! This function may help to decide whether to use a MultiReadPhysValueDouble method or iterate over all records and check each result individually.
  //! It considers the options to ignore some warnings specified at creating the IDataPointer (see IChannelGroup::CreateDataPointerEx).
  //! \param[in]  pChannel channel to use for this operation
  //! \param[in]  options bit combination of option flags, see ::CanReadChannelValueFlagsEnum
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

  ///////////////////////////////////////////////////
  //! Read parts of the record data bytes
  //!
  //! \note consider IChannelGroup::GetDataByteCount()
  //! \param[in]  offset offset in Bytes from start of record
  //! \param[in]  count number of bytes to read
  //!             <br>if offset + count exceeds the number of data bytes, the method returns an error code ::eInvalidArgument
  //! \param[out] pByteArray buffer to receive the result (must not be NULL)
  //! \param[in]  arraySize size of the result buffer in Bytes
  //! <ul>
  //!   <li>If count is less than arraySize, only consider the respective number of bytes.
  //!   <li>If count is larger than arraySize, not all Bytes have been copied
  //!       <br>and the return code will be ::eResultBufferTooSmall.
  //! </ul>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL ReadDataBytesOfRecord(UINT32 offset, UINT32 count, BYTE* pByteArray, UINT32 arraySize) = 0;

  ///////////////////////////////////////////////////
  //! Read parts of the record invalidation bytes
  //!
  //! \note consider IChannelGroup::GetInvalByteCount()
  //! \param[in]  offset offset in Bytes from start of invalidation bytes
  //! \param[in]  count number of bytes to read
  //!             <br>if offset + count exceeds the number of invalidation bytes, the method returns an error code ::eInvalidArgument
  //! \param[out] pByteArray buffer to receive the result (must not be NULL)
  //! \param[in]  arraySize size of the result buffer in Bytes
  //! <ul>
  //!   <li>If count is less than arraySize, only consider the respective number of bytes.
  //!   <li>If count is larger than arraySize, not all Bytes have been copied
  //!       <br>and the return code will be ::eResultBufferTooSmall.
  //! </ul>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL ReadInvalBytesOfRecord(UINT32 offset, UINT32 count, BYTE* pByteArray, UINT32 arraySize) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of raw values for each of the given channels as double values and the respective time stamps in ns in one go
  //!
  //! This function is similar to MultiReadRawValueDouble, but can reads the values of a number of channels and (optionally) the time stamps in ns.
  //! For details see description there.
  //! \param[in]  ppChannels channels to use for this operation (must not be NULL)
  //! \param[in]  channelCount number of channels, i.e. size of array ppChannels
  //! \param[out] pTimeStampArrayNs optional array buffer to receive the time stamps in ns (can be NULL)
  //!             <br>Note that the time values are always stored consecutively, independent of parameter interval.
  //!             <br>The length of the array must be large enough to store as many time stamps as values can be stored in the value arrays,
  //!             <br>i.e. at least (arrayElements+interval-1)/interval.
  //! \param[out] ppValueArrays array of array buffers to receive the result values for the respective channel in ppChannels (must not be NULL)
  //!             <br>the array must contain an array buffer for each channel in ppChannels.
  //! \param[in]  arrayElements length of each array buffer in ppValueArrays (must not be 0), i.e. maximum number of elements (values) that can be stored in case of interval = 1
  //! \param[out] pElementsRead if not NULL this argument returns the number of elements actually read (<= (arrayElements+interval-1)/interval)
  //! \param[in]  interval this parameter can be used to store values from different channels alternating in the value buffer (must not be 0).
  //!             <br>For interval > 1 thus only each <em>x</em>-th element in each array given by ppValueArrays[i] will be filled:
  //!             <br><br>\image html MultiReadInterval.png <br><br>
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL MultiReadRawValueDoubleOfMultipleChannels(IChannel** ppChannels, UINT32 channelCount, INT64* pTimeStampArrayNs, REAL** ppValueArrays, UINT32 arrayElements, UINT32* pElementsRead = NULL, UINT32 interval = 1) = 0;

};
