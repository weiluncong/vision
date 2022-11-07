/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IAttachment
-------------------------------------------------------------------------------
MDF4Lib public unmanaged C++ interface
See API documentation for details
-------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
-----------------------------------------------------------------------------*/
#pragma once

#include "MDF4LibDataTypes.h"

//! \file
//! \brief Definition of IAttachment

///////////////////////////////////////////////////
//! \brief Attachment of the MDF file (external or embedded)
//!
//! An attachment is a file that is either included in the MDF file as embedded attachment
//! or that is referenced as external attachment. Attachments were introduced with MDF4.
//! <br><br>
//! For MDF3, a program block (PR block) is the equivalent of an embedded attachment. There can be only one single PR block for an MDF3 file.
//! The MDF4Lib will model it as single attachment, however no MIME type or other information will be available.
//! <br><br>
//! You may also read the binary data of an attachment (embedded or external), or save the attachment to a file on hard disk.
interface IAttachment
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IAttachment* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //!
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Get file name of attachment
  //!
  //! \note For embedded attachments the file name is optional. If given, it is the name of the original file.
  //! The file name may contain a relative or absolute path.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with file name, NULL on error
  virtual LPCTSTR MDFLIBCALL GetFileName(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get comment of attachment
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IComment object, NULL on error or for MDF3
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IComment* MDFLIBCALL GetComment(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get MIME type of attachment
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with MIME type, NULL on error or for MDF3
  virtual LPCTSTR MDFLIBCALL GetMimeType(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get type of attachment
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return type of attachment, see ::AttachmentTypeEnum
  virtual AttachmentType MDFLIBCALL GetType(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Request if attachment is embedded
  //! \see GetType
  //! \return true if attachment is embedded, false otherwise
  virtual bool MDFLIBCALL IsEmbedded() = 0;

  ///////////////////////////////////////////////////
  //! Read the MD5 checksum calculated for the attachment
  //! \note The MD5 check is only optional and may not be available. It is not available for MDF3.
  //! \param[out] checksum 16 Byte buffer for receiving the 128-bit value for MD5 checksum
  //! (of the uncompressed data if data is embedded and compressed)
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL ReadCheckSumMD5(BYTE checksum[16]) = 0;

  ///////////////////////////////////////////////////
  //! Get info about creator of the attachment
  //! \see IFile::GetCreatorInfo, IFile::GetFileHistory
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to ICreatorInfo object, NULL on error or for MDF3
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ICreatorInfo* MDFLIBCALL GetCreatorInfo(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Request the original size of the data
  //! \return Original data size in Bytes, either for external file or for uncompressed data
  virtual UINT64 MDFLIBCALL GetOriginalSize() = 0;

  ///////////////////////////////////////////////////
  //! Request the size of the embedded data in Bytes
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return Number of Bytes for binary embedded data (possibly compressed)
  virtual UINT64 MDFLIBCALL GetEmbeddedSize(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Read a number of Bytes of attachment data
  //! \note in case of compressed embedded attachment, the uncompressed data will be returned!
  //! \param[out] pBuffer allocated buffer to receive the read data
  //! \param[in]  bufferSize size of the buffer
  //! \param[out] pReadSize if given, this argument returns the actually read size (may be less than bufferSize)
  //! \param[in]  offset offset from start of attachment data
  //! \return error code of the operation, ::eReachedEndOfData in case not complete buffer could be read (actually read size in pReadSize)
  virtual ErrorCode MDFLIBCALL ReadData(BYTE* pBuffer, UINT32 bufferSize, UINT32* pReadSize = NULL, UINT64 offset = 0) = 0;

  ///////////////////////////////////////////////////
  //! Save the attachment to a file
  //! \note be careful not to overwrite existing file (see argument overwrite)
  //! \param[in]  fileName name of destination file
  //! \param[in]  overwrite false if an existing destination file must not be overwritten, overwrite true if if can be overwritten
  //! \param[in]  pProgressCallback for large attachments, this operation may take some time. For feedback (e.g. progress bar),
  //!             a IProgressCallback interface pointer may be given which receives the progress feedback and which may
  //!             also be used to cancel the operation
  virtual ErrorCode MDFLIBCALL SaveToFile(LPCTSTR fileName, bool overwrite = false, IProgressCallback* pProgressCallback = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Calculate the MD5 checksum for the attachment
  //! \note for embedded compressed data, the checksum is calculated for the uncompressed data!
  //! \param[out] checksum 16 Byte buffer for receiving the 128-bit value for MD5 checksum
  //! \param[in]  pProgressCallback for large attachments, this operation may take some time. For feedback (e.g. progress bar),
  //!             a IProgressCallback interface pointer may be given which receives the progress feedback and which may
  //!             also be used to cancel the operation
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL CalculateCheckSumMD5(BYTE checksum[16], IProgressCallback* pProgressCallback = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get file path of attachment
  //!
  //! The file path is determined by using the file name relative to the path of the MDF file
  //! \note For embedded attachments the file name is optional. If given, usually represents the name of the original file.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with file path, NULL on error
  virtual LPCTSTR MDFLIBCALL GetFilePath(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set comment of attachment
  //! \note only available for newly created or writable file, not available for MDF3
  //! \param[in] pComment pointer to IComment object, NULL to remove reference to comment object.
  //!            <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (at_comment.XSD).
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetComment(IComment* pComment) = 0;

  ///////////////////////////////////////////////////
  //! Calculate and update the MD5 checksum for the attachment
  //! \note only available for newly created or writable file, not available for MDF3
  //! \note for embedded compressed data, the checksum is calculated for the uncompressed data!
  //! \param[in]  pProgressCallback for large attachments, this operation may take some time. For feedback (e.g. progress bar),
  //!             a IProgressCallback interface pointer may be given which receives the progress feedback and which may
  //!             also be used to cancel the operation
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL UpdateCheckSumMD5(IProgressCallback* pProgressCallback = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set the MD5 checksum for the attachment
  //! \note only available for newly created or writable file, not available for MDF3
  //! This method can be used to set or change the checksum of an attachment, e.g. if it has been calculated before.
  //! It also allows to reset (invalidate) the checksum.
  //! <br><br>Please make sure that the checksum is correct!
  //! \param[in] checksum 16 Byte buffer for the 128-bit value for MD5 checksum
  //! (of the uncompressed data if data is embedded and compressed)
  //! \param[in] isValid true if the checksum is valid, false if the checksum should be invalidated
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetCheckSumMD5(BYTE checksum[16], bool isValid = true) = 0;

  ///////////////////////////////////////////////////
  //! Set or change the file path of the attachment
  //!
  //! \note only available for newly created or writable file, not available for MDF3
  //! A file path either is absolute or relative to the path of the MDF file.
  //! Note that after changing the file path, the MD5 check sum may become invalid (possibly call UpdateCheckSumMD5 or SetCheckSumMD5)
  //! \note For embedded attachments the file name is optional. If given, it usually represents the name of the original file.
  //! \param[in] filePath file path to be set, NULL to delete the file path (only allowed for embedded attachments)
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetFilePath(LPCTSTR filePath) = 0;

  ///////////////////////////////////////////////////
  //! Set or change the mime type of the attachment
  //!
  //! \note only available for newly created or writable file, not available for MDF3
  //! \param[in] mimeType mime type string to be set, NULL to delete the mime type.
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetMimeType(LPCTSTR mimeType) = 0;

  ///////////////////////////////////////////////////
  //! Get start addresses of AT block
  //! \return returns start address of AT block for this channel, -1 on error.
  virtual INT64 MDFLIBCALL GetBlockStartAddress() = 0;
};
