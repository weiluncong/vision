/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: ICreatorInfo
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
//! \brief Definition of ICreatorInfo

///////////////////////////////////////////////////
//! \brief Information about the creator of an object
//!
//! The ICreatorInfo describes who/which tool generated or changed the MDF file or an element. 
//! It represents the content of a FH block for MDF4. For MDF3 there is no equivalent information.
//! <br><br>
//! For MDF4, the list of FH blocks contains the change history of the MDF file. 
//! The log entries can be accessed by IFile::GetFileHistoryLog. 
//! For the main creator of the MDF file, use IFile::GetCreatorInfo, which should be equal to IFile::GetFileHistoryLog(0).
//! <br><br>
//! The IAttachment and the IEvent object, or the meta info in XML also contain information about the creator info, i.e. who/which tool
//! generated or changed it (IAttachment::GetCreatorInfo / IEvent::GetCreatorInfo)
interface ICreatorInfo
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ICreatorInfo* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //! 
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Get index in global list of file history entries
  //! \note index 0 means that this is the creator info of the file
  //! \see IFile::GetCreatorInfo
  //! \return zero-based index
  virtual UINT32 MDFLIBCALL GetIndex() = 0;

  ///////////////////////////////////////////////////
  //! Get comment of creator info
  //! \note for creator info (MDF4), it is always an XML string
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IComment object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IComment* MDFLIBCALL GetComment(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get new reference to time stamp object of the creator info
  //!
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return interface pointer for ITimeStamp object
  virtual ITimeStamp* MDFLIBCALL GetTimeStamp(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get name of tool
  //!
  //! Returns the contents of the \c &lt;tool_id&gt; tag 
  //! with name/identification of the tool that has written this log entry.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to text or NULL on error
  virtual LPCTSTR MDFLIBCALL GetToolName(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get name of vendor
  //!
  //! Returns the contents of the \c &lt;tool_vendor&gt; tag 
  //! with name of the vendor/author of the tool that has written this log entry.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to text or NULL on error
  virtual LPCTSTR MDFLIBCALL GetVendorName(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get tool version
  //!
  //! Returns the contents of the \c &lt;tool_version&gt; tag 
  //! with version info about the tool that has written this log entry.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to text or NULL on error
  virtual LPCTSTR MDFLIBCALL GetToolVersion(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get user name
  //!
  //! Returns the contents of the \c &lt;user_name&gt; tag 
  //! with name/identification of the user who has written this log entry.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to text or NULL on error
  virtual LPCTSTR MDFLIBCALL GetUserName(ErrorCode* pErrorCode = NULL) = 0;

};
