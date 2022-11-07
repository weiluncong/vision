/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IFactory
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
//! \brief Definition of IFactory


///////////////////////////////////////////////////
//! \brief Factory object for creation of sharable objects in the file.
//!
//! The IFactory object serves to create new "sharable" objects.
//! Sharable objects are objects that may be used by several other objects.
//! <br><br>
//! The following objects are sharable:
//! <ul><li>IComment<li>IUnit<li>IConversion<li>ISourceInfo</ul>
//! After creation of the sharable object, the object must be assigned to some other object in the file.
//! This can be done using one of the Set methods (e.g. IFile::SetComment) or in one of the other creation methods (e.g. IFile::CreateChannelGroup).
//! <br><br>
//! Access to the IFactory object is provided by IFile::GetFactory.
//! It is only available for newly created files (with IFileMananger::CreateFile) or
//! for files opened for modification (see IFileMananger::OpenFile).
interface IFactory
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IFactory* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //!
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Get reference to parent file
  //! \return pointer to IFile object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IFile* MDFLIBCALL GetFile() = 0;

  ///////////////////////////////////////////////////
  //! Create comment object
  //! \note CreateCommentEx() offers additionally to add/modify comment fields to a newly created or copied comment
  //! \param[in]  text pointer to text for comment.
  //!             <br>This can be a plain text or an XML fragment containing additional meta data, depending on parameter isXML.
  //!             In case of an XML fragment it should be valid XML and match the schema for the designated parent object and the respective MDF version of the file.
  //! \param[in]  isXml if true, the given text is an XML fragment, otherwise plain text. Must be false for MDF 3.x.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IComment object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IComment* MDFLIBCALL CreateComment(LPCTSTR text, bool isXml = false, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create unit object
  //! \param[in]  text pointer to text for unit.
  //!             <br>This can be a plain text or an XML fragment containing additional meta data, depending on parameter isXML.
  //!             In case of an XML fragment it should be valid XML and match the schema for the designated parent object and the respective MDF version of the file.
  //!             <br>For MDF3, the length of the unit text is restricted (< 20 chars). In case text is longer, it will be trunctated and the error code set to ::eStringTruncated.
  //! \param[in]  isXml if true, the given text is an XML fragment, otherwise plain text. Must be false for MDF 3.x.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IUnit object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IUnit* MDFLIBCALL CreateUnit(LPCTSTR text, bool isXml = false, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create source info object
  //! \note only available for MDF4. For MDF3, use the special methods IFactory::CreateBusSourceInfoMdf3 or IFactory::CreateEcuSourceInfoMdf3.
  //! The resulting ISourceInfo object may be used either for a channel or for a channel group.
  //! \param[in]  name pointer to text for name of source
  //! \param[in]  path pointer to text for path of source
  //! \param[in]  type type of source, see ::SourceTypeEnum
  //! \param[in]  busType type of bus in case source type = ::eSourceBus
  //! \param[in]  pComment comment to add to the source info.
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema si_comment.xsd for the respective MDF version of the file.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to ISourceInfo object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ISourceInfo* MDFLIBCALL CreateSourceInfo(LPCTSTR name, LPCTSTR path, SourceType type, SourceBusType busType, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create identity conversion f(x) = x
  //! \param[in]  pUnit unit for the conversion, NULL if no unit should be used
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_unit.xsd for the respective MDF version of the file.
  //! \param[in]  name pointer to text for name of conversion (can be NULL, ignored for MDF 3.x).
  //! \param[in]  pComment comment to add to the source info (can be NULL, ignored for MDF 3.x).
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_comment.xsd for the respective MDF version of the file.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL CreateIdentityConversion(IUnit* pUnit = NULL, LPCTSTR name = NULL, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create linear conversion f(x) = A * x + B
  //! \param[in]  factor factor A for linear conversion
  //! \param[in]  offset offset B for linear conversion
  //! \param[in]  pUnit unit for the conversion, NULL if no unit should be used
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_unit.xsd for the respective MDF version of the file.
  //! \param[in]  name pointer to text for name of conversion (can be NULL, ignored for MDF 3.x).
  //! \param[in]  pComment comment to add to the source info (can be NULL, ignored for MDF 3.x).
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_comment.xsd for the respective MDF version of the file.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL CreateLinearConversion(REAL factor, REAL offset, IUnit* pUnit = NULL, LPCTSTR name = NULL, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create rational conversion
  //! \param[in]  pParameterArray array with factors for rational conversion
  //! \param[in]  arrayElements number of array elements
  //!             <br>for type = ::eRational and ::ePolynomial exactly 6 parameters are required
  //!             <br>for type = ::eExponential and ::eLogarithmic exactly 7 parameters are required
  //!             <br>for type = ::eLinear exactly 2 parameters are required (same as when using CreateLinearConversion() with offset = pParameterArray[0] and factor = pParameterArray[1])
  //!             <br>Please refer to MDF specification for exact parameter definition of the respective conversion type
  //! \param[in]  type specifies the type of algebraic conversion to create (default: eRational)
  //!             <br>for MDF 4.x this can only be ::eRational and ::eLinear
  //!             <br>for MDF 3.x in addition ::ePolynomial, ::eExponential, and ::eLogarithmic are allowed
  //! \param[in]  pUnit unit for the conversion, NULL if no unit should be used
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_unit.xsd for the respective MDF version of the file.
  //! \param[in]  name pointer to text for name of conversion (can be NULL, ignored for MDF 3.x).
  //! \param[in]  pComment comment to add to the source info (can be NULL, ignored for MDF 3.x).
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_comment.xsd for the respective MDF version of the file.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL CreateRationalConversion(REAL* pParameterArray, UINT32 arrayElements, ConversionType type = eRational, IUnit* pUnit = NULL, LPCTSTR name = NULL, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create algebraic conversion
  //! \param[in]  formulaText pointer to text for formula. The formula must be valid for the current MDF version.
  //!             <br>Otherwise, please use CreateAlgebraicConversionEx where the syntax version of the given formula text can be specified.
  //! \param[in]  pUnit unit for the conversion, NULL if no unit should be used
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_unit.xsd for the respective MDF version of the file.
  //! \param[in]  name pointer to text for name of conversion (can be NULL, ignored for MDF 3.x).
  //! \param[in]  pComment comment to add to the source info (can be NULL, ignored for MDF 3.x).
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_comment.xsd for the respective MDF version of the file.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL CreateAlgebraicConversion(LPCTSTR formulaText, IUnit* pUnit = NULL, LPCTSTR name = NULL, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create value 2 value conversion
  //! \param[in]  pValue2ValueArray array with key-value pairs. The array should be sorted according to key values (increasing).
  //! \param[in]  arrayElements number of array elements (must be > 0)
  //! \param[in]  interpolate if true, a value2value conversion with interpolation is created, otherwise without
  //! \param[in]  pUnit unit for the conversion, NULL if no unit should be used
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_unit.xsd for the respective MDF version of the file.
  //! \param[in]  name pointer to text for name of conversion (can be NULL, ignored for MDF 3.x).
  //! \param[in]  pComment comment to add to the source info (can be NULL, ignored for MDF 3.x).
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_comment.xsd for the respective MDF version of the file.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL CreateValue2ValueConversion(IMDF4LibStructs::Value2Value* pValue2ValueArray, UINT32 arrayElements, bool interpolate = true, IUnit* pUnit = NULL, LPCTSTR name = NULL, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create value 2 text conversion
  //! \param[in]  pValue2TextArray array with key-text pairs. The array should be sorted according to key values (increasing).
  //! \param[in]  arrayElements number of array elements (must be > 0)
  //! \param[in]  defaultValue default value (text) to use (can be NULL, not available for MDF 3.x).
  //! \param[in]  name pointer to text for name of conversion (can be NULL, ignored for MDF 3.x).
  //! \param[in]  pComment comment to add to the source info (can be NULL, ignored for MDF 3.x).
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_comment.xsd for the respective MDF version of the file.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL CreateValue2TextConversion(IMDF4LibStructs::Value2Text* pValue2TextArray, UINT32 arrayElements, LPCTSTR defaultValue = NULL, LPCTSTR name = NULL, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create text 2 value conversion
  //! \note not available for MDF3
  //! \param[in]  pText2ValueArray array with text-value pairs.
  //! \param[in]  arrayElements number of array elements (must be > 0)
  //! \param[in]  defaultValue default value to use
  //! \param[in]  pUnit unit for the conversion, NULL if no unit should be used.
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_unit.xsd for the respective MDF version of the file.
  //! \param[in]  name pointer to text for name of conversion (can be NULL).
  //! \param[in]  pComment comment to add to the source info (can be NULL).
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_comment.xsd for the respective MDF version of the file.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL CreateText2ValueConversion(IMDF4LibStructs::Text2Value* pText2ValueArray, UINT32 arrayElements, REAL defaultValue, IUnit* pUnit = NULL, LPCTSTR name = NULL, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create text 2 text conversion
  //! \note not available for MDF3
  //! \param[in]  pText2TextArray array with key-text pairs.
  //! \param[in]  arrayElements number of array elements (must be > 0)
  //! \param[in]  defaultValue default value (text) to use (can be NULL)
  //! \param[in]  name pointer to text for name of conversion (can be NULL).
  //! \param[in]  pComment comment to add to the source info (can be NULL).
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_comment.xsd for the respective MDF version of the file.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL CreateText2TextConversion(IMDF4LibStructs::Text2Text* pText2TextArray, UINT32 arrayElements, LPCTSTR defaultValue = NULL, LPCTSTR name = NULL, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create value range 2 value conversion
  //! \note not available for MDF3
  //! \param[in]  pValueRange2ValueArray array with key range-value pairs. The array should be sorted according to key ranges (increasing).
  //! \param[in]  arrayElements number of array elements (must be > 0)
  //! \param[in]  defaultValue default value to use
  //! \param[in]  pUnit unit for the conversion, NULL if no unit should be used.
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_unit.xsd for the respective MDF version of the file.
  //! \param[in]  name pointer to text for name of conversion (can be NULL).
  //! \param[in]  pComment comment to add to the source info (can be NULL).
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_comment.xsd for the respective MDF version of the file.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL CreateValueRange2ValueConversion(IMDF4LibStructs::ValueRange2Value* pValueRange2ValueArray, UINT32 arrayElements, REAL defaultValue, IUnit* pUnit = NULL, LPCTSTR name = NULL, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create value range 2 text conversion
  //! \param[in]  pValueRange2TextArray array with key range-text pairs. The array should be sorted according to key ranges (increasing).
  //! \param[in]  arrayElements number of array elements (must be > 0)
  //! \param[in]  defaultValue default value (text) to use (can be NULL)
  //! \param[in]  name pointer to text for name of conversion (can be NULL, ignored for MDF 3.x).
  //! \param[in]  pComment comment to add to the source info (can be NULL, ignored for MDF 3.x).
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_comment.xsd for the respective MDF version of the file.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL CreateValueRange2TextConversion(IMDF4LibStructs::ValueRange2Text* pValueRange2TextArray, UINT32 arrayElements, LPCTSTR defaultValue = NULL, LPCTSTR name = NULL, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create status string table conversion with value 2 text status table
  //! \param[in]  pValue2TextArray array with key-text pairs for status text table. The array should be sorted according to key values (increasing).
  //! \param[in]  arrayElements number of array elements (must be > 0)
  //! \param[in]  pMainConversion pointer to main conversion (e.g. linear conversion)
  //!             <br>Note: object must be in current file and must not be a status table conversion. For MDF 3.x, this must be a linear conversion.
  //! \param[in]  name pointer to text for name of conversion (can be NULL, ignored for MDF 3.x).
  //! \param[in]  pComment comment to add to the source info (can be NULL, ignored for MDF 3.x).
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_comment.xsd for the respective MDF version of the file.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL CreateValue2StatusTextConversion(IMDF4LibStructs::Value2Text* pValue2TextArray, UINT32 arrayElements, IConversion* pMainConversion, LPCTSTR name = NULL, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create status string table conversion with value range 2 text status table
  //! \param[in]  pValueRange2TextArray array with key range-text pairs for status text table. The array should be sorted according to key ranges (increasing).
  //! \param[in]  arrayElements number of array elements (must be > 0)
  //! \param[in]  pMainConversion pointer to main conversion (e.g. linear conversion)
  //!             <br>Note: object must be in current file and must not be a status table conversion. For MDF 3.x, this must be a linear conversion.
  //! \param[in]  name pointer to text for name of conversion (can be NULL, ignored for MDF 3.x).
  //! \param[in]  pComment comment to add to the source info (can be NULL, ignored for MDF 3.x).
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_comment.xsd for the respective MDF version of the file.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL CreateValueRange2StatusTextConversion(IMDF4LibStructs::ValueRange2Text* pValueRange2TextArray, UINT32 arrayElements, IConversion* pMainConversion, LPCTSTR name = NULL, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create value 2 text / scale conversion
  //! \note not available for MDF3
  //! \param[in]  pValue2TextOrConversionArray array with pairs of key and a pointer to either a text or a conversion. The array should be sorted according to key values (increasing).
  //!             <br>Note: all conversion objects must be in current file
  //! \param[in]  arrayElements number of array elements (must be > 0)
  //! \param[in]  pDefaultValue pointer to either a text or a conversion to use as default value. Can be NULL.
  //!             <br>Note: in case of a conversion, the object must be in current file
  //! \param[in]  name pointer to text for name of conversion (can be NULL).
  //! \param[in]  pComment comment to add to the source info (can be NULL).
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_comment.xsd for the respective MDF version of the file.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL CreateValue2PartialConversion(IMDF4LibStructs::Value2TextOrConversion* pValue2TextOrConversionArray, UINT32 arrayElements, IMDF4LibStructs::TextOrConversion* pDefaultValue = NULL, LPCTSTR name = NULL, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create value range 2 text / scale conversion
  //! \note not available for MDF3
  //! \param[in]  pValueRange2TextOrConversionArray array with pairs of key and a pointer to either a text or a conversion. The array should be sorted according to key values (increasing).
  //!             <br>Note: all conversion objects must be in the current file.
  //! \param[in]  arrayElements number of array elements (must be > 0)
  //! \param[in]  pDefaultValue pointer to either a text or a conversion to use as default value. Can be NULL.
  //!             <br>Note: in case of a conversion, the object must  be in the current file.
  //! \param[in]  name pointer to text for name of conversion (can be NULL).
  //! \param[in]  pComment comment to add to the source info (can be NULL).
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_comment.xsd for the respective MDF version of the file.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL CreateValueRange2PartialConversion(IMDF4LibStructs::ValueRange2TextOrConversion* pValueRange2TextOrConversionArray, UINT32 arrayElements, IMDF4LibStructs::TextOrConversion* pDefaultValue = NULL, LPCTSTR name = NULL, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create source info object of type ::eSourceBus for a channel (MDF3)
  //! \note only available for MDF3. For MDF4, use method IFactory::CreateSourceInfo.
  //! The resulting ISourceInfo object can only be used for a channel.
  //! \param[in]  busChannelNumber 1-based bus channel number. Use 0 if unknown.
  //! \param[in]  messageID numeric ID of the message. Use 0 if unknown.
  //! \param[in]  messageName name of the message. Use NULL or empty string if unknown.
  //!             <br>the length of the text is restricted (< 36 chars). In case text is longer, it will be trunctated and the error code set to ::eStringTruncated.
  //! \param[in]  description text for a description. Typically this is the network node name or the name of the sender. Use NULL or empty string if unknown.
  //!             <br>the length of the text is restricted (< 36 chars). In case text is longer, it will be trunctated and the error code set to ::eStringTruncated.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation.
  //! \return pointer to ISourceInfo object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ISourceInfo* MDFLIBCALL CreateBusSourceInfoMdf3(UINT32 busChannelNumber, LPCTSTR messageName, UINT32 messageID, LPCTSTR description = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create source info object of type ::eSourceEcu for a channel (MDF3)
  //! \note only available for MDF3. For MDF4, use method IFactory::CreateSourceInfo.
  //! The resulting ISourceInfo object can only be used for a channel.
  //! \param[in]  deviceName name of the device/ECU. Use NULL or empty string if unknown.
  //!             <br>the length of the text is restricted (< 32 chars). In case text is longer, it will be trunctated and the error code set to ::eStringTruncated.
  //! \param[in]  description text for a description. Use NULL or empty string if unknown.
  //!             <br>the length of the text is restricted (< 80 chars). In case text is longer, it will be trunctated and the error code set to ::eStringTruncated.
  //! \param[in]  moduleNumber number of the module, e.g. CCP station address. Use 0 if unknown.
  //! \param[in]  address address of the module, usually unused. Use 0 if unknown.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation.
  //! \return pointer to ISourceInfo object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ISourceInfo* MDFLIBCALL CreateEcuSourceInfoMdf3(LPCTSTR deviceName, LPCTSTR description = NULL, UINT16 moduleNumber = 0, UINT32 address = 0, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get a previously created or read unit object with the same text or create a new one if not found
  //!
  //! This can be used to "share" unit objects in MDF4. For MDF3 this is identical to CreateUnit().
  //! \note only objects can be found that were created or read before!
  //! \param[in]  text pointer to text for unit.
  //!             <br>This can be a plain text or an XML fragment containing additional meta data, depending on parameter isXML.
  //!             In case of an XML fragment it should be valid XML and match the schema for the designated parent object and the respective MDF version of the file.
  //! \param[in]  isXml if true, the given text is an XML fragment, otherwise plain text. Must be false for MDF 3.x.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IUnit object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IUnit* MDFLIBCALL GetOrCreateUnit(LPCTSTR text, bool isXml, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create algebraic conversion
  //! In contrast to CreateAlgebraicConversion(), here the syntax version of the formula text can be specified.
  //! In case it does not match the syntax required for the current MDF version, the method tries to transform the formula to the required syntax.
  //! In any case, the formula will be parsed and checked for syntax errors (CreateAlgebraicConversion() does not check the syntax).
  //! \param[in]  formulaText pointer to text for formula
  //! \param[in]  syntaxVersion specifies the syntax version of the given formula text, see ::SyntaxVersionEnum
  //! \param[in]  pUnit unit for the conversion, NULL if no unit should be used
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_unit.xsd for the respective MDF version of the file.
  //! \param[in]  name pointer to text for name of conversion (can be NULL, ignored for MDF 3.x).
  //! \param[in]  pComment comment to add to the source info (can be NULL, ignored for MDF 3.x).
  //!             <br>This can be a plain text or an XML fragment containing additional meta data.
  //!             In case of an XML fragment it should be valid XML and match the schema cc_comment.xsd for the respective MDF version of the file.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL CreateAlgebraicConversionEx(LPCTSTR formulaText, SyntaxVersion syntaxVersion, IUnit* pUnit = NULL, LPCTSTR name = NULL, IComment* pComment = NULL, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Create comment object and optionally add/update/remove comment fields.
  //!
  //! This method can be used to
  //! <ul>
  //! <li>create a new comment, or
  //! <li>to clone a comment from the same or another file by appending/replacing/removing the comment text and/or comment fields
  //!     <br>See options parameter for details on how to merge the comment information.
  //!     <br>Don't forget to set ::eUpdateCreatorInformation in case the comment originates from another file.
  //! </ul>
  //! For file comments, consider using IFile::SetCommentTextAndFields instead.
  //! <br>This method offers also to set standard comment fields which, for MDF 3.x, are not encoded in the comment text.
  //! <br>
  //! <br>Example for adding/updating a comment field for an existing channel comment or to create a new comment, in case there is no comment present before.
  //! \code
  //!    IMDF4LibStructs::FieldProperties myField(_T("myFieldName"), _T("myFieldValue"));
  //!    ICommentPtr pComment = pChannel->GetComment();
  //!    if (pComment)
  //!    {
  //!      bool isXml = pComment->IsXml();
  //!      LPCTSTR oldText = isXml ? pComment->GetXml() : pComment->GetText();
  //!      pComment = pFactory->CreateCommentEx(oldText, isXml, NULL, eCommentParentChannel, &myField, 1);
  //!    }
  //!    else
  //!    {
  //!      // create a new comment with empty text and the comment field
  //!      pComment = pFactory->CreateCommentEx(NULL, false, NULL, eCommentParentChannel, &myField, 1);
  //!    }
  //!    // set the newly created comment
  //!    pChannel->SetComment(pComment);
  //! \endcode
  //! \param[in]  text pointer to text for comment; can be NULL (treated equal to empty text).
  //!             <br>This can be a plain text or text with encoded information, e.g. from an existing comment.
  //!             <br>This text will be updated or merged, depending on the options and the other parameters.
  //!             <br>For MDF 4.x encoded information requires an XML fragment (parameter isXml = true),
  //!             <br>whereas for MDF 3.x also non-XML text may contain encoded information.
  //!             <br>In case of an XML fragment it should be valid XML and match the schema for the designated parent object
  //!             and the respective MDF version of the file.
  //! \param[in]  isXml if true, the given text is an XML fragment, otherwise plain text. Must be false for MDF 3.x.
  //! \param[out] pErrorCode if given the variable returns the error code for the operation
  //! \param[in]  parentType type of parent object the comment should be assigned to. See ::CommentParentTypeEnum.
  //!             <br>Only relevant for MDF 4.x in case comment fields must be encoded, otherwise it can be set to ::eCommentParentTypeUnspec.
  //!             <br>Note: when creating a file comment for MDF 3.x, please specify ::eCommentParentFile to avoid using reserved field names for standard fields.
  //!             <br>Or, better, use IFile::SetCommentTextAndFields.
  //! \param[in]  pCommentFields array with comment fields and their properties; can be NULL if no list is specified.
  //!             <br>An empty list can be specified by pCommentFields != NULL and arrayElements = 0.
  //! \param[in]  arrayElements number of elements in pCommentFields; can be 0 if empty list, must be 0 if pCommentFields is NULL.
  //! \param[in]  plainText plain comment text to set (i.e. in &lt;TX&gt; tag for MDF 4.x); can be NULL to keep the original text.
  //!             <br>See ::CommentMergeFlagsEnum for further details.
  //! \param[in]  options bit flags for merging existing information with new information, see ::CommentMergeFlagsEnum.
  //!             <br>Attributes in MDF4 are only replaced for an already existing field if the attribute value is not NULL (for unit, desc, lang)
  //!             <br>or ::eFieldDataTypeUnspec (for type).
  //!             <br>Note: In MDF 4.x, existing fields are only possible if isXml is true.
  //! \return pointer to IComment object, NULL on error.
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IComment* MDFLIBCALL CreateCommentEx(LPCTSTR text, bool isXml = false, ErrorCode* pErrorCode = NULL, CommentParentType parentType = eCommentParentTypeUnspec,
                                               IMDF4LibStructs::FieldProperties* pCommentFields = NULL, UINT32 arrayElements = 0, LPCTSTR plainText = NULL,
                                               UINT16 options = eKeepExstingInformation) = 0;

};

