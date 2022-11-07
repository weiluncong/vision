/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IFileManager
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
//! \brief Definition of IFileManager

///////////////////////////////////////////////////////////////////////////////////////////////
/*!
\brief File manager for opening a file

The file manager is the root for opening MDF files with MDF4Lib.
It also returns general information about MDF4Lib and allows to register
so-called log sinks for creating debug log output.
\see ILogSink, IFileManager::RegisterLogSink

The file manager will be returned by the exported GetFileManager() function.
Find below an example how to get the IFileManager interface.
Alternatively, use the class VMdf4Lib provided in MdfUse.h which encapsulates the complete loading and verification of
the library and gives convenient access to the IFileManager.
<br>
Note: calling GetFileManager multiple times with exactly the same license information will not create a new instance, but re-use the old one.
However, if a different license is used, a new instance will be created and sharing of IFile objects between the instances is not possible.
<br>
<br>
When calling IFileManager::OpenFile() for the same file path in the <b>same</b> IFileManager instance, the already existing IFile object may be returned
depending on the parameters of method IFileManager::OpenFile(). In such a case, the method retunrs as error code ::eFileAlreadyOpen instead of ::eOK.
<br>
<br>
By default, all instances of IFileManager write to the same global log file which can be activated via a config file.
To alter this behavior so that each instance creates an own log file, parameter "addFID" must be specified in the config file, see \ref diagnostics.
*/
#ifndef LINUX
/*!
\code

#include "MDF4LibAPI.h"

HANDLE hMdf4Lib = ::LoadLibrary(MDF4LIB_DLL_NAME); // possibly path to DLL must be specified, too!
if (!hMdf4Lib) {
  return false;
}

// licensing information as delivered with purchase of the library
LPCTSTR customerName = _T("...");
LPCTSTR serialNumber = _T("...");
LPCTSTR regKey       = _T("...");

MDF4Lib::GetFileManagerPtr GetFileManager = (MDF4Lib::GetFileManagerPtr)::GetProcAddress(hMdf4Lib, "GetFileManager");
IFileManager* pFileManager = GetFileManager ? GetFileManager(customerName, serialNumber, regKey) : NULL;
if (pFileManager)
{
  // ... work with file manager
}
// any open files or resources will be released automatically
::FreeLibrary(hMdf4Lib);
\endcode

*/
#else   // LINUX
/*!
\code
#include <dlfcn.h>    // For dlopen, dlsym, dlclose
#include <libmdf4/MDF4LibAPI.h>

void* hMdf4Lib = dlopen(MDF4LIB_DLL_NAME, RTLD_LAZY); // possibly path to shared library must be specified, too!
if (hMdf4Lib == nullptr) {
  return false;
}

// licensing information as delivered with purchase of the library
LPCTSTR customerName = "...";
LPCTSTR serialNumber = "...";
LPCTSTR regKey       = "...";

MDF4Lib::GetFileManagerPtr GetFileManager = (MDF4Lib::GetFileManagerPtr)dlsym(hMdf4Lib, "GetFileManager");
IFileManager* pFileManager = GetFileManager ? GetFileManager(customerName, serialNumber, regKey) : nullptr;
if (pFileManager != nullptr)
{
  // ... work with file manager
}

// any open files or resources will be released automatically
dlclose(hMdf4Lib);
\endcode

*/
#endif  // LINUX

interface IFileManager
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IFileManager* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //!
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Opens the specified file
  //!
  //! An opened file will be closed automatically when the IFile object and all child objects have been released
  //! \see IFile::Release
  //! \note
  //! If the file already is open and has been opened with equal flags,
  //! the method simply returns the IFile interface of the already open file
  //! and pErrorCode will indicate that the file already was open (eFileAlreadyOpen)
  //! In this case, the reference counter of the file will be incremented, so make sure
  //! that the IFile::Release method will be called for this instance (best to use the provided smart pointer class)
  //! \note when opening the file with a mode that locks the file for reading by others, performance for file operations will decrease!
  //! \param[in]  filePath file path and name of the file to open
  //! \param[out] pErrorCode if given the variable returns the error code for the operation. Default: NULL
  //!             <br>Note: in case the file is already open in this IFileManager instance, depending no the mode and allowMultiThreading parameters,
  //!             the already existing IFile object is returned. This is indicated by error code ::eFileAlreadyOpen instad of ::eOK.
  //! \param[in]  mode mode for opening the file (read/write access, sharing). Default: ReadShare.
  //!             <br>Note: eReadWriteExclusive allows to modify (extend) existing MDF files. This is only available with extended licensing of MDF4 Lib ("write option").
  //! \param[in]  allowMultiThreading if true, then the opened file (and its elements except IDataPointer and IDataWriter) may be used in other threads than the current one.
  //!             Default: false
  //! \return     on success the IFile interface of the opened file, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IFile* MDFLIBCALL OpenFile(LPCTSTR filePath, ErrorCode* pErrorCode = NULL, OpenFileMode mode = eReadShare, bool allowMultiThreading = false) = 0;

  ///////////////////////////////////////////////////
  //! Sorts the specified MDF file
  //!
  //! \note the method also can be used to only determine if the file is sorted without immediately sorting it.
  //!       See ::eTestSortMode.
  //!
  //! \param[in]  srcFilePath file path and name of the file to sort
  //! \param[in]  dstFilePath file path and name of the destination file to save the sorted file.
  //!             <br>If not specified or equal to source file, then the source file will be replaced (requires option bit ::eOverwriteDestinationFile)
  //! \param[in]  options bit combination of option flags, see ::SortFileOptionFlagsEnum
  //! \param[in]  pProgressCallback the sort operation may take some time. For feedback (e.g. progress bar),
  //!             a IProgressCallback interface pointer may be given which receives the progress feedback and which may
  //!             also be used to cancel the operation
  //! \return     error code of the operation.
  //! If the file already was sorted, error code eFileAlreadySorted will be returned and the file will <b>NOT</b> be copied to the destination file!
  virtual ErrorCode MDFLIBCALL SortFile(LPCTSTR srcFilePath, LPCTSTR dstFilePath = NULL, UINT32 options = 0, IProgressCallback* pProgressCallback = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Request version of MDF4Lib
  //! \param[out] pMajorVerson if not NULL, the major version number will be returned (e.g. 1 for MDF4Lib 1.0.2)
  //! \param[out] pMinorVerson if not NULL, the minor version number will be returned (e.g. 0 for MDF4Lib 1.0.2)
  //! \param[out] pRevision if not NULL, the revision number will be returned (e.g. 2 for MDF4Lib 1.0.2)
  //! \return     version number as string for display (including date and other info), NULL on error
  virtual LPCTSTR MDFLIBCALL GetLibraryVersion(UINT8* pMajorVerson = NULL, UINT8* pMinorVerson = NULL, UINT8* pRevision = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Request version of MDF4Lib interface
  //! \note The interface (API) of MDF4Lib has a 3 part version number which will be increased in case
  //! the interface is changed.
  //! \see ::CheckInterfaceCompatibility
  //! \param[out] pMajorVerson if not NULL, the major version number will be returned (e.g. 1 for interface version 1.0.2)
  //! \param[out] pMinorVerson if not NULL, the minor version number will be returned (e.g. 0 for MDF4Lib 1.0.1)
  //! \param[out] pRevision if not NULL, the revision number will be returned (e.g. 2 for MDF4Lib 1.0.2)
  //! \return     interface version number as string for display, NULL on error
  virtual LPCTSTR MDFLIBCALL GetInterfaceVersion(UINT8* pMajorVerson = NULL, UINT8* pMinorVerson = NULL, UINT8* pRevision = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Request the supported MDF version within the given major version
  //! \param[in]  majorVersion major version number (e.g. 3 for MDF 3.x or 4 for MDF 4.x)
  //! \return     max supported MDF version number, e.g. 400 for MDF 4.0.0
  virtual UINT16 MDFLIBCALL GetMaxSupportedMdfVersion(UINT8 majorVersion) = 0;

  ///////////////////////////////////////////////////
  //! Register a LogSink for debug logging
  //!
  //! All registered log sinks will receive the debug log messages equal or higher to the specified
  //! minimum logging level (default: Info, i.e. all messages are logged)
  //! A LogSink usually will write the messages to a debug log file, but may also write them to the GUI or some
  //! other output.
  //! \param[in]  pLogSink log sink to be registered
  //! \param[in]  minimumLevel minimum logging level of messages given to log sink. This can be used to restrict logging only to warnings or errors.
  //!             Default: Info (i.e. all messages are logged)
  //! \return     error code for the operation
  //! \see ILogSink, IFileManager::UnregisterLogSink
  virtual ErrorCode MDFLIBCALL RegisterLogSink(ILogSink* pLogSink, LogSinkLevel minimumLevel = eInfo) = 0;

  ///////////////////////////////////////////////////
  //! Unregister a LogSink for debug logging
  //!
  //! Only log sinks previously registered can be unregistered.
  //! \note
  //! If a log sink has been registered several times, only the last inserted instance will be removed.
  //! \param[in]  pLogSink log sink to be unregistered
  //! \return     error code for the operation
  //! \see ILogSink, IFileManager::RegisterLogSink
  virtual ErrorCode MDFLIBCALL UnregisterLogSink(ILogSink* pLogSink) = 0;

  ///////////////////////////////////////////////////
  //! Log a message to all registered global log sinks
  //!
  //! This method can be used to log own log messages from client code.
  //! \note
  //! Only log sinks previously registered by IFileManager::RegisterLogSink will receive the message.
  //! \param[in]  logLevel logging level for this message
  //! \param[in]  message the message to write
  //! \return     error code for the operation
  //! \see ILogSink, IFileManager::RegisterLogSink
  virtual ErrorCode MDFLIBCALL LogMessage(LogSinkLevel logLevel, LPCTSTR message) = 0;

  ///////////////////////////////////////////////////
  //! Request the current license level
  //! \note the license level is determined by first call to ::GetFileManager from
  //! specified customer information (customer name, serial number and registration key)
  //! \see \ref licensing
  //! \return license level, see ::LicenseLevelEnum
  virtual LicenseLevel MDFLIBCALL GetLicenseLevel() = 0;

  ///////////////////////////////////////////////////
  //! Request the customer name used for licensing
  //! \note the customer name must be given for first call to ::GetFileManager
  //! \see \ref licensing
  //! \return customer name, NULL on error
  virtual LPCTSTR MDFLIBCALL GetCustomerName() = 0;

  ///////////////////////////////////////////////////
  //! Request the serial number used for licensing
  //! \note the serial number must be given for first call to ::GetFileManager()
  //! \see \ref licensing
  //! \return serial number, NULL on error
  virtual LPCTSTR MDFLIBCALL GetSerialNumber() = 0;

  ///////////////////////////////////////////////////
  //! Creates a new file and opens it
  //!
  //! \note only available with extended licensing of MDF4 Lib ("write option").
  //! Additional method CreateFileEx available to specify the program identifier in MDF header.
  //!
  //! An opened file will be closed automatically when the IFile object and all child objects have been released
  //! \see IFile::Release
  //! \note If the file already exists, the function either fails or overwrites (deletes!) the file, depending on the setting of option bit ::eOverwriteExistingFile.
  //!
  //! \param[in]  filePath file path and name of the file to create
  //! \param[in]  mdfVersion version number of MDF format to use. Currently supported values: 200, 210, 300, 310, 320, 330, 400, 410.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation. Default: NULL
  //! \param[in]  options bit combination of option flags, see ::CreateFileOptionFlagsEnum
  //! \param[in]  allowMultiThreading if true, then the opened file (and its elements except IDataPointer) may be used in other threads than the current one.
  //!             Default: false
  //! \return     on success the IFile interface of the created file, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IFile* MDFLIBCALL CreateFile(LPCTSTR filePath, UINT16 mdfVersion = 400, ErrorCode* pErrorCode = NULL, UINT32 options = 0, bool allowMultiThreading = false) = 0;

  ///////////////////////////////////////////////////
  //! Finalizes an unfinalized MDF file
  //!
  //! \note the method also can be used to only determine if the file is unfinalized without immediately finalizing it.
  //!       See ::eTestFinalizationMode.
  //!
  //! \param[in]  srcFilePath file path and name of the file to finalize
  //! \param[in]  dstFilePath file path and name of the destination file to save the finalized file.
  //!             <br>If not specified or equal to source file, then the source file will be replaced (requires option bit ::eOverwriteDestFile)
  //! \param[in]  options bit combination of option flags, see ::FinalizeFileOptionFlagsEnum
  //! \param[in]  pProgressCallback the sort operation may take some time. For feedback (e.g. progress bar),
  //!             a IProgressCallback interface pointer may be given which receives the progress feedback and which may
  //!             also be used to cancel the operation
  //! \return     error code of the operation.
  //! If the file already was sorted, error code ::eFileAlreadySorted will be returned and the file will <b>NOT</b> be copied to the destination file!
  virtual ErrorCode MDFLIBCALL FinalizeFile(LPCTSTR srcFilePath, LPCTSTR dstFilePath = NULL, UINT32 options = 0, IProgressCallback* pProgressCallback = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Validates an MDF file
  //!
  //! \note the method also can be used to only determine if the file is sorted without immediately sorting it.
  //!       See ::eTestSortMode.
  //!
  //! \param[in]  filePath file path and name of the file to validate
  //! \param[in]  options bit combination of option flags, see ::ValidateFileOptionFlagsEnum
  //! \param[in]  pValidateFileCallback an ::IValidateFileCallback interface pointer may be given to receive error
  //!             and (if activated) warning messages during the validation.
  //!             The messages are equal to the messages in error log of Vector MDF Validator.
  //! \param[in]  pProgressCallback the sort operation may take some time. For feedback (e.g. progress bar),
  //!             a IProgressCallback interface pointer may be given which receives the progress feedback and which may
  //!             also be used to cancel the operation
  //! \return     error code of the operation.
  virtual ErrorCode MDFLIBCALL ValidateFile(LPCTSTR filePath, UINT32 options = 0, IValidateFileCallback* pValidateFileCallback = NULL, IProgressCallback* pProgressCallback = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Creates a new file and opens it
  //!
  //! \note only available with extended licensing of MDF4 Lib ("write option").
  //! Same as CreateFile, but allows to specify the program identifier in the MDF header.
  //!
  //! An opened file will be closed automatically when the IFile object and all child objects have been released
  //! \see IFile::Release
  //! \note If the file already exists, the function either fails or overwrites (deletes!) the file, depending on the setting of option bit ::eOverwriteExistingFile.
  //!
  //! \param[in]  filePath file path and name of the file to create
  //! \param[in]  mdfVersion version number of MDF format to use. Currently supported values: 200, 210, 300, 310, 320, 330, 400, 410.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation. Default: NULL
  //! \param[in]  options bit combination of option flags, see ::CreateFileOptionFlagsEnum
  //! \param[in]  allowMultiThreading if true, then the opened file (and its elements except IDataPointer) may be used in other threads than the current one.
  //!             Default: false
  //! \param[in]  progId program identifier to set in MDF header (max 8 characters, string will be truncated if longer).
  //!             <br>If NULL or empty, "MDF4Lib" will be used as program identifier.
  //!             <br>Note: the program identifier in MDF can only contain Basic Latin ASCII characters (i.e. 32 <= ASCII code <= 126, no extended ASCII codes).
  //!             <br>As a recommendation, the  program  identifier inserted into the 8 characters should be the base name (first eight characters) of the EXE/DLL of the writing application.
  //!             Alternatively, also version information of the application can be appended (e.g. "MyApp45" for version 4.5 of MyApp.exe).
  //!             <br>For MDF4, detailed information about the generating application can be added in the first FileHistory block.
  //!             Please use IFile::SetFileHistoryInfo() for this.
  //! \return     on success the IFile interface of the created file, otherwise NULL
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IFile* MDFLIBCALL CreateFileEx(LPCTSTR filePath, UINT16 mdfVersion = 400, ErrorCode* pErrorCode = NULL, UINT32 options = 0, bool allowMultiThreading = false, LPCTSTR progId = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Re-writing of an MDF file
  //!
  //! \note the method can be used to defragment an MDF file, i.e. to close gaps (e.g. after IFile::DeleteAttachment).
  //!
  //! \param[in]  srcFilePath file path and name of the file to be re-written
  //! \param[in]  dstFilePath file path and name of the destination file to save the re-written file.
  //!             <br>If not specified or equal to source file, then the source file will be replaced (requires option bit ::eOverwriteRewriteFile)
  //! \param[in]  options bit combination of option flags, see ::RewriteFileOptionFlagsEnum
  //! \param[in]  pProgressCallback the sort operation may take some time. For feedback (e.g. progress bar),
  //!             a IProgressCallback interface pointer may be given which receives the progress feedback and which may
  //!             also be used to cancel the operation
  //! \return     error code of the operation, ::eOK or (in case option bit ::eReportNoChange is set) ::eNoChange on success, some other error code on failure.
  virtual ErrorCode MDFLIBCALL RewriteFile(LPCTSTR srcFilePath, LPCTSTR dstFilePath, UINT32 options = 0, IProgressCallback* pProgressCallback = NULL) = 0;

};

