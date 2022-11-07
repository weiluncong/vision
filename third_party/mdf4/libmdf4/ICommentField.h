/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: ICommentField
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
//! \brief Definition of ICommentField

///////////////////////////////////////////////////
//! \brief Description of a comment field
//!
//! The ICommentField object describes a custom field of a comment.
//! <ul>
//! <li>For MDF 4.x, these are &lt;e&gt; or &lt;elist&gt; tags in XML comment (below &lt;common_properties&gt; tag).</li>
//! <li>For MDF 3.x comment fields are encoded in a proprietary format in the text comment.</li>
//! </ul>
//! \note &lt;elist&gt; tags are only available in MDF 4.1 or higher. They are similar to &lt;e&gt; tags, but can store a list of values.
interface ICommentField
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object.
  //!
  //! \return interface pointer for this object.
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ICommentField* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object.
  //!
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference.<BR>
  //! > 0 if the object still is referenced by some other object.<BR>
  //! < 0 on error.
  virtual INT32 MDFLIBCALL Release() = 0;

  /////////////////////////////////////////////////////
  //! Get name of field.
  //!
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return pointer to string with field name, NULL on error.
  virtual LPCTSTR MDFLIBCALL GetName(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get type of field.
  //!
  //! \return field type, see ::CommentFieldTypeEnum, ::eCommentFieldTypeUnspec on error.
  virtual CommentFieldType MDFLIBCALL GetType() = 0;

  /////////////////////////////////////////////////////
  //! Get number of values for this field
  //!
  //! \note MDF 4.1 or higher allows a list of values (using an &lt;elist&gt; tag).
  //! For MDF 4.0 there can be only one value.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return Number of values for this field, 0 on error.
  virtual UINT32 MDFLIBCALL GetValueCount(ErrorCode* pErrorCode = NULL) = 0;

  /////////////////////////////////////////////////////
  //! Get value of field as text for given index.
  //!
  //! \param[in]  index index of value, can be > 0 for a multi-value field (&lt;elist&gt; in MDF 4.1 or hither). \see GetValueCount()
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return pointer to string with field value, NULL on error.
  virtual LPCTSTR MDFLIBCALL GetValue(UINT32 index = 0, ErrorCode* pErrorCode = NULL) = 0;

  /////////////////////////////////////////////////////
  //! Get unit of field.
  //!
  //! \note Optional, only available for MDF version >= 4.0.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return pointer to string with unit for field, NULL on error.
  virtual LPCTSTR MDFLIBCALL GetUnit(ErrorCode* pErrorCode = NULL) = 0;

  /////////////////////////////////////////////////////
  //! Get description of field.
  //!
  //! \note Optional, only available for MDF version >= 4.0.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return pointer to string with description for field, NULL on error.
  virtual LPCTSTR MDFLIBCALL GetDescription(ErrorCode* pErrorCode = NULL) = 0;

  /////////////////////////////////////////////////////
  //! Get language of field.
  //!
  //! \note Optional, only available for MDF version >= 4.0.
  //! The language is returned as string as defined in XML standard.
  //! Example: "en-US" = English as used in the United States.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return pointer to string with language of field, NULL on error.
  virtual LPCTSTR MDFLIBCALL GetLanguage(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get data type of field value.
  //!
  //! \note Optional, only available for MDF version >= 4.0. If not specified, ::eFieldDataTypeString is used.
  //! \return event type, see ::FieldDataTypeEnum, ::eFieldDataTypeUnspec on error.
  virtual FieldDataType MDFLIBCALL GetDataType() = 0;

  ///////////////////////////////////////////////////
  //! Check if field is marked as read-only.
  //!
  //! \note Optional, only available for MDF version >= 4.0. If not specified, the field is considred not to be read-only.
  //! \return true if this field is marked as read-only, false otherwise.
  virtual bool MDFLIBCALL IsReadOnly() = 0;

  ///////////////////////////////////////////////////
  //! Get information about creator of comment field.
  //!
  //! \note Optional, only available for MDF version >= 4.0.
  //! \see IFile::GetCreatorInfo(), IFile::GetFileHistoryLog()
  //! \param[in]  index  if < 0, information about the creator of the field is returned,
  //!                    <br>otherwise about the creator of the field value
  //!                    <br>(identical to the field creator for standard &lt;e&gt; tag, can be different for a list of values in &lt;elist&gt; tag)
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return pointer to ICreatorInfo object, NULL on error or for MDF version < 4.0.
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ICreatorInfo* MDFLIBCALL GetCreatorInfo(ErrorCode* pErrorCode = NULL, INT64 index = -1) = 0;

  ///////////////////////////////////////////////////
  //! Get information of comment field as struct.
  //!
  //! \note string pointers in the struct are only valid as long as the ICommentField object exists!
  //!
  //! Can be used to copy comment fields, see IFile::SetCommentTextAndFields and IFactory::CreateCommentEx.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return property struct with information about the comment field. Missing properties are set to default values.
  virtual IMDF4LibStructs::FieldProperties MDFLIBCALL ToStruct(ErrorCode* pErrorCode = NULL) = 0;

};

