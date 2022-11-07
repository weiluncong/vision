/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IFileSizeObserver
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
//! \brief Definition of IFileSizeObserver

///////////////////////////////////////////////////
//! \brief Helper for observing the file size when writing an MDF file
//!
//! The IFileSizeObserver object allows to observe the size of an MDF file during writing. 
//! <br><br>
//! When writing sorted MF4 files or using option eUseAsyncFileWrite for IFileManager::CreateFile,
//! data is buffered and not written immediately to the file on the hard disk.
//! In order to survey the size of the final MDF file, the IFileSizeObserver object helps. 
//! \note the estimated file size may differ from the final file size, especially when writing compressed data (MDF >= 4.1)
interface IFileSizeObserver
{

public:
  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IFileSizeObserver* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //! 
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Get reference to observed file
  //! \return pointer to IFile object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IFile* MDFLIBCALL GetFile() = 0;

  ///////////////////////////////////////////////////
  //! Return the current estimated file size
  //! The file size on disk may not be the final file size because some data might still be contained in memory buffers (e.g. using IDataWriter or with AsyncFileWrite option).
  //! This method returns an estimated size of the file after flushing the buffers. When writing compressed data (MDF >= 4.1), this may only be a rough guess.
  //! The method can be used in a polling mechanism to react if the final file size exceeds a certain limit (e.g. start a new output file).
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return current estimated file size, 0 on error or if file size estimation is not active (see pErrorCode)
  virtual UINT64 MDFLIBCALL GetEstimatedFileSize(ErrorCode* pErrorCode = NULL) = 0;

};
