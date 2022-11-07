/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IFormatInfo
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
//! \brief Definition of IFormatInfo

///////////////////////////////////////////////////
//! \brief Information about the file format and version
//!
//! The IFormatInfo object is the equivalent to the ID block of the MDF file. 
//! <br><br>
//! The ID block is defined both for MDF3 and MDF4 and contains the format information.
//! You can read the MDF version number and string and the program identifier.
//! \note for MDF4 more info about the creating tool can be found in ICreatorInfo object 
//! returned by IFile::GetCreatorInfo.
interface IFormatInfo
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IFormatInfo* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //! 
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Get MDF version number
  virtual UINT16 MDFLIBCALL GetVersionNumber() = 0;
  
  ///////////////////////////////////////////////////
  //! Get version string
  virtual LPCTSTR MDFLIBCALL GetVersionString() = 0;

  ///////////////////////////////////////////////////
  //! Get program identifier
  virtual LPCTSTR MDFLIBCALL GetProgramIdentifier() = 0;

};

