/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: ICommentFieldSet
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
//! \brief Definition of ICommentFieldSet

///////////////////////////////////////////////////
//! \brief Collection of ICommentField objects
//!
//! The ICommentFieldSet object contains a number of comment fields  (see ICommentField).
//! <br>It can be used to filter comment fields associated to a comment or a comment node.
interface ICommentFieldSet
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ICommentFieldSet* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //!
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Get number of objects in set
  //! \return number of comment fields, 0 if empty or on error
  virtual UINT64 MDFLIBCALL GetCount() = 0;

  ///////////////////////////////////////////////////
  //! Get object from set for given index
  //! \param[in]  index index of element
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to ICommentField object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ICommentField* MDFLIBCALL GetFieldByIndex(UINT64 index, ErrorCode* pErrorCode = NULL) = 0;

};
