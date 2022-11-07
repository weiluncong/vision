/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: ILogSink
-------------------------------------------------------------------------------
MDF4Lib public unmanaged C++ interface
See API documentation for details
-------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
-----------------------------------------------------------------------------*/
#pragma once

#ifndef __ILOGSINK_H__
#define __ILOGSINK_H__
#endif

#include "MDF4LibDataTypes.h"
///////////////////////////////////////////////////
//! \file
//! \brief Definition of ILogSink

///////////////////////////////////////////////////
//! \brief Callback interface for debug logging
//!
//! For debugging, one or more log sinks can be registered for the global IFileManager or each IFile.
//! Own log sinks can be implemented by deriving from this interface.
interface ILogSink
{

public:

  ///////////////////////////////////////////////////
  //! Called when the log sink receives a message
  //! \note
  //! MDF4Lib only logs messages with the a logging level higher or equal to the minimum level specified
  //! at registration of the log sink.
  //! \param[in] level logging level for this message, see ::LogSinkLevelEnum
  //! \param[in] msg the message to write.
  //!            <br>Note that the string in msg is only valid until the method returns.
  //!            <br>In case it should be used later, it is essential to copy it.
  //! \see IFileManager::RegisterLogSink, IFile::RegisterLogSink, IFileManager::LogMessage
  virtual void MDFLIBCALL OnLogEntry(LogSinkLevel level, LPCTSTR msg) = 0;

  ///////////////////////////////////////////////////
  //! Get the identifier of the log sink
  //!
  //! The identifier might be used to identify a log sink.
  //! However, MDF4Lib currently only uses it for debug outputs.
  //! \return pointer to string with identifier
  virtual LPCTSTR MDFLIBCALL GetIdentifier() = 0;

};

