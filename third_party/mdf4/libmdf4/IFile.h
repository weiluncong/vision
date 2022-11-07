/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IFile
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
//! \brief Definition of IFile

///////////////////////////////////////////////////
//! \brief Represents an open MDF file
//!
//! The IFile object represents an open MDF file. It gives access to the file information and its contents.
//! <br><br>
//! An MDF file usually contains one or more channel groups which build the tree hierarchy for the channels.
//! The IFile also allows to access the channels directly by using IFile::GetChannelSet which allows to collect the channels
//! of all channel groups into a set.
//! <br><br>
//! A file also can contain attachments, events and (MDF4 only) information about a channel hierarchy and a file history.
//! <br><br>
//! An MDF file can be sorted for faster (index-based) access to the signal data. A file is sorted if all of its (data) groups are sorted.
//! Even if the file is not sorted, it is still possible to access all information. There may be sorted channel groups in an unsorted file,
//! e.g. if the file is only considered unsorted due to an array channel using CG template (MDF4 feature, currently not supported).
//! <br><br>
//! Like for the IFileManager, for a IFile also log sinks can be registered to receive debug information. This is useful
//! to filter log entries for a specific file, e.g. to use a log sink for error messages for IFileManager, but to get more details for a
//! specific file.
interface IFile
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IFile* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //!
  //! \note If the last instance of IFile is released (return value = 0), implicitely Close() will be called.
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Register a LogSink for debug logging for this file
  //!
  //! All registered log sinks will receive the debug log messages for this file that are equal or higher to the specified
  //! minimum logging level (default: Info, i.e. all messages are logged)
  //! A LogSink usually will write the messages to a debug log file, but may also write them to the GUI or some
  //! other output.
  //! \note In contrast to IFileManager::RegisterLogSink, the log sinks registered for a file filter out all log messages which are
  //! global or for other files. This may be used, for instance, to log error messages for a file while the global log sink
  //! logs all messages.
  //! \param[in]  pLogSink log sink to be registered
  //! \param[in]  minimumLevel minimum logging level of messages given to log sink. This can be used to restrict logging only to warnings or errors.
  //!             Default: Info (i.e. all messages are logged)
  //! \return error code for the operation
  //! \see ILogSink, IFile::UnregisterLogSink
  virtual ErrorCode MDFLIBCALL RegisterLogSink(ILogSink* pLogSink, LogSinkLevel minimumLevel = eInfo) = 0;

  ///////////////////////////////////////////////////
  //! Unregister a LogSink for debug logging for this file
  //!
  //! Only log sinks previously registered can be unregistered.
  //! \note
  //! If a log sink has been registered several times, only the last inserted instance will be removed.
  //! \param[in]  pLogSink log sink to be unregistered
  //! \return error code for the operation
  //! \see ILogSink, IFile::RegisterLogSink
  virtual ErrorCode MDFLIBCALL UnregisterLogSink(ILogSink* pLogSink) = 0;

  ///////////////////////////////////////////////////
  //! Get file name
  //! \return name of the file without path
  virtual LPCTSTR MDFLIBCALL GetFileName() = 0;

  ///////////////////////////////////////////////////
  //! Get new reference to format info object of the file
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return interface pointer for IFormatInfo object
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IFormatInfo* MDFLIBCALL GetFormatInfo(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Check if file has been opened with write access
  //! \note this method only checks the mode used for IFile::Open, it does not check if a file is write protected on disc or not!
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return true if file has successfully been opened with write access, false otherwise
  virtual bool MDFLIBCALL HasWriteAccess(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Check if file is sorted, i.e. all groups are sorted
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return true if file is sorted, false otherwise or on error
  virtual bool MDFLIBCALL IsSorted(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get comment of file
  //! \note for MDF4 this can be XML (MD block) or plain text (TX block), for MDF3 it can only  be plain text
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IComment object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IComment* MDFLIBCALL GetComment(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get plain comment text
  //! \note for MDF3, Vector tools encode custom file comment fields into the comment text:
  //! use this method to get the comment text without the encoded fields
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with plain comment text, NULL on error
  virtual LPCTSTR MDFLIBCALL GetPlainCommentText(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get author as specified in file header
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with name of author, NULL on error
  virtual LPCTSTR MDFLIBCALL GetAuthor(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get department as specified in file header
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with department name, NULL on error
  virtual LPCTSTR MDFLIBCALL GetDepartment(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get project as specified in file header
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with project name, NULL on error
  virtual LPCTSTR MDFLIBCALL GetProject(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get subject as specified in file header
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with subject name, NULL on error
  virtual LPCTSTR MDFLIBCALL GetSubject(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get number of custom fields for file comment
  //! \note in addition to the four standard fields (author, department, project, subject),
  //! there can be additional fields for the file comment.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return number of additional comment fields, 0 on error or if no additional fields (check pErrorCode)
  virtual UINT64 MDFLIBCALL GetAdditionalCommentFieldCount(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get name of additional comment field for given index
  //! \see GetAdditionalCommentFieldCount
  //! \param[in]  index index of additional comment field
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with name of additional comment field, NULL on error
  virtual LPCTSTR MDFLIBCALL GetAdditionalCommentFieldName(UINT64 index, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get value of additional comment field for given index
  //! \see GetAdditionalCommentFieldCount
  //! \param[in]  index index of additional comment field
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with value of additional comment field, NULL on error
  virtual LPCTSTR MDFLIBCALL GetAdditionalCommentFieldValue(UINT64 index, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get info about creator of the file
  //! \note only available for MDF4, for MDF3 please check IFormatInfo::GetProgramIdentifier
  //! \see GetFormatInfo GetFileHistoryLog
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to ICreatorInfo object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ICreatorInfo* MDFLIBCALL GetCreatorInfo(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get new reference to start time stamp object of the file
  //!
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return interface pointer for ITimeStamp object
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ITimeStamp* MDFLIBCALL GetStartTime(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get start angle
  //! \note start angle is optional and not available for MDF3
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return start angle in radiants (0 on error or if not available)
  virtual REAL MDFLIBCALL GetStartAngle(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get start distance
  //! \note start distance is optional and not available for MDF3
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return start distance in meters (0 on error or if not available)
  virtual REAL MDFLIBCALL GetStartDistance(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get channel set for channels in file
  //! \param[in]  filterFlags flags for filtering, value is a bit combination of ::ChannelFilterFlagsEnum
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IChannelSet object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannelSet* MDFLIBCALL GetChannelSet(UINT32 filterFlags = 0, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get number of channel groups
  //! \see GetChannelGroup
  //! \note for unsorted MDF4, VLSD channel groups will not be considered
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return number of channel groups (0 on error)
  virtual UINT64 MDFLIBCALL GetChannelGroupCount(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get channel group for given index
  //! \see GetChannelGroupCount
  //! \note for unsorted MDF4, VLSD channel groups will not be considered
  //! \param[in]  index index of channel group
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IChannelGroup object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannelGroup* MDFLIBCALL GetChannelGroup(UINT64 index, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get number of attachments
  //! \note for MDF3, a PR block is treated as attachment. There can be at most one PR block per MDF3 file.
  //! \see GetAttachment
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return number of attachments (0 on error or if not available, e.g. for MDF3)
  virtual UINT64 MDFLIBCALL GetAttachmentCount(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get attachment for given index
  //! \note for MDF3, a PR block is treated as attachment. There can be at most one PR block per MDF3 file.
  //! \see GetAttachmentCount
  //! \param[in]  index index of attachment.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IAttachment object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IAttachment* MDFLIBCALL GetAttachment(UINT64 index, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get number of file history entries (FH block)
  //! \note only available for MDF4
  //! \see GetFileHistoryLog GetCreatorInfo
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return number of file history entries (0 for MDF3 or on error)
  virtual UINT64 MDFLIBCALL GetFileHistoryLogCount(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get file history entry for given index
  //! \note only available for MDF4
  //! \see GetFileHistoryLogCount GetCreatorInfo
  //! \param[in]  index index of file history entry. For index 0, the same entry will be returned as for GetCreatorInfo
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to ICreatorInfo object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ICreatorInfo* MDFLIBCALL GetFileHistoryLog(UINT64 index, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create a group data pointer
  //! \note A group data pointer synchronizes several channel groups by the given synchronization domain (default: time).
  //! \see IGroupDataPointer
  //!
  //! \note the extended method IFile::CreateGroupDataPointerEx allows to specifiy options for the created IGroupDataPointer
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IGroupDataPointer object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IGroupDataPointer* MDFLIBCALL CreateGroupDataPointer(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get file path
  //! \return full path name of the file
  virtual LPCTSTR MDFLIBCALL GetFilePath() = 0;

  ///////////////////////////////////////////////////
  //! Get access to factory for creating sharable MDF objects (IComment, IUnit, IConversion and ISourceInfo)
  //!
  //! \note only available for newly created or writable file.
  //!
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IFactory object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IFactory* MDFLIBCALL GetFactory(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set comment of file
  //! \note only available for newly created or writable file.
  //! \param[in] pComment pointer to IComment object, NULL to remove reference to comment object.
  //!            <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (cn_comment.XSD).
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetComment(IComment* pComment) = 0;

  ///////////////////////////////////////////////////
  //! Change start time stamp of the file
  //! \note only available for newly created or writable file.
  //!
  //! For a newly created files the start time stamp automatically was set to the current system date/time
  //! \param[in] pStartTime start time to set, NULL to set to current system time.
  //! \param[in] offsetNs offset in nanoseconds to add to the given or created time stamp
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetStartTimeCopy(ITimeStamp* pStartTime, INT64 offsetNs = 0) = 0;

  ///////////////////////////////////////////////////
  //! Change start time stamp of the file
  //! \note only available for newly created or writable file.
  //!
  //! When ::eLocalTime is specified, the user input has to INCLUDE the daylight saving time offset.
  //! \param[in] year,month,day,hour,minute,second,ns time in human readable form. Must include daylight saving time offset if it represents a local time.
  //! \param[in] timeStampType determines type of time stamp (local or UTC time). Also determines if the offsets for time zone and daylight saving time are valid.
  //! \param[in] timeZoneOffsetMin time zone offsets in minutes with respect to UTC time. Only valid if ::eUtcTimeWithOffsets or ::eLocalTimeWithOffsets is specified.
  //! \param[in] daylightSavingTimeOffsetMin daylight saving time offset in minutes. Only valid if ::eUtcTimeWithOffsets or ::eLocalTimeWithOffsets is specified.
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetStartTime(UINT32 year, UINT32 month, UINT32 day, UINT32 hour, UINT32 minute, UINT32 second, UINT32 ns = 0, TimeStampType timeStampType = eLocalTime, INT32 timeZoneOffsetMin = 0, INT32 daylightSavingTimeOffsetMin = 0) = 0;

  ///////////////////////////////////////////////////
  //! Change start time stamp of the file
  //! \note only available for newly created or writable file.
  //!
  //! \param[in] timeStampNs elapsed nanoseconds since midnight (00:00:00), January 1, 1970. This does not include any daylight saving time offset.
  //! \param[in] timeStampType determines type of time stamp (local or UTC time). Also determines if the offsets for time zone and daylight saving time are valid.
  //! \param[in] timeZoneOffsetMin time zone offsets in minutes with respect to UTC time. Only valid if ::eUtcTimeWithOffsets or ::eLocalTimeWithOffsets is specified.
  //! \param[in] daylightSavingTimeOffsetMin daylight saving time offset in minutes. Only valid if ::eUtcTimeWithOffsets or ::eLocalTimeWithOffsets is specified.
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetStartTimeNs(UINT64 timeStampNs, TimeStampType timeStampType = eUtcTime, INT32 timeZoneOffsetMin = 0, INT32 daylightSavingTimeOffsetMin = 0) = 0;

  ///////////////////////////////////////////////////
  //! Set start angle of file
  //! \note start angle is optional and not available for MDF3.
  //! \note only available for newly created or writable file.
  //!
  //! \param[in] startAngle start angle in radiants
  //! \param[in] isValid true if start angle is valid, otherwise false (for newly created file, start angle will be invalid by default)
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetStartAngle(REAL startAngle, bool isValid = true) = 0;

  ///////////////////////////////////////////////////
  //! Set start distance of file
  //! \note start distance is optional and not available for MDF3.
  //! \note only available for newly created or writable file.
  //!
  //! \param[in] startDistance start distance in meters
  //! \param[in] isValid true if start distance is valid, otherwise false (for newly created file, start distance will be invalid by default)
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetStartDistance(REAL startDistance, bool isValid = true) = 0;

  ///////////////////////////////////////////////////
  //! Create a new channel group in this file
  //!
  //! The new channel group will be appended to the channel group list of this file.
  //!
  //! \note only available for newly created or writable file.
  //!
  //! \param[in]  acquisitionName acquisition name of channel group.
  //! \param[in]  pAcquisitionSource pointer to ISourceInfo object to set as acquisition source for the channel group.
  //!             <br>Note: object must be in same file.
  //! \param[in]  pComment pointer to IComment object to set for the channel.
  //!             <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (cg_comment.XSD).
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     on success the IChannelGroup interface of the created channel group, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannelGroup* MDFLIBCALL CreateChannelGroup(LPCTSTR acquisitionName = NULL, ISourceInfo* pAcquisitionSource = NULL, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create an attachment
  //! \note only available for newly created or writable file.
  //!
  //! \note for MDF3, only a single attachment is possible and external or compressed attachments are not supported
  //!
  //! \param[in]  filePath name and path of the file to be attached
  //!             <br>Note: if no path or a relative path is specified, the folder of the MDF file is used as root folder.
  //!             <br>For MDF4, the file name will be saved:
  //!              <ul>
  //!                  <li>for embedded attachments, only the file name will be saved without path information.
  //!                  <li>for external attachments, the path as specified will be saved, which may be the file path relative to the MDF file or an absolute path.
  //!              </ul>
  //! \param[in]  mimeType MIME type string of the attachment.
  //! \param[in]  pComment comment to add to the attachment. Default: NULL
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema at_comment.xsd for the respective MDF version of the file.
  //! \param[in]  embedded if true, an embedded attachment will be created, otherwise an external, see ::AttachmentTypeEnum. Must be true for MDF3. Default: false
  //! \param[in]  compressionLevel level of compression for embedded attachment (0 = none, 1 = best speed, ..., 9 = best compression, -1 = default compression level). Must be 0 for MDF3. Default: 0
  //! \param[in]  calculateMD5 if true, an MD5 check sum will be calculated for the attachment (MDF4 only). Default: false
  //! \param[in]  pProgressCallback the operation may take some time, e.g. when calculating the MD4 check sum or when embedding the attachment.
  //!             For feedback (e.g. progress bar), a IProgressCallback interface pointer may be given which receives the progress feedback and which may
  //!             also be used to cancel the operation.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation. Default: NULL
  //! \return     on success the IAttachment interface of the created attachment, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IAttachment* MDFLIBCALL CreateAttachment(LPCTSTR filePath, LPCTSTR mimeType, IComment* pComment = NULL,
                                                   bool embedded = false, int compressionLevel = 0, bool calculateMD5 = false,
                                                   IProgressCallback* pProgressCallback = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create an embedded attachment from a buffer
  //! \note only available for newly created or writable file.
  //!
  //! \note for MDF3, only a single attachment is possible and compressed attachments are not supported (error code will be set to eNotAvailable)
  //!
  //! \param[in]  pBuffer buffer containing the data to attach
  //! \param[in]  bufferSize number of Bytes in pBuffer to attach
  //! \param[in]  mimeType MIME type string of the attachment.
  //! \param[in]  pComment comment to add to the attachment. Default: NULL
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema at_comment.xsd for the respective MDF version of the file.
  //! \param[in]  compressionLevel level of compression: 0 = none, 1 = best speed, ..., 9 = best compression, -1 = default compression level). Must be 0 for MDF3.
  //! \param[in]  calculateMD5 if true, an MD5 check sum will be calculated for the attachment
  //! \param[in]  pProgressCallback the operation may take some time, e.g. when calculating the MD4 check sum or when embedding the attachment.
  //!             For feedback (e.g. progress bar), a IProgressCallback interface pointer may be given which receives the progress feedback and which may
  //!             also be used to cancel the operation.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation. Default: NULL
  //! \return     on success the IAttachment interface of the created attachment, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IAttachment* MDFLIBCALL CreateEmbeddedAttachment(BYTE* pBuffer, UINT32 bufferSize, LPCTSTR mimeType, IComment* pComment = NULL,
                                                           int compressionLevel = 0, bool calculateMD5 = false,
                                                           IProgressCallback* pProgressCallback = NULL, ErrorCode* pErrorCode = NULL) = 0;


  ///////////////////////////////////////////////////
  //! For a newly created or changed file, information about the writing application can be written to the file history (latest ICreatorInfo)
  //!
  //! This information will be written to the file history entry created for the new/changed file.
  //! If this function is not called, the fields of the file history entry are filled with the default information specified below for each argument.
  //! \note not available for MDF3, only available for newly created or writable file.
  //!
  //! \param[in]  appName string with name of the writing application. If NULL or empty, the base name of the MDF4 Lib DLL will be used, e.g. "MDF4Lib".
  //! \param[in]  appVersion version string of the writing application in the format A.B or A.B.C or A.B.C.D. If NULL or empty or no appName specified, the version number of the MDF4 Lib will be used.
  //! \param[in]  vendorName vendor name of the writing application. If NULL or empty or no appName specified, the vendor name of the MDF4 Lib will be used.
  //! \param[in]  userName user name. If NULL or empty, the Windows login name will be used.
  //! \param[in]  comment main comment text for file history entry. If NULL or empty, a default text created by MDF4 Lib is used.
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetFileHistoryInfo(LPCTSTR appName, LPCTSTR appVersion, LPCTSTR vendorName, LPCTSTR userName = NULL, LPCTSTR comment = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Check if there is any channel group that contains a bus event (bus logging for MDF 4.1)
  //! \note for MDF version >= 4.1 only a flag of the channel group is checked, for earlier versions this is not supported
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return true if there is at least one channel group that contains a bus event, false otherwise or on error
  virtual bool MDFLIBCALL ContainsBusEvent(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get the type of bus events contained in the file
  //! \note for MDF version >= 4.1 only flags of the channel groups are checked, for earlier versions this is not supported yet.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return type of the contained bus events (see ::BusEventTypeEnum), or ::eBusEventUnspec in case of an error or if not supported.
  virtual BusEventType MDFLIBCALL GetBusEventType(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get access to IFileSizeObserver object
  //!
  //! \note only available for newly created or writable file, not available while an IDataWriter object exists.
  //!
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IFileSizeObserver object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IFileSizeObserver* MDFLIBCALL GetFileSizeObserver(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set a limit for a specific limit type
  //!
  //! \param[in] type type of limit to be set. See ::LimitTypeEnum for details.
  //! \param[in] limitSize size of limit in Bytes. If 0 no limit will be used, if < 0 then the default value for the respective limit type will be used.
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetLimit(LimitType type, INT32 limitSize) = 0;

  ///////////////////////////////////////////////////
  //! Delete an attachment from global list of attachments
  //! \see GetAttachmentCount
  //! \note the attachment can only be deleted if currently no IAttachment object exists for it
  //! This method does not support future MDF versions.
  //! It only deletes the attachment references, but does not decrease the file size.
  //! \param[in]  index index of attachment
  //! \param[in]  options bit combination of option flags, see ::DeleteAttachmentFlagsEnum
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL DeleteAttachment(UINT64 index, UINT32 options = 0) = 0;

  ///////////////////////////////////////////////////
  //! Log a message to all log sinks registered for this file
  //!
  //! This method can be used to log own log messages from client code.
  //! \note
  //! In contrast to IFileManager::LogMessage, only log sinks previously registered by IFile::RegisterLogSink will receive the message.
  //! \param[in]  logLevel logging level for this message
  //! \param[in]  message the message to write
  //! \return     error code for the operation
  //! \see ILogSink, IFile::RegisterLogSink
  virtual ErrorCode MDFLIBCALL LogMessage(LogSinkLevel logLevel, LPCTSTR message) = 0;

  ///////////////////////////////////////////////////
  //! Get event set for events in file
  //! \note The resulting IEventSet object offers further filtering of the events.
  //! \param[in]  filterFlags flags for filtering, value is a bit combination of ::EventFilterFlagsEnum
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IEventSet object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IEventSet* MDFLIBCALL GetEventSet(UINT32 filterFlags = 0, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create a new event in this file
  //!
  //! \note only available for newly created or writable file.
  //! \note not yet available for MDF3
  //!
  //! The new event will be appended to the event list of this file.
  //! By default, the new event has a global scope, i.e. applies to all channels in the file. This can be changed by IEvent::SetScope().
  //!
  //!
  //! \param[in]  type type of the event, see ::EventTypeEnum. For MDF3, only event type ::eTrigger is possible.
  //! \param[in]  cause cause of the event, see ::EventCauseEnum. Will be ignored for MDF3.
  //! \param[in]  name name of the event, can be NULL if there is no name. Will be ignored for MDF3.
  //! \param[in]  timeInSeconds time in seconds when the event occurred (relative to start time of file)
  //!             <br>Note: use CreateEventEx() to specify baseSync and syncFactor
  //! \param[in]  pComment pointer to IComment object to set for the event.
  //!             <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (ev_comment.XSD).
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     on success the IEvent interface of the created event, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IEvent* MDFLIBCALL CreateEvent(EventType type,
                                         EventCause cause,
                                         LPCTSTR name,
                                         REAL timeInSeconds,
                                         IComment* pComment = NULL,
                                         ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create a new event in this file
  //!
  //! \note only available for newly created or writable file.
  //! \note not yet available for MDF3
  //!
  //! The new event will be appended to the event list of this file.
  //! By default, the new event has a global scope, i.e. applies to all channels in the file. This can be changed by IEvent::SetScope().
  //!
  //! \param[in]  type type of the event, see ::EventTypeEnum. For MDF3, only event type ::eTrigger is possible.
  //! \param[in]  cause cause of the event, see ::EventCauseEnum. Will be ignored for MDF3.
  //! \param[in]  name name of the event, can be NULL if there is no name. Will be ignored for MDF3.
  //! \param[in]  syncBase base for sync value. The sync value is calculated by multiplication of syncBase with syncFactor and has a unit according to syncType.
  //!             <br>The sync value is when the event occurred (relative to start sync value of file). Typically time is used for synchronization.
  //!             <br>For example, the raw value of the time channel (e.g. nanoseconds) can be used as syncBase whereas syncFactor is used to convert it to seconds.
  //!             This allows a higher precision than only using double values. See also IEvent::GetSyncBaseValue() and IEvent::GetSyncFactor().
  //! \param[in]  syncFactor factor for sync value. syncFactor must not be zero, except if syncBase is one (e.g. to time value in seconds as REAL value). See also above.
  //! \param[in]  syncType type of synchronization with other events and master channels of channel groups, see ::SyncTypeEnum.
  //!             <br>Only values ::eTimeSync, ::eAngleSync, ::eDistanceSync are allowed.
  //!             <br>Index-based events (sync type ::eIndexSync) must be created for a channel group or channel scope.
  //! \param[in]  pComment pointer to IComment object to set for the event.
  //!             <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (ev_comment.XSD).
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return     on success the IEvent interface of the created event, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IEvent* MDFLIBCALL CreateEventEx(EventType type,
                                         EventCause cause,
                                         LPCTSTR name,
                                         INT64 syncBase,
                                         REAL syncFactor,
                                         SyncType syncType = eTimeSync,
                                         IComment* pComment = NULL,
                                         ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Close the file handle for the file
  //!
  //! \note After calling this method, no read or write operations (also for contained objects) will be possible any longer!
  //! For a file opened with write access, the file will be flushed before closing it, also the IDataWriter objects.
  //! Close will be called internally when the IFile object is released (see Release()),
  //! so an explicit call to Close may only be necessary when not all MDF4 Lib objects for this IFile object are released yet
  //! (for instance when using .NET interface and not Dispose is called for each MDF4 Lib object).
  //! \return     error code for the operation
  virtual ErrorCode MDFLIBCALL Close() = 0;

  ///////////////////////////////////////////////////
  //! Get properties (including name and value) of a standard comment field
  //! \see GetAuthor(), GetDepartment(), GetProject(), GetSubject()
  //! \param[in]  type specifies the standard field type for reading the properties, see StandardFieldTypeEnum
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return structure with properties, see IMDF4LibStructs::FieldProperties. In case of error, name is NULL.
  virtual IMDF4LibStructs::FieldProperties MDFLIBCALL GetStandardCommentFieldProperties(StandardFieldType type, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get properties (including name and value) of additional comment field for given index
  //! \see GetAdditionalCommentFieldCount
  //! \param[in]  index index of additional comment field, see GetAdditionalCommentFieldCount()
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return structure with properties, see IMDF4LibStructs::FieldProperties. In case of error, name is NULL.
  virtual IMDF4LibStructs::FieldProperties MDFLIBCALL GetAdditionalCommentFieldProperties(UINT64 index, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set comment text and comment fields of file
  //! \note only available for newly created or writable file. The previous comment will be be replaced (like for SetComment()).
  //! \param[in] comment string with plain comment text, can be NULL (see options). See GetPlainCommentText().
  //! \param[in] author string with text for author, can be NULL (see options). See GetAuthor().
  //! \param[in] department string with text for department, can be NULL (see options). See GetDepartment().
  //! \param[in] project string with text for project, can be NULL (see options). See GetProject().
  //! \param[in] subject string with text for subject, can be NULL (see options). See GetSubject().
  //! \param[in] pAdditionalCommentFields array with additional comment fields and their properties, can be NULL. See GetAdditionalCommentFieldProperties().
  //! \param[in] arrayElements number of elements in pAdditionalCommentFields, can be 0 if empty
  //! \param[in] options bit flags for merging existing information with new information, see ::CommentMergeFlagsEnum.
  //!            <br>Attributes in MDF4 are only replaced for an already existing additional field
  //!            if the attribute value is not NULL (for unit, desc, lang) or ::eFieldDataTypeUnspec (for type).
  //!            <br>For standard fields: if the string pointer for a standard field is NULL (e.g. author = NULL) and
  //!            <ul>
  //!            <li>if ::eMergeCommentFields flag is set, the respective standard field will not be changed. </li>
  //!            <li>if ::eMergeCommentFields flag is not set, the respective standard field will be removed (or set to empty text for MDF 3.x)<li>
  //!            </ul>
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetCommentTextAndFields(LPCTSTR comment, LPCTSTR author, LPCTSTR department, LPCTSTR project, LPCTSTR subject,
    IMDF4LibStructs::FieldProperties* pAdditionalCommentFields = NULL, UINT32 arrayElements = 0, UINT16 options = eKeepExstingInformation) = 0;

  ///////////////////////////////////////////////////
  //! Create a group data pointer
  //! \note A group data pointer synchronizes several channel groups by the given synchronization domain (default: time).
  //! \see IGroupDataPointer
  //!
  //! \param[in]  options bit combination of option flags, see ::DataPointerOptionsFlagsEnum
  //!             <br>Currently, only the following flags are allowed: ::eUseReadBuffer, ::eDisableStatusTabWarning, ::eDisableVerbalTableError
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IGroupDataPointer object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IGroupDataPointer* MDFLIBCALL CreateGroupDataPointerEx(UINT32 options, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Search the first and/or last (relative) time stamp of all channel groups in ns
  //!
  //! \note: in addition or instead of the time stamp itself, the index of the respective channel group can be retrieved. See GetChannelGroup.
  //! This method does not consider the stored min/max values of the time channel. Please consider using IFile::GetFirstAndLastTimeStampNsEx which allows more options.
  //! Hint: in order to convert the relative time to an absolute time, get ITimeStamp with GetStartTime and use ITimeStamp::GetTimeStringForRelativeTimeNs or ITimeStamp::GetTimePartsForRelativeTimeNs
  //! \param[out] pFirstNs if not NULL the first (lowest) relative time stamp in ns will be returned for all channel groups in the file which are not contained in the exclude list.
  //! \param[out] pLastNs  if not NULL the last (highest) relative time stamp in ns will be returned for all channel groups in the file which are not contained in the exclude list.
  //! \param[out] pCgIndexFirst if not NULL the index of the channel group that contains the first (lowest) relative time stamp will be returned for all channel groups in the file which are not contained in the exclude list.
  //!             <br>In case several channel groups contain the same first (lowest) time stamp, the group with the lowest index will be returned.
  //! \param[out] pCgIndexLast if not NULL the index of the channel group that contains the last (highest) relative time stamp will be returned for all channel groups in the file which are not contained in the exclude list.
  //!             <br>In case several channel groups contain the same last (highest) time stamp, the group with the lowest index will be returned.
  //! \param[in]  ppExcludeList if not NULL this is a list of pointers to IChannelGroup objects which should be excluded for the search (exclude list). Can be NULL.
  //!             <br>Note: all channel group objects must be in the same file
  //! \param[in]  excludeListCount size of exclude list, i.e. number of channel group objects to exclude from search. Can be 0.
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL GetFirstAndLastTimeStampNs(INT64* pFirstNs, INT64* pLastNs = NULL,
    UINT64* pCgIndexFirst = NULL, UINT64* pCgIndexLast = NULL, IChannelGroup** ppExcludeList = NULL, UINT32 excludeListCount = 0) = 0;

  ///////////////////////////////////////////////////
  //! Get name of time source as specified in file header (optional information, only available for MDF 3.2 or higher)
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with name of time source, NULL on error or if not available
  virtual LPCTSTR MDFLIBCALL GetTimeSourceName(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get class of time source as specified in file header (only available for MDF 3.2 or higher)
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return value of time source class, see ::TimeSrcClassEnum, ::eTimeSrcClassUnspec on error or if not available
  virtual TimeSrcClass MDFLIBCALL GetTimeSourceClass(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set class and name of time source as specified in file header (only available for MDF 3.2 or higher)
  //! \note only available for newly created or writable file.
  //! \param[in] timeSourceClass class of time source, see ::TimeSrcClassEnum (if ::eTimeSrcClassUnspec, the existing class will not be changed)
  //! \param[in] timeSourceName name of time source (if NULL, the existing name will not be changed).<br>
  //!            <b>Note</b>:
  //!            <br>For MDF4 the name is encoded in file comment XML. Thus changing the file comment afterwards with IFile::SetComment
  //!            (or IFile::SetCommentTextAndFields without option bit ::ePreserveAdditionalInformation) may over write/remove the name!
  //!            <br>For MDF3 the time source name is restricted to 32 characters and is saved in ASCII characters (i.e. 32 <= ASCII code <= 126, no extended ASCII codes).
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetTimeSourceClassAndName(TimeSrcClass timeSourceClass, LPCTSTR timeSourceName) = 0;

  ///////////////////////////////////////////////////
  //! Search the first and/or last (relative) time stamp of all channel groups in ns
  //!
  //! This method offers more options compared to IFile::GetFirstAndLastTimeStampNs. In contrast to IFile::GetFirstAndLastTimeStampNs, this method considers the min/max values stored in time channel or its conversion rule.
  //! The behavior can be adapted by parameter 'options'.
  //! \note: in addition or instead of the time stamp itself, the index of the respective channel group can be retrieved. See GetChannelGroup.
  //! Hint: in order to convert the relative time to an absolute time, get ITimeStamp with GetStartTime and use ITimeStamp::GetTimeStringForRelativeTimeNs or ITimeStamp::GetTimePartsForRelativeTimeNs
  //! \param[out] pFirstNs if not NULL the first (lowest) relative time stamp in ns will be returned for all channel groups in the file which are not contained in the exclude list.
  //! \param[out] pLastNs  if not NULL the last (highest) relative time stamp in ns will be returned for all channel groups in the file which are not contained in the exclude list.
  //! \param[out] pCgIndexFirst if not NULL the index of the channel group that contains the first (lowest) relative time stamp will be returned for all channel groups in the file which are not contained in the exclude list.
  //!             <br>In case several channel groups contain the same first (lowest) time stamp, the group with the lowest index will be returned.
  //! \param[out] pCgIndexLast if not NULL the index of the channel group that contains the last (highest) relative time stamp will be returned for all channel groups in the file which are not contained in the exclude list.
  //!             <br>In case several channel groups contain the same last (highest) time stamp, the group with the lowest index will be returned.
  //! \param[in]  options bit combination of option flags, see ::GetFirstAndLastTimeStampOptionsFlagsEnum
  //! \param[in]  ppExcludeList if not NULL this is a list of pointers to IChannelGroup objects which should be excluded for the search (exclude list). Can be NULL.
  //!             <br>Note: all channel group objects must be in the same file
  //! \param[in]  excludeListCount size of exclude list, i.e. number of channel group objects to exclude from search. Can be 0.
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL GetFirstAndLastTimeStampNsEx(INT64* pFirstNs, INT64* pLastNs = NULL,
    UINT64* pCgIndexFirst = NULL, UINT64* pCgIndexLast = NULL, UINT32 options = 0, IChannelGroup** ppExcludeList = NULL, UINT32 excludeListCount = 0) = 0;


};
