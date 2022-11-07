/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IValidateFileCallback
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
//! \brief Definition of IValidateFileCallback

///////////////////////////////////////////////////
//! \brief Callback interface for IFileManager::ValidateFile
//!
//! The callback receives the errors and warnings reported during validation of the MDF file.
//! This may be used to filter for specific errors/warnings and to report (log or display) them.
//! \note The log messages and error/warning levels are the same as in the error log of Vector MDF Validator. 
interface IValidateFileCallback
{

public:

  ///////////////////////////////////////////////////
  //! Called when the file validation reports an error
  //! \note All strings provided are only valid until the method returns. In case they should be used later, it is essential to copy them.
  //! \param[in] message message for the error
  //! \param[in] nodeName name of the node the error is related to, NULL if there is no relation to a node
  //! \param[in] propertyName name of the property the error is related to, NULL if there is no relation to a property
  //! \param[in] messageType message type (currently always 0)
  virtual void MDFLIBCALL OnError(LPCTSTR message, LPCTSTR nodeName, LPCTSTR propertyName, UINT32 messageType) = 0;

  ///////////////////////////////////////////////////
  //! Called when the file validation reports a warning
  //! \note warning messages will only be posted if ::eReportWarnings bit is set in options for call to IFileManager::ValidateFile.
  //! \note All strings provided are only valid until the method returns. In case they should be used later, it is essential to copy them.
  //! \param[in] message message for the warning
  //! \param[in] nodeName name of the node the warning is related to, NULL if there is no relation to a node
  //! \param[in] propertyName name of the property the warning is related to, NULL if there is no relation to a property
  //! \param[in] messageType the message type (currently always 0)
  virtual void MDFLIBCALL OnWarning(LPCTSTR message, LPCTSTR nodeName, LPCTSTR propertyName, UINT32 messageType) = 0;

};

