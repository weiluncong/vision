/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: ITimeStamp
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
//! \brief Definition of ITimeStamp

///////////////////////////////////////////////////
//! \brief time stamp of a file or a file history log
//!
//! The ITimeStamp object contains information about a time stamp.
interface ITimeStamp
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ITimeStamp* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //!
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Get the time stamp in nanoseconds
  //! \see IsLocalTime, AreOffsetsValid
  //! \param[in]  localTime if true, the local time stamp will be returned, otherwise the UTC time stamp (if available).<br>
  //!             If offsets are missing and localTime is true, the offsets of the running system are used to determine the local time.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     time stamp in ns elapsed since 00:00:00 01.01.1970 (UTC time or local time, depending on localTime argument)
  //!             <BR>0 on error
  virtual UINT64 MDFLIBCALL GetTimeStampNs(bool localTime, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get a formatted string with time stamp
  //! The full format will be <code>YYYY-MM-DD hh-mm-ss.nnnnn GMT</code> where
  //! <ul>
  //! <li><code>YYYY</code> is the year,
  //! <li><code>MM</code> is the month number,
  //! <li><code>DD</code> is the day,
  //! <li><code>ff</code> is the hour (24 hr system),
  //! <li><code>mm</code> is the minute,
  //! <li><code>ss</code> is the second,
  //! <li><code>nnnnn</code> is the nanosecond (number of decimal places given in decimalPlacesForNs)
  //! <li><code>GMT</code> is the time zone offset, e.g. <code>GMT+1</code>, <code>GMT</code> or <code>GMT-4</code>
  //! </ul>
  //! \note the returned string pointer will only be valid during lifetime of this object and until next call to a GetTimeString method of this object.
  //! So we strongly recommend to immediately store the string pointer into some string variable.
  //! \param[in]  localTime if true, the local time stamp will be returned, otherwise the UTC time stamp (if available).<br>
  //!             If offsets are missing and localTime is true, the offsets of the running system are used to determine the local time.
  //! \param[in]  addDate if true, not only the time but also the date will be contained in the formatted string
  //! \param[in]  addTimeZone if true, the time zone information will be appended
  //! \param[in]  precisionForNs number of decimal places for nanoseconds (max 9). 0 if nanoseconds should be omitted.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     pointer to formatted string or NULL on error
  virtual LPCTSTR MDFLIBCALL GetTimeString(bool localTime = true, bool addDate = true, bool addTimeZone = false, BYTE precisionForNs = 0, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get single parts of the time stamp
  //! \param[in]  localTime if true, the local time stamp will be returned, otherwise the UTC time stamp (if available).<br>
  //!             If offsets are missing and localTime is true, the offsets of the running system are used to determine the local time.
  //! \param[out] pYear if not NULL, this argument will receive the year number, e.g. 2011
  //! \param[out] pMonth if not NULL, this argument will receive the month number (1-12)
  //! \param[out] pDay if not NULL, this argument will receive the day number (1-31)
  //! \param[out] pHour if not NULL, this argument will receive the hour number (0-23)
  //! \param[out] pMinute if not NULL, this argument will receive the minute number (0-59)
  //! \param[out] pSecond if not NULL, this argument will receive the second number (0-59)
  //! \param[out] pNs if not NULL, this argument will receive the nanosecond number (0-999999999)
  //! \return     error code of the operation
  virtual ErrorCode MDFLIBCALL GetTimeParts(bool localTime, UINT32* pYear = NULL, UINT32* pMonth = NULL, UINT32* pDay = NULL, UINT32* pHour = NULL, UINT32* pMinute = NULL, UINT32* pSecond = NULL, UINT32* pNs = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get the time zone offset in minutes
  //! \note Only valid if AreOffsetsValid returns true
  //! The value is in range [-720,720], i.e. it can be negative!
  //! For example a value of 60 (min) means UTC+1 time zone = Central European Time (CET).
  //! \see AreOffsetsValid
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return time zone offset in minutes or 0 in case of error or if time zone offset is invalid/unknown
  virtual INT32 MDFLIBCALL GetTimeZoneOffset(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get the daylight saving time (DST) offset in minutes
  //! \note Only valid if AreOffsetsValid returns true
  //! During the summer months, most regions observe a DST offset of 60 min (1 hour).
  //! \see AreOffsetsValid
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return daylight saving time offset in minutes or 0 in case of error or if time zone offset is invalid/unknown
  virtual INT32 MDFLIBCALL GetDaylightSavingTimeOffset(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Request if time stamp is local time
  //!
  //! If true, the start time stamp in nanoseconds represents the local time instead of the UTC time
  //! \see GetTimeStampNs
  //! \return true if time stamp is local time, otherwise false
  virtual bool MDFLIBCALL IsLocalTime() = 0;

  ///////////////////////////////////////////////////
  //! Request if time zone and daylight saving time offsets are valid
  //! \see GetFlags, GetTimeZoneOffset, GetDaylightSavingTimeOffset
  //! \return true if both offsets are valid, otherwise false
  virtual bool MDFLIBCALL AreOffsetsValid() = 0;

  ///////////////////////////////////////////////////
  //! Get time flags
  //! \see ::TimeFlagsEnum, IsLocalTime, AreOffsetsValid
  //! \return flags of the time stamp
  virtual UINT32 MDFLIBCALL GetFlags() = 0;

  ///////////////////////////////////////////////////
  //! Get a formatted string with absolute time stamp for a relative time given as offset in nanoseconds
  //! The full format will be <code>YYYY-MM-DD hh-mm-ss.nnnnn GMT</code> where
  //! <ul>
  //! <li><code>YYYY</code> is the year,
  //! <li><code>MM</code> is the month number,
  //! <li><code>DD</code> is the day,
  //! <li><code>ff</code> is the hour (24 hr system),
  //! <li><code>mm</code> is the minute,
  //! <li><code>ss</code> is the second,
  //! <li><code>nnnnn</code> is the nanosecond (number of decimal places given in decimalPlacesForNs)
  //! <li><code>GMT</code> is the time zone offset, e.g. <code>GMT+1</code>, <code>GMT</code> or <code>GMT-4</code>
  //! </ul>
  //! \note the returned string pointer will only be valid during lifetime of this object and until next call to a GetTimeString method of this object.
  //! So we strongly recommend to immediately store the string pointer into some string variable.
  //! \param[in]  offsetNs relative time in ns, i.e. offset to be added to the absolute time represented by this object. The ITimeStamp object will not be changed!
  //! \param[in]  localTime if true, the local time stamp will be returned, otherwise the UTC time stamp (if available).<br>
  //!             If offsets are missing and localTime is true, the offsets of the running system are used to determine the local time.
  //! \param[in]  addDate if true, not only the time but also the date will be contained in the formatted string
  //! \param[in]  addTimeZone if true, the time zone information will be appended
  //! \param[in]  precisionForNs number of decimal places for nanoseconds (max 9). 0 if nanoseconds should be omitted.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     pointer to formatted string or NULL on error
  virtual LPCTSTR MDFLIBCALL GetTimeStringForRelativeTimeNs(INT64 offsetNs, bool localTime = true, bool addDate = true, bool addTimeZone = false, BYTE precisionForNs = 0, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get a formatted string with absolute time stamp for a relative time given as offset in seconds
  //! The full format will be <code>YYYY-MM-DD hh-mm-ss.nnnnn GMT</code> where
  //! <ul>
  //! <li><code>YYYY</code> is the year,
  //! <li><code>MM</code> is the month number,
  //! <li><code>DD</code> is the day,
  //! <li><code>ff</code> is the hour (24 hr system),
  //! <li><code>mm</code> is the minute,
  //! <li><code>ss</code> is the second,
  //! <li><code>nnnnn</code> is the nanosecond (number of decimal places given in decimalPlacesForNs)
  //! <li><code>GMT</code> is the time zone offset, e.g. <code>GMT+1</code>, <code>GMT</code> or <code>GMT-4</code>
  //! </ul>
  //! \note the returned string pointer will only be valid during lifetime of this object and until next call to a GetTimeString method of this object.
  //! So we strongly recommend to immediately store the string pointer into some string variable.
  //! \param[in]  offsetSec relative time in seconds, i.e. offset to be added to the absolute time represented by this object. The ITimeStamp object will not be changed!
  //! \param[in]  localTime if true, the local time stamp will be returned, otherwise the UTC time stamp (if available).<br>
  //!             If offsets are missing and localTime is true, the offsets of the running system are used to determine the local time.
  //! \param[in]  addDate if true, not only the time but also the date will be contained in the formatted string
  //! \param[in]  addTimeZone if true, the time zone information will be appended
  //! \param[in]  precisionForNs number of decimal places for nanoseconds (max 9). 0 if nanoseconds should be omitted.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     pointer to formatted string or NULL on error
  virtual LPCTSTR MDFLIBCALL GetTimeStringForRelativeTimeSec(REAL offsetSec, bool localTime = true, bool addDate = true, bool addTimeZone = false, BYTE precisionForNs = 0, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get single parts of the time stamp for a relative time given as offset in nanoseconds
  //! \param[in]  offsetNs relative time in ns, i.e. offset to be added to the absolute time represented by this object. The ITimeStamp object will not be changed!
  //! \param[in]  localTime if true, the local time stamp will be returned, otherwise the UTC time stamp (if available).<br>
  //!             If offsets are missing and localTime is true, the offsets of the running system are used to determine the local time.
  //! \param[out] pYear if not NULL, this argument will receive the year number, e.g. 2011
  //! \param[out] pMonth if not NULL, this argument will receive the month number (1-12)
  //! \param[out] pDay if not NULL, this argument will receive the day number (1-31)
  //! \param[out] pHour if not NULL, this argument will receive the hour number (0-23)
  //! \param[out] pMinute if not NULL, this argument will receive the minute number (0-59)
  //! \param[out] pSecond if not NULL, this argument will receive the second number (0-59)
  //! \param[out] pNs if not NULL, this argument will receive the nanosecond number (0-999999999)
  //! \return     error code of the operation
  virtual ErrorCode MDFLIBCALL GetTimePartsForRelativeTimeNs(INT64 offsetNs, bool localTime, UINT32* pYear = NULL, UINT32* pMonth = NULL, UINT32* pDay = NULL, UINT32* pHour = NULL, UINT32* pMinute = NULL, UINT32* pSecond = NULL, UINT32* pNs = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get single parts of the time stamp for a relative time given as offset in seconds
  //! \param[in]  offsetSec relative time in seconds, i.e. offset to be added to the absolute time represented by this object. The ITimeStamp object will not be changed!
  //! \param[in]  localTime if true, the local time stamp will be returned, otherwise the UTC time stamp (if available).<br>
  //!             If offsets are missing and localTime is true, the offsets of the running system are used to determine the local time.
  //! \param[out] pYear if not NULL, this argument will receive the year number, e.g. 2011
  //! \param[out] pMonth if not NULL, this argument will receive the month number (1-12)
  //! \param[out] pDay if not NULL, this argument will receive the day number (1-31)
  //! \param[out] pHour if not NULL, this argument will receive the hour number (0-23)
  //! \param[out] pMinute if not NULL, this argument will receive the minute number (0-59)
  //! \param[out] pSecond if not NULL, this argument will receive the second number (0-59)
  //! \param[out] pNs if not NULL, this argument will receive the nanosecond number (0-999999999)
  //! \return     error code of the operation
  virtual ErrorCode MDFLIBCALL GetTimePartsForRelativeTimeSec(REAL offsetSec, bool localTime, UINT32* pYear = NULL, UINT32* pMonth = NULL, UINT32* pDay = NULL, UINT32* pHour = NULL, UINT32* pMinute = NULL, UINT32* pSecond = NULL, UINT32* pNs = NULL) = 0;

};
