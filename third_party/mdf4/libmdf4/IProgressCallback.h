/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IProgressCallback
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
//! \brief Definition of IProgressCallback

///////////////////////////////////////////////////
//! \brief Callback interface to report progress of a task
//!
//! For long-lasting tasks (e.g. sorting of a file), you may provide a callback to receive information about the progress
//! of the task and to abort the task. This can be done by implementing a class that derives from this interface.
interface IProgressCallback
{

public:
  
  //! will be called when task starts with initialization message
  //! (e.g. for display in task bar etc.)
  //!
  //! \param[in] message initialization message
  //!            <br>Note that the string in message is only valid until the method returns. 
  //!            <br>In case it should be used later, it is essential to copy it.
  virtual void MDFLIBCALL OnInitialize(LPCTSTR message) = 0;

  //! will be called when task has finished with result and result message
  //! (e.g. for display in a result log or (in case of error) in a dialog)
  //!
  //! \param[in] result error code of task (< 0 in case of critical errors)
  //! \param[in] message result/error message
  //!            <br>Note that the string in message is only valid until the method returns. 
  //!            <br>In case it should be used later, it is essential to copy it.
  virtual void MDFLIBCALL OnFinish(ErrorCode result, LPCTSTR message) = 0;

  //! will be called on update of progress
  //! (e.g. for update of progress bar)
  //!
  //! will also check if user wants to cancel the task
  //! \sa OnCheckCancel
  //!
  //! \param[in] percentage percentage of progress (0-100)
  //! \return true in case user wants to cancel the task
  //!         false if the task can continue
  virtual bool MDFLIBCALL OnProgress(UINT8 percentage) = 0;

  //! will be called when checking if user wants to cancel the task
  //!
  //! \return true in case user wants to cancel the task, 
  //!         false if the task can continue
  virtual bool MDFLIBCALL OnCheckCancel() = 0;

};
