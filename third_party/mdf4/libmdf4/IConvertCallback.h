/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IConvertCallback
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
//! \brief Definition of IConvertCallback

///////////////////////////////////////////////////
//! \brief Callback interface to for IFileManager::ConvertToMdf
//!
//! For conversion of multiple files, it can be used to receive a notification after a single file has finished including its result code (ErrorCode).
//! Furthermore, it can be used to receive log messages (similar to ILogSink), but including the file name of the converted file the log messages belongs to.
//! \note: in case the conversion is executed multi-threaded (i.e. option bit ::eConvertMultiThreaded is set), then the calls to each interface method must be thread-safe!
interface IConvertCallback
{

public:

  ///////////////////////////////////////////////////
  //! Called when a destination has been finished with result and result message
  //! (e.g. for display in a result log or (in case of error) in a dialog)
  //!
  //! \param[in] dstFile path and name of destination file for which the current log entry has been generated
  //! \param[in] result error code of task (< 0 in case of critical errors)
  //! \param[in] msg result/error message
  //!            <br>Note that the string in message is only valid until the method returns.
  //!            <br>In case it should be used later, it is essential to copy it.
  virtual void MDFLIBCALL OnConversionFinished(LPCTSTR dstFile, ErrorCode result, LPCTSTR msg) = 0;

  ///////////////////////////////////////////////////
  //! Determine the minimum logging level for a converted file.
  //! \note the value is cached, i.e. the method is only called once per file.
  //! Before conversion, the method also is called once with argument dstFile = NULL. In case the method here returns :eLogSinkLevelUnspec, it is assumed that logging is disabled for all files.
  //! \param[in] dstFile path and name of destination file for which the current logging level is requested, NULL to check if logging is enabled at all (disabled if result is :eLogSinkLevelUnspec)
  //! \return the logging level for the specified input file (see ::LogSinkLevelEnum). In case the methods returns ::eLogSinkLevelUnspec, logging is assumed to be disabled.
  virtual LogSinkLevel MDFLIBCALL GetMinimumLogLevel(LPCTSTR dstFile = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Called when the log sink receives a message
  //! \note
  //! MDF4Lib only calls this method in case a previous request of IsLoggingEnabled() returns true for the specified level.
  //! \param[in] dstFile path and name of destination file for which the current log entry has been generated (NULL if this is a general message)
  //! \param[in] level logging level for this message, see ::LogSinkLevelEnum
  //! \param[in] msg the message to write.
  //!            <br>Note that the string in msg is only valid until the method returns.
  //!            <br>In case it should be used later, it is essential to copy it.
  //! \see IFileManager::RegisterLogSink, IFile::RegisterLogSink, IFileManager::LogMessage
  virtual void MDFLIBCALL OnLogEntry(LPCTSTR dstFile, LogSinkLevel level, LPCTSTR msg) = 0;

};



