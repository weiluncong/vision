/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IComment
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
//! \brief Definition of IComment

///////////////////////////////////////////////////
//! \brief Comment of some other object
//!
//! The IComment object contains the comment text of some other object.
//! It can be a plain text or (for MDF4) also an XML fragment.
//!
//! Apart from a plain text, each comment may contain additional information encoded in XML (MDF 4.x) or in a Vector proprietary encoding (MDF 3.x).
//! <br>This information may contain so-called "Comment fields" which are the most common way to store custom information in MDF that is generally readable.
//! <br>Comment fields store custom information as key-value pairs. MDF 4.x even offers hierarchical structure of these fields using tree or list nodes (MDF 4.x).
//! <ul>
//! <li>In MDF4 Lib, comment fields are modelled by ICommentField objects that can be accessed via IComment::GetCommentFieldSet().
//! <li>In order to create IComment objects with comment fields, either use IFactory::CreateCommentEx() or (only for file comment) IFile::SetCommentTextAndFields.
//! </ul>
interface IComment
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IComment* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //!
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Get main text of comment (contents of TXBLOCK or contents of \c &lt;TX&gt; tag for MDBLOCK)
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to text, NULL on error or if not available
  virtual LPCTSTR MDFLIBCALL GetText(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Request if comment is XML string
  //! \return true if comment is XML string (MDBLOCK in MDF4 only)
  virtual bool MDFLIBCALL IsXml() = 0;

  ///////////////////////////////////////////////////
  //! Get plain XML string (only if IsXml() is true)
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to XML string, NULL on error or if not available
  virtual LPCTSTR MDFLIBCALL GetXml(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get plain text of comment after removing encoded comment fields
  //! and other Vector specific insertions in MDF 3.x
  //! \note for MDF 4.x this is identical to GetText()
  //!
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to text, NULL on error or if not available
  virtual LPCTSTR MDFLIBCALL GetPlainText(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get type of parent object of the comment
  //!
  //! \note only available for MDF 4.x in case of XML comments.
  //!       <br>The parent type is determined from the root XML tag.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return event type, see ::CommentParentTypeEnum, ::eCommentParentTypeUnspec on error or if not available
  virtual CommentParentType MDFLIBCALL GetParentType(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get comment fields encoded in IComment object
  //!
  //! \param[in]  filterFlags flags for filtering, value is a bit combination of ::CommentFieldFilterFlagsEnum
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to ICommentFieldSet object, NULL on error or if no comment fields are available
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ICommentFieldSet* MDFLIBCALL GetCommentFieldSet(UINT32 filterFlags = 0, ErrorCode* pErrorCode = NULL) = 0;

};
