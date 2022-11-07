/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IDataWriter
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
//! \brief Definition of IDataWriter

///////////////////////////////////////////////////
//! \brief Object for writing signal data
//!
//! The IDataWriter object can be used to write signal data.
//! <br><br>
//! A data writer is created for a channel group (by IChannelGroup::CreateDataWriter).
//! There can be only one data writer object at one time.
//! It must be released before opening a data writer for another channel group.
//! \note
//! For MDF3, it is not possible to create any other elements (MDF blocks) before release of the data writer object.
//!
//! The signal values are stored in records.
//! The content of a record is defined by the channels of the respective channel group.
//! All signal values in a record have the same time stamp.
//! <br><br>
//! The data writer allows to append records to the record stream of the respective channel group.
//! The writing of a single record is called to "flush" the record.
//! The data writer always creates sorted data.
//! <br><br>
//! A record contains the data bytes for the values of the channels contained in the channel group.
//! For MDF4, it may also contain additional bytes for the invalidation bits used by the channels.
//! With invalidation bits it is possible to invalidate a single value within a specific record.
//! <br><br>
//! Using the Write/SetTime methods, the signal value(s) for a specific channel can be written to the currently accessed record.
//! It is also possible to write the complete record in one go using IDataWriter::WriteRecord.
//! <br><br>
//! There are three ways to use a data writer which usually should not be mixed:
//! <ol>
//! <li>Assembling the record in your own code and writing the complete record with IDataWriter::WriteRecord.<br><br>
//!     This method immediately flushes the record, i.e. writes it to the stream of records.
//!     An explicit call to IDataWriter::FlushRecord is not necessary.
//!     This is the fastest way to write records.<br><br>
//!     <ul>
//!     <li>Setting the flush check mode (see IDataWriter::SetFlushCheckMode) to ::eFlushCheckDuringWrite or ::eFlushCheckAll, then
//!         IDataWriter::WriteRecord checks if any Write/SetTime method was used for the currently accessed record.
//!         In case signal values have been set to the current record, IDataWriter::WriteRecord will indicate thsi by return value ::eNoFlushSinceLastValue (warning),
//!         but the record will be overwritten nevertheless!<br><br>
//!     <li>Setting the flush check mode to ::eFlushCheckDisabled or ::eFlushCheckDuringFlush, there will be no check in IDataWriter::WriteRecord.
//!     </ul>
//!     <br><br>
//! <li>Setting the write mode to ::eAutoFlush (see IDataWriter::SetWriteMode).<br><br>
//!     The signal values can be set to the currently accessed record using Write/SetTime methods for all channels contained in the channel group.
//!     Whenever a Write/SetTime method detects that a channel value already is set in the current record (the channel is "dirty"), it automatically flushes the record.
//!     An explicit call to IDataWriter::FlushRecord is not necessary. The last record will be written when the IDataWriter object is released.<br><br>
//!     <ul>
//!     <li>Setting the flush check mode to ::eFlushCheckDuringFlush or ::eFlushCheckAll, then the automatic flush will check if all channels are dirty.
//!         In case one or more channels are not dirty, the respective Write/SetTime method reports an error and the automatic flush does not occur.<br><br>
//!         Note that this check may not work correctly when using structures or signals with overlapping bytes in the record.<br><br>
//!     <li>Setting the flush check mode to ::eFlushCheckDisabled or ::eFlushCheckDuringFlush, there will be no check. During a flush, non-dirty channel values will be filled with zeros and, if used, the invalidation bits will be set for these channels.
//!     </ul>
//!     <br><br>
//! <li>Setting the write mode to ::eSetValue (using IDataWriter::SetWriteMode).<br><br>
//!     In this case after setting the values with the Write/SetTime methods, an explicit call to IDataWriter::FlushRecord is required.<br><br>
//!     <ul>
//!     <li>Setting the flush check mode to ::eFlushCheckDuringFlush or ::eFlushCheckAll, IDataWriter::FlushRecord will return ::eFlushNotAllValuesWritten as warning in case not all channels are dirty.
//!         Non-dirty channel values will be filled with zeros and, if used, the invalidation bits will be set for these channels.<br><br>
//!     <li>Setting the flush check mode to ::eFlushCheckDuringWrite or ::eFlushCheckAll, the Write/SetTime method will return ::eNoFlushSinceLastValue in case the channel already is dirty. In this case, the existing value will not be overwritten. <br><br>
//!     <li>Without flush check mode (::eFlushCheckDisabled), there is no check. In this case, Write/SetTime will overwrite an already set value.
//!     </ul>
//!     <br><br>
//! </ol>
//! It is also possible to let the IDataWriter check the time stamps and report an error/warning in case of reverse time stamps. For details see IDataWriter::SetTimeCheckMode.
interface IDataWriter
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IDataWriter* MDFLIBCALL GetSelf() = 0;

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
  //! <br>to improve performance when using the IDataWriter object in current thread only.
  //! <br>After calling the method, the object cannot be used in any other thread any longer
  //! <br>(except when calling AttachToCurrentThread again within the new thread!).
  //! \see IFileManager::CreateFile or IFileManager::OpenFile
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL AttachToCurrentThread() = 0;

  ///////////////////////////////////////////////////
  //! Get the number of records in data
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return the number of records or 0 on error
  virtual UINT64 MDFLIBCALL GetRecordCount(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get the current write mode
  //! \see SetWriteMode
  //! \return current read mode, see ::DataWriteModeEnum
  virtual DataWriteMode MDFLIBCALL GetWriteMode() = 0;

  ///////////////////////////////////////////////////
  //! Set the current write mode to use
  //! \note the write mode influences all Write methods
  //! \param[in]  mode write mode to set, see ::DataWriteModeEnum
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetWriteMode(DataWriteMode mode) = 0;

  ///////////////////////////////////////////////////
  //! Flush the current record to file
  //!
  //! \note the method also prepares the new record, i.e. record filled with zeros and all invalidation bits are set.
  //!
  //! Depending on the configured FlushCheckMode (see SetFlushCheckMode), the method may detect that not all channel values were set and return ::eFlushNotAllValuesWritten, but the record will be written nevertheless!
  //! Depending on the configured TimeCheckMode (see SetTimeCheckMode), the method may detect reverse time stamps and return ::eReverseTimeStamp, but the record will be written nevertheless!
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL FlushRecord() = 0;

  ///////////////////////////////////////////////////
  //! Write a complete record
  //!
  //! \note writing a complete record is not possible if the channel group of the record contains a VLSD channel!
  //! For optimized performance the record data (and possibly invalidation) Bytes may be assembled in the calling application and written directly.
  //! Any previously set time stamps or values and invalidation bits in the record will be overwritten!
  //! Depending on the configured FlushCheckMode (see SetFlushCheckMode), the method may detect this and return ::eNoFlushSinceLastValue, but the record will be written nevertheless!
  //! Depending on the configured TimeCheckMode (see SetTimeCheckMode), the method may detect reverse time stamps and return ::eReverseTimeStamp, but the record will be written nevertheless!
  //! \note min/max value statistics for the individual channels are not updated when using this method.
  //! \param[in]  pDataBytes buffer for the data Bytes to write (must not be NULL)
  //! \param[in]  dataByteCount size of the data Byte buffer in Bytes
  //! \param[in]  pInvalBytes buffer for the invalidation Bytes to write (optional, MDF4 only). If NULL, all used invalidation bits will be reset.
  //! \param[in]  invalByteCount size of the invalidation Byte buffer in Bytes
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL WriteRecord(BYTE* pDataBytes, UINT32 dataByteCount, BYTE* pInvalBytes = NULL, UINT32 invalByteCount = 0) = 0;

  ///////////////////////////////////////////////////
  //! Set time stamp in nanoseconds in current record
  //! \note the time stamp should be larger than time stamp of previous record!
  //! Depending on the configured TimeCheckMode (see SetTimeCheckMode), the method may detect reverse time stamps and return ::eReverseTimeStamp, but the time stamp will be set nevertheless!
  //! Not possible in case a virtual time channel is used.
  //! \param[in] timeStampNs time stamp in nanoseconds
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetTimeNs(INT64 timeStampNs) = 0;

  ///////////////////////////////////////////////////
  //! Set time stamp in seconds in current record
  //! \note the time stamp should be larger than time stamp of previous record!
  //! Depending on the configured TimeCheckMode (see SetTimeCheckMode), the method may detect reverse time stamps and return ::eReverseTimeStamp, but the time stamp will be set nevertheless!
  //! Not possible in case a virtual time channel is used.
  //! \param[in] timeStampSec time stamp in seconds
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetTimeSec(REAL timeStampSec) = 0;

  ///////////////////////////////////////////////////
  //! Write the raw value (given as double value) for the given channel
  //! \param[in]  pChannel channel to use for this operation
  //! \param[in]  value value to write
  //! \param[in]  isValid in case MDF4 invalidation bit is used for this channel,
  //!             the invalidation bit will be set for isValid = false, or reset if isValid = true (default).
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL WriteRawValueDouble(IChannel* pChannel, REAL value, bool isValid = true) = 0;

  ///////////////////////////////////////////////////
  //! Write the raw value (given as Integer value) for the given channel
  //!
  //! \param[in]  pChannel channel to use for this operation
  //! \param[in]  value value to write
  //! \param[in]  isValid in case MDF4 invalidation bit is used for this channel,
  //!             the invalidation bit will be set for isValid = false, or reset if isValid = true (default).
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL WriteRawValueInt64(IChannel* pChannel, INT64 value, bool isValid = true) = 0;

  ///////////////////////////////////////////////////
  //! Write the raw value (given as BYTE array) for the given channel
  //!
  //! \note for numeric values, the value bytes will be written!
  //! \param[in]  pChannel channel to use for this operation
  //! \param[in]  pByteArray buffer for the Byte array to write (must not be NULL)
  //! \param[in]  arraySize size of the input buffer in Bytes (can be 0 in order to set the data size for MDF4 MSLD/VLSD channels!)
  //! \param[in]  isValid in case MDF4 invalidation bit is used for this channel,
  //!             the invalidation bit will be set for isValid = false, or reset if isValid = true (default).
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL WriteRawValueByteArray(IChannel* pChannel, BYTE* pByteArray, UINT32 arraySize, bool isValid = true) = 0;

  ///////////////////////////////////////////////////
  //! Write the raw value (given as string) for the given channel
  //!
  //! \note
  //! <ul>
  //!   <li>Only possible for string data types.
  //!   <li>The written string will be transformed from the Unicode or MBCS input string, depending on the library.
  //!   <li>The written string always will be zero terminated. In case it is too long, it will be truncated.
  //! </ul>
  //! \param[in]  pChannel channel to use for this operation
  //! \param[in]  value string value to write (must not be NULL)
  //! \param[in]  isValid in case MDF4 invalidation bit is used for this channel,
  //!             the invalidation bit will be set for isValid = false, or reset if isValid = true (default).
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL WriteRawValueString(IChannel* pChannel, LPCTSTR value, bool isValid = true) = 0;

  ///////////////////////////////////////////////////
  //! Write the physical value (given as double value) for the given channel
  //! \note in case there is no conversion rule, the value will be written as raw value (1:1 conversion).
  //! If there is a conversion rule of other type than eLinear, an inverse conversion must be specified!
  //! \param[in]  pChannel channel to use for this operation
  //! \param[in]  value value to write
  //! \param[in]  isValid in case MDF4 invalidation bit is used for this channel,
  //!             the invalidation bit will be set for isValid = false, or reset if isValid = true (default).
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL WritePhysValueDouble(IChannel* pChannel, REAL value, bool isValid = true) = 0;

  ///////////////////////////////////////////////////
  //! Write the physical value (given as string) for the given channel
  //!
  //! \note
  //! <ul>
  //!   <li>Only possible if conversion returns a string or 1:1 conversion for string data types.
  //!   <li>The written string will be transformed from the Unicode or MBCS input string, depending on the library.
  //!   <li>The written string always will be zero terminated. In case it is too long, it will be truncated.
  //! </ul>
  //! \param[in]  pChannel channel to use for this operation
  //! \param[in]  value string value to write (must not be NULL)
  //! \param[in]  isValid in case MDF4 invalidation bit is used for this channel,
  //!             the invalidation bit will be set for isValid = false, or reset if isValid = true (default).
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL WritePhysValueString(IChannel* pChannel, LPCTSTR value, bool isValid = true) = 0;

  ///////////////////////////////////////////////////
  //! Get the currently active time check mode
  //! \see SetTimeCheckMode
  //! \note the default time check mode may be changed by configuration file
  //! \return current time check mode, see ::TimeCheckModeEnum
  virtual TimeCheckMode MDFLIBCALL GetTimeCheckMode() = 0;

  ///////////////////////////////////////////////////
  //! Change the time check mode
  //! \note the time check mode influences performance and return values of Flush/Write methods
  //! The initial time check mode can be changed via config file. By default it is ::eTimeCheckDisabled.
  //! \param mode time check mode to set, see ::TimeCheckModeEnum
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetTimeCheckMode(TimeCheckMode mode) = 0;

  ///////////////////////////////////////////////////
  //! Get the currently active flush check mode
  //! \see SetFlushCheckMode
  //! \note the default flush check mode may be changed by configuration file
  //! \return current flush check mode, see ::FlushCheckModeEnum
  virtual FlushCheckMode MDFLIBCALL GetFlushCheckMode() = 0;

  ///////////////////////////////////////////////////
  //! Change the flush check mode
  //! \note the flush check mode influences performance and return values of Flush/Write methods
  //! The initial flush check mode can be changed via config file. By default it is ::eFlushCheckDisabled.
  //! \param mode flush check mode to set, see ::FlushCheckModeEnum
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetFlushCheckMode(FlushCheckMode mode) = 0;

  ///////////////////////////////////////////////////
  //! Write parts of a record
  //!
  //! \note this method can be used to write the parts of the record not containing the reference to a VLSD channel or to write multiple values in one go, e.g. for arrays.
  //! For optimized performance the record data (and possibly invalidation) Bytes may be assembled in the calling application and written directly.
  //! <br>Any previously set time stamps or values and invalidation bits in the record can be overwritten.
  //! <br>Depending on the configured FlushCheckMode (see SetFlushCheckMode), the method may detect this and return ::eNoFlushSinceLastValue, but the record will be written nevertheless!
  //! <br>Depending on the configured TimeCheckMode (see SetTimeCheckMode), the method may detect reverse time stamps and return ::eReverseTimeStamp, but the record will be written nevertheless!
  //! <br>Some flush and time check modes will not work with this method
  //! \note this method cannot be used with write mode ::eAutoFlush (see SetWriteMode).
  //! \note min/max value statistics for the individual channels are not updated when using this method.
  //! \param[in]  offsetDataBytes offset from start of data bytes to write the bytes given in pDataBytes.
  //! \param[in]  pDataBytes buffer for the data Bytes to write
  //! \param[in]  dataByteCount size of the data Byte buffer (and, if specified pMaskDataBytes) in Bytes, i.e. number of bytes to write.
  //!             <br>Note: offsetDataBytes + dataByteCount must not exceed the number of data bytes (see IChannelGroup::GetDataByteCount).
  //! \param[in]  pMaskDataBytes if NULL, all bytes in pDataBytes are written unchanged.
  //!             <br>If specified, each byte pMaskDataBytes will be used as mask for the respective byte in pDataBytes: only bits set in the mask will be changed in the record.
  //!             <br>For example, a mask with value 0 (no bit set) will leave the respective byte unchanged in the record, whereas a mask with value 0x0F changes the lower 4 bits and leaves the upper 4 bits unchanged.
  //!             <br>The masks can be used to exclude bytes used by a VLSD channel, or to write bit channels or interleaving arrays.
  //! \param[in]  offsetInvalBytes offset from start of invalidation bytes to write the bytes given in pInvalBytes.
  //! \param[in]  pInvalBytes buffer for the invalidation Bytes to write (optional, MDF4 only). If NULL, none of the invalidation bits will be changed.
  //!             <br>Note: if a channel whose value is contained in the data bytes written by this method uses an invalidation bit, this bit must be reset with this method because all invalidation bits are set automatically when starting a new record (e.g. after Flush).
  //! \param[in]  invalByteCount size of the invalidation Byte buffer in Bytes, i.e. number of bytes to write
  //!             <br>Note: offsetInvalBytes + invalByteCount must not exceed the number of invalidation bytes (see IChannelGroup::GetInvalByteCount).
  //! \param[in]  pMaskInvalBytes if NULL, all bytes in pInvalBytes are written unchanged.
  //!             <br>If specified, each byte pMaskInvalBytes will be used as mask for the respective byte in pInvalBytes: only bits set in the mask will be changed in the record.
  //!             <br>For example, a mask with value 0 (no bit set) will leave the respective byte unchanged in the record, whereas a mask with value 0x0F changes the lower 4 bits and leaves the upper 4 bits unchanged.
  //!             <br>The mask can be used to leave invalidation bits of channels not contained in the data bytes unchanged.
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL WritePartsOfRecord(UINT32 offsetDataBytes, BYTE* pDataBytes, UINT32 dataByteCount, BYTE* pMaskDataBytes = NULL,
    UINT32 offsetInvalBytes = 0, BYTE* pInvalBytes = NULL, UINT32 invalByteCount = 0, BYTE* pMaskInvalBytes = NULL) = 0;

};
