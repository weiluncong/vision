/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IUnit
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
//! \brief Definition of IUnit

///////////////////////////////////////////////////
//! \brief Unit of a channel or a conversion
//!
//! The IUnit object contains the unit name and possibly description of a physical unit.
//! It can be a plain text or (for MDF4) also an XML fragment.
interface IUnit
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IUnit* MDFLIBCALL GetSelf() = 0;  

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //! 
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Get name of unit (contents of TXBLOCK or contents of \c &lt;TX&gt; tag for MDBLOCK)
  //! \note for MDF4 channel data types eMimeSample and eMimeStream, the name of IUnit of the channel specifies the MIME type
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to text
  virtual LPCTSTR MDFLIBCALL GetName(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Request if unit is XML string
  //! \return true if unit is XML string (MDBLOCK in MDF4 only)
  virtual bool MDFLIBCALL IsXml() = 0;

  ///////////////////////////////////////////////////
  //! Get plain XML string (only if IsXml() is true)
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to XML string
  virtual LPCTSTR MDFLIBCALL GetXml(ErrorCode* pErrorCode = NULL) = 0;

};
