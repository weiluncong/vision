/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IConversion
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
//! \brief Definition of IConversion

///////////////////////////////////////////////////
//! \brief Conversion rule to convert a value
//!
//! The IConversion object specifies a conversion formula to convert a raw value to a physical value with a physical unit
//! , or vice versa for an inverse conversion.
//! The result of a conversion always is either a floating-point value (double) or a character string (text).
//! <br><br>
//! A conversion may return always a double value, or always a string value, or the result type may depend on the input value
//! (partial conversion or conversion that has a status table).
interface IConversion
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //!
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Get type of the conversion
  //! \return conversion type, see ::ConversionTypeEnum, ::eConversionTypeUnspec on error
  virtual ConversionType MDFLIBCALL GetType() = 0;

  ///////////////////////////////////////////////////
  //! Get result type of the conversion
  //! \return conversion result type, see ::ConversionResultTypeEnum, ::eConversionResultTypeUnspec on error
  virtual ConversionResultType MDFLIBCALL GetResultType() = 0;

  ///////////////////////////////////////////////////
  //! Request if conversion has a status string table
  //! \note for a status string table normally some raw values are reserved for exceptional states
  //! while all other raw values are converted as usual.
  //! For MDF3, a status string table may only appear in combination with a linear conversion.
  //! For MDF4, it may be combined with any conversion.
  //! \return true if conversion has a status string table, otherwise false
  virtual bool MDFLIBCALL HasStatusStringTable() = 0;

  /////////////////////////////////////////////////////
  //! Get name of conversion
  //! \note optional for MDF4, not available for MDF3
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with channel name, NULL on error
  virtual LPCTSTR MDFLIBCALL GetName(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get comment of conversion
  //! \note optional for MDF4, not available for MDF3
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IComment object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IComment* MDFLIBCALL GetComment(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get inverse conversion
  //! \note optional for MDF4, not available for MDF3 or for inverse conversion
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IConversion object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL GetInverse(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get unit of conversion
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IUnit object, NULL on error or if not available
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IUnit* MDFLIBCALL GetUnit(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Request the precision to use for display of floating point values
  //! \note the precision information is optional and not avialble for MDF3 (use IChannel::GetPrecision instead)
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return number of decimal places to use for display of floating point values or 255 (0xFF) in case of error or unrestricted precision
  virtual UINT8 MDFLIBCALL GetPrecision(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Read physical range
  //!
  //! The conversion can be used to store the minimum and maximum physical value that
  //! occurred for a signal.
  //! For arrays, the minimum and maximum value is calculated over all elements.
  //! \note the range may not be valid
  //! \param[out] pMin if not NULL the minimum physical value of the range will be assigned
  //! \param[out] pMax if not NULL the maximum physical value of the range will be assigned
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL ReadPhysRange(REAL* pMin = NULL, REAL* pMax = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Convert the given integer value
  //! \param[in] rawValue value to convert
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return result of conversion, 0 on error (check error code!)
  virtual REAL MDFLIBCALL ConvertInt64ToValue(INT64 rawValue, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Convert the given floating point value
  //! \param[in] rawValue value to convert
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return result of conversion, 0 on error (check error code!)
  virtual REAL MDFLIBCALL ConvertDoubleToValue(REAL rawValue, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Convert the given text value
  //! \param[in] rawValue text value to convert
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return result of conversion, 0 on error (check error code!)
  virtual REAL MDFLIBCALL ConvertStringToValue(LPCTSTR rawValue, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Convert the given integer value
  //! \param[in] rawValue value to convert
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with result of conversion, NULL on error
  virtual LPCTSTR MDFLIBCALL ConvertInt64ToString(INT64 rawValue, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Convert the given floating point value
  //! \param[in] rawValue value to convert
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with result of conversion, NULL on error
  virtual LPCTSTR MDFLIBCALL ConvertDoubleToString(REAL rawValue, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Convert the given text value
  //! \param[in] rawValue text value to convert
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with result of conversion, NULL on error
  virtual LPCTSTR MDFLIBCALL ConvertStringToString(LPCTSTR rawValue, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set comment of conversion
  //! \note only available for newly created or writable file and not available for MDF3
  //! \param[in] pComment pointer to IComment object, NULL to remove reference to a comment object.
  //!            <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (cc_comment.XSD).
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetComment(IComment* pComment) = 0;

  ///////////////////////////////////////////////////
  //! Set the precision to use for display of floating point values
  //! \note only available for newly created or writable file and not available for MDF3
  //! \param[in] precision number of decimal places to use for display of floating point values or 255 (0xFF) in case of unrestricted precision
  //! \param[in] isValid false if precision is invalid, true if valid (default)
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetPrecision(UINT8 precision, bool isValid = true) = 0;

  ///////////////////////////////////////////////////
  //! Set physical range
  //!
  //! \note only available for newly created conversion rule that is not assigned to any channel!
  //! To alter the physical range of a channel, consider using IChannel::SetPhysRange.
  //!
  //! Set the minimum and maximum physical value that occurred for the signal.
  //! For arrays, the minimum and maximum value is calculated over all elements.
  //! \note should only be set if this conversion rule is used for one channel only.
  //! \param[in] min minimum physical value
  //! \param[in] max maximum physical value
  //! \param[in] isValid false if physical range is invalid, true if valid (default)
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetPhysRange(REAL min, REAL max, bool isValid = true) = 0;

  ///////////////////////////////////////////////////
  //! Set unit of conversion
  //! \note only available for newly created or writable file
  //! \param[in] pUnit pointer to IUnit object, NULL to remove reference for unit.
  //!            <br>Note: object must be in same file and if unit is XML for MDF4, it should match the respective XML schema (cc_unit.XSD).
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetUnit(IUnit* pUnit) = 0;

  ///////////////////////////////////////////////////
  //! Set inverse conversion
  //! \note only available for newly created or writable file and not available for MDF3 or for inverse conversion
  //! \param[in] pConversion pointer to IConversion object for inverse conversion, NULL to remove reference for inverse conversion.
  //!            <br>Note: object must be in same file.
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetInverse(IConversion* pConversion) = 0;

  ///////////////////////////////////////////////////
  //! Get reference to parent file
  //! \return pointer to IFile object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IFile* MDFLIBCALL GetFile(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Request if conversion is a partial conversion
  //! \note Partial conversions are only possible for MDF 4.x.
  //! A status string table internally is modeled as partial conversion in MDF 4.x,
  //! but this method only returns true if the main conversion is a partial conversion.
  //! \see IFactory::CreateValue2PartialConversion() / IFactory::CreateValueRange2PartialConversion().
  //!
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return true if this is a partial conversion, false if not or on error (check error code!)
  virtual bool MDFLIBCALL IsPartialConversion(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get type of status string table
  //! \note only available if HasStatusStringTable() returns true.
  //! \see HasStatusStringTable() and IFactory::CreateValue2StatusTextConversion() / IFactory::CreateValueRange2StatusTextConversion().
  //!
  //! If the method returns
  //! <ul>
  //! <li>::eValue2Text: use GetStatusStringTableEntriesValue2Text() to access the table entries.
  //! <li>::eValueRange2Text: use GetStatusStringTableEntriesValueRange2Text() to access the table entries.
  //! </ul>
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return type of status string table (either ::eValue2Text or ::eValueRange2Text), or
  //!         <br>::eConversionTypeUnspec on error or if no status table available (check error code!)
  virtual ConversionType MDFLIBCALL GetStatusStringTableType(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get number of parameter values of formula
  //! \note only the following conversion types have parameter values:
  //! <ul>
  //! <li>::eLinear: 2 parameters (offset and factor)
  //! <li>::eRational: 6 parameters
  //! <li>::ePolynomial: 6 parameters
  //! <li>::eExponential: 7 parameters
  //! <li>::eLogarithmic: 7 parameters
  //! </ul>
  //! Please refer to MDF specification for exact parameter definition of the respective conversion type.
  //! \see IFactory::CreateLinearConversion() and IFactory::CreateRationalConversion()
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return number of parameter values, 0 on error or if not available (check error code!)
  virtual UINT32 MDFLIBCALL GetParameterCount(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get parameter value of formula
  //! \see GetParameterCount()
  //! \note For a linear conversion, GetParameterValue(0) is the offset and GetParameterValue(1) is the factor.
  //! Please refer to MDF specification for parameter definition of other conversion types (see IFactory::CreateRationalConversion()).
  //! \param[in]  index index of the parameter value to be returned
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return parameter value, 0 on error (check error code!)
  virtual REAL MDFLIBCALL GetParameterValue(UINT32 index, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get formula text for algebraic conversion
  //! \note only available for conversion type ::eAlgebraic
  //! \see IFactory::CreateAlgebraicConversionEx()
  //! \param[in]  syntaxVersion specifies the syntax version the formula text should be returned, see ::SyntaxVersionEnum.
  //!             <br>::eSyntaxVersionUnspec returns the formula text "as is".
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return formula text, NULL on error or if not available
  //! <br><br>Note that the returned text is only valid during the lifetime of the parent IConversion object.
  virtual LPCTSTR MDFLIBCALL GetFormulaText(SyntaxVersion syntaxVersion = eSyntaxVersionUnspec, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get number of entries for a conversion table
  //! \note only available for the following conversion types:
  //! <ul>
  //! <li>::eValue2ValueInterpol: use GetTableEntriesValue2Value() to access the table entries.
  //! <li>::eValue2ValueNoInterpol: use GetTableEntriesValue2Value() to access the table entries.
  //! <li>::eValueRange2Value: use GetTableEntriesValueRange2Value() to access the table entries.
  //! <li>::eValue2Text: use GetTableEntriesValue2Text() or GetTableEntriesValue2TextOrConversion() (in case of a partial converison) to access the table entries.
  //! <li>::eValueRange2Text: use GetTableEntriesValueRange2Text() or GetTableEntriesValueRange2TextOrConversion() (in case of a partial converison) to access the table entries.
  //! <li>::eText2Value: use GetTableEntriesText2Value() to access the table entries.
  //! <li>::eText2Text: use GetTableEntriesText2Text() to access the table entries.
  //! </ul>
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return number of table entries, 0 on error or if no table available (check error code!)
  virtual UINT32 MDFLIBCALL GetTableEntryCount(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get number of entries for a status string table
  //! \note only available if HasStatusStringTable() returns true.
  //! \see HasStatusStringTable() and IFactory::CreateValue2StatusTextConversion() / IFactory::CreateValueRange2StatusTextConversion().
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return number of status string table entries, 0 on error or if no status table available (check error code!)
  virtual UINT32 MDFLIBCALL GetStatusStringTableEntryCount(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get list of table entries for a value 2 value conversion
  //! \note only available for the following conversion types: ::eValue2ValueInterpol and ::eValue2ValueNoInterpol.
  //! \param[out] pValue2ValueArray array buffer to receive the key-value pairs (must not be NULL)
  //! \param[in]  arrayElements length of array, i.e. maximum number of table entries that can be stored in pValue2ValueArray (must not be 0).
  //!             <br>arrayElements may be smaller or larger than the actual number of table entries. See GetTableEntryCount().
  //! \param[in]  startElement element to start with, i.e. first table entry to insert into list, 0 to start with first element (default).
  //!             <br>This can be used to fetch only a sub range of the table entries or even a single table entry if arrayElements is 1.
  //!             <br><b>Example 1:</b>
  //!             <br>Assume the table has 10 entries and the method is called with an array buffer with arrayElements = 5 elements and startElement = 2.
  //!             <br>In this case, the table entries with (zero-based) indices 2, 3, 4, 5, 6 will be stored.
  //!             <br><b>Example 2:</b> same as Example 1, but startElement = 7.
  //!             <br>In this case, the table entries with (zero-based) indices 7, 8, 9 will be stored (index 9 is the last table entry).
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL GetTableEntriesValue2Value(IMDF4LibStructs::Value2Value* pValue2ValueArray, UINT32 arrayElements, UINT32 startElement = 0) = 0;

  ///////////////////////////////////////////////////
  //! Get list of table entries for a value range 2 value conversion
  //! \note only available for the following conversion type: ::eValueRange2Value
  //! \param[out] pValueRange2ValueArray array buffer to receive the key-value pairs (must not be NULL)
  //! \param[in]  arrayElements length of array, i.e. maximum number of table entries that can be stored in pValueRange2ValueArray (must not be 0).
  //!             <br>arrayElements may be smaller or larger than the actual number of table entries. See GetTableEntryCount().
  //! \param[in]  startElement element to start with, i.e. first table entry to insert into list, 0 to start with first element (default).
  //!             <br>This can be used to fetch only a sub range of the table entries or even a single table entry if arrayElements is 1.
  //!             <br>See also example for GetTableEntriesValue2Value().
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL GetTableEntriesValueRange2Value(IMDF4LibStructs::ValueRange2Value* pValueRange2ValueArray, UINT32 arrayElements, UINT32 startElement = 0) = 0;

  ///////////////////////////////////////////////////
  //! Get list of table entries for a value text 2 value conversion
  //! \note only available for the following conversion type: ::eText2Value
  //! \param[out] pText2ValueArray array buffer to receive the key-value pairs (must not be NULL).
  //!             <br>Please keep in mind that the text pointers for the table entries are only valid during the lifetime of the parent IConversion object.
  //! \param[in]  arrayElements length of array, i.e. maximum number of table entries that can be stored in pText2ValueArray (must not be 0).
  //!             <br>arrayElements may be smaller or larger than the actual number of table entries. See GetTableEntryCount().
  //! \param[in]  startElement element to start with, i.e. first table entry to insert into list, 0 to start with first element (default).
  //!             <br>This can be used to fetch only a sub range of the table entries or even a single table entry if arrayElements is 1.
  //!             <br>See also example for GetTableEntriesValue2Value().
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL GetTableEntriesText2Value(IMDF4LibStructs::Text2Value* pText2ValueArray, UINT32 arrayElements, UINT32 startElement = 0) = 0;

  ///////////////////////////////////////////////////
  //! Get list of table entries for a text 2 text conversion
  //! \note only available for the following conversion type: ::eText2Text
  //! \param[out] pText2TextArray array buffer to receive the key-value pairs (must not be NULL).
  //!             <br>Please keep in mind that the text pointers for the table entries are only valid during the lifetime of the parent IConversion object.
  //! \param[in]  arrayElements length of array, i.e. maximum number of table entries that can be stored in pText2TextArray (must not be 0).
  //!             <br>arrayElements may be smaller or larger than the actual number of table entries. See GetTableEntryCount().
  //! \param[in]  startElement element to start with, i.e. first table entry to insert into list, 0 to start with first element (default).
  //!             <br>This can be used to fetch only a sub range of the table entries or even a single table entry if arrayElements is 1.
  //!             <br>See also example for GetTableEntriesValue2Value().
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL GetTableEntriesText2Text(IMDF4LibStructs::Text2Text* pText2TextArray, UINT32 arrayElements, UINT32 startElement = 0) = 0;

  ///////////////////////////////////////////////////
  //! Get list of table entries for a value 2 text conversion
  //! \note only available for the following conversion type: ::eValue2Text and if IsPartialConversion() returns false.
  //!             <br>In case of a partial conversion, please use GetTableEntriesValue2TextOrConversion().
  //! \param[out] pValue2TextArray array buffer to receive the key-value pairs (must not be NULL)
  //!             <br>Please keep in mind that the text pointers for the table entries are only valid during the lifetime of the parent IConversion object.
  //! \param[in]  arrayElements length of array, i.e. maximum number of table entries that can be stored in pValue2TextArray (must not be 0).
  //!             <br>arrayElements may be smaller or larger than the actual number of table entries. See GetTableEntryCount().
  //! \param[in]  startElement element to start with, i.e. first table entry to insert into list, 0 to start with first element (default).
  //!             <br>This can be used to fetch only a sub range of the table entries or even a single table entry if arrayElements is 1.
  //!             <br>See also example for GetTableEntriesValue2Value().
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL GetTableEntriesValue2Text(IMDF4LibStructs::Value2Text* pValue2TextArray, UINT32 arrayElements, UINT32 startElement = 0) = 0;

  ///////////////////////////////////////////////////
  //! Get list of table entries for a value range 2 text conversion
  //! \note only available for the following conversion type: ::eValueRange2Text and if IsPartialConversion() returns false
  //!             <br>In case of a partial conversion, please use GetTableEntriesValueRange2TextOrConversion().
  //! \param[out] pValueRange2TextArray array buffer to receive the key-value pairs (must not be NULL).
  //!             <br>Please keep in mind that the text pointers for the table entries are only valid during the lifetime of the parent IConversion object.
  //! \param[in]  arrayElements length of array, i.e. maximum number of table entries that can be stored in pValueRange2TextArray (must not be 0).
  //!             <br>arrayElements may be smaller or larger than the actual number of table entries. See GetTableEntryCount().
  //! \param[in]  startElement element to start with, i.e. first table entry to insert into list, 0 to start with first element (default).
  //!             <br>This can be used to fetch only a sub range of the table entries or even a single table entry if arrayElements is 1.
  //!             <br>See also example for GetTableEntriesValue2Value().
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL GetTableEntriesValueRange2Text(IMDF4LibStructs::ValueRange2Text* pValueRange2TextArray, UINT32 arrayElements, UINT32 startElement = 0) = 0;

  ///////////////////////////////////////////////////
  //! Get list of table entries for a value 2 text conversion or a partial conversion
  //! \note only available for the following conversion type: ::eValue2Text.
  //!             <br>If GetPartialConversion() returns false, there should be only texts in the result buffer. In this case you also may use GetTableEntriesValue2Text().
  //! \param[out] pValue2TextOrConversionArray array buffer to receive the key-value pairs (must not be NULL)
  //!             <br>Each table entry value can be a text or an IConversion pointer, check isText member of struct.
  //!             <br>Please keep in mind that the text or IConversion pointers for the table entries are only valid during the lifetime of the parent IConversion object.
  //!             <br>In contrast to other return values, the IConversion pointers must not be released.
  //!             <br>When assigning them to a smart pointer (see \ref ref_counting), use IConversion::GetSelf(), i.e.
  //!             <br>
  //!             <code>
  //!             TextOrConversion& result = pValue2TextOrConversionArray[i].result;
  //!             </code><br><code>
  //!             VReferencePointer<IConversion> smartPtr = result.isText ? NULL : result.pConversion->GetSelf();
  //!             </code>
  //! \param[in]  arrayElements length of array, i.e. maximum number of table entries that can be stored in pValue2TextOrConversionArray (must not be 0).
  //!             <br>arrayElements may be smaller or larger than the actual number of table entries. See GetTableEntryCount().
  //! \param[in]  startElement element to start with, i.e. first table entry to insert into list, 0 to start with first element (default).
  //!             <br>This can be used to fetch only a sub range of the table entries or even a single table entry if arrayElements is 1.
  //!             <br>See also example for GetTableEntriesValue2Value().
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL GetTableEntriesValue2TextOrConversion(IMDF4LibStructs::Value2TextOrConversion* pValue2TextOrConversionArray, UINT32 arrayElements, UINT32 startElement = 0) = 0;

  ///////////////////////////////////////////////////
  //! Get list of table entries for a value range 2 text conversion or a partial conversion
  //! \note only available for the following conversion type: ::eValueRange2Text.
  //!             <br>If GetPartialConversion() returns false, there should be only texts in the result buffer. In this case you also may use GetTableEntriesValueRange2Text().
  //! \param[out] pValueRange2TextOrConversionArray array buffer to receive the key-value pairs (must not be NULL).
  //!             <br>Each table entry value can be a text or an IConversion pointer, check isText member of struct.
  //!             <br>Please keep in mind that the text or IConversion pointers for the table entries are only valid during the lifetime of the parent IConversion object.
  //!             <br>In contrast to other return values, the IConversion pointers must not be released.
  //!             <br>When assigning them to a smart pointer (see \ref ref_counting), use IConversion::GetSelf(), i.e.
  //!             <br>
  //!             <code>
  //!             TextOrConversion& result = pValue2TextOrConversionArray[i].result;
  //!             VReferencePointer<IConversion> smartPtr = result.isText ? NULL : result.pConversion->GetSelf();
  //!             </code>
  //! \param[in]  arrayElements length of array, i.e. maximum number of table entries that can be stored in pValueRange2TextOrConversionArray (must not be 0).
  //!             <br>arrayElements may be smaller or larger than the actual number of table entries. See GetTableEntryCount().
  //! \param[in]  startElement element to start with, i.e. first table entry to insert into list, 0 to start with first element (default).
  //!             <br>This can be used to fetch only a sub range of the table entries or even a single table entry if arrayElements is 1.
  //!             <br>See also example for GetTableEntriesValue2Value().
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL GetTableEntriesValueRange2TextOrConversion(IMDF4LibStructs::ValueRange2TextOrConversion* pValueRange2TextOrConversionArray, UINT32 arrayElements, UINT32 startElement = 0) = 0;

  ///////////////////////////////////////////////////
  //! Get list of table entries of status string table with value 2 text conversion
  //! \note only available if GetStatusStringTableType() returns ::eValue2Text
  //! \param[out] pValue2TextArray array buffer to receive the key-value pairs of the status string table (must not be NULL)
  //!             <br>Please keep in mind that the text pointers for the table entries are only valid during the lifetime of the parent IConversion object.
  //! \param[in]  arrayElements length of array, i.e. maximum number of table entries that can be stored in pValue2TextArray (must not be 0).
  //!             <br>arrayElements may be smaller or larger than the actual number of table entries. See GetStatusStringTableEntryCount().
  //! \param[in]  startElement element to start with, i.e. first table entry to insert into list, 0 to start with first element (default).
  //!             <br>This can be used to fetch only a sub range of the table entries or even a single table entry if arrayElements is 1.
  //!             <br>See also example for GetTableEntriesValue2Value().
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL GetStatusStringTableEntriesValue2Text(IMDF4LibStructs::Value2Text* pValue2TextArray, UINT32 arrayElements, UINT32 startElement = 0) = 0;

  ///////////////////////////////////////////////////
  //! Get list of table entries of status string table with value range 2 text conversion
  //! \note only available if GetStatusStringTableType() returns ::eValueRange2Text
  //! \param[out] pValueRange2TextArray array buffer to receive the key-value pairs of the status string table (must not be NULL)
  //!             <br>Please keep in mind that the text pointers for the table entries are only valid during the lifetime of the parent IConversion object.
  //! \param[in]  arrayElements length of array, i.e. maximum number of table entries that can be stored in pValueRange2TextArray (must not be 0).
  //!             <br>arrayElements may be smaller or larger than the actual number of table entries. See GetStatusStringTableEntryCount().
  //! \param[in]  startElement element to start with, i.e. first table entry to insert into list, 0 to start with first element (default).
  //!             <br>This can be used to fetch only a sub range of the table entries or even a single table entry if arrayElements is 1.
  //!             <br>See also example for GetTableEntriesValue2Value().
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL GetStatusStringTableEntriesValueRange2Text(IMDF4LibStructs::ValueRange2Text* pValueRange2TextArray, UINT32 arrayElements, UINT32 startElement = 0) = 0;

  ///////////////////////////////////////////////////
  //! Get default value of a conversion table (double)
  //! \note only available for the following conversion types: ::eValueRange2Value and ::eText2Value
  //! \param[out] result will be assigned to the default value (double)
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL GetTableDefaultValueDouble(REAL& result) = 0;

  ///////////////////////////////////////////////////
  //! Get default value of a conversion table (text or IConversion)
  //! \note only available for the following conversion types: ::eValue2Text, ::eValueRange2Text, ::eText2Text. The default value is optional for these conversion types.
  //! \param[out] result will be assigned to the default value (text or IConversion)
  //!             <br>The result can be a text or an IConversion pointer, check isText member of struct.
  //!             <br>Please keep in mind that the text or IConversion pointer for the result is only valid during the lifetime of the parent IConversion object.
  //!             <br>In contrast to other return values, the IConversion pointer must not be released.
  //!             <br>When assigning it to a smart pointer (see \ref ref_counting), use IConversion::GetSelf(), i.e.
  //!             <br>
  //!             <code>
  //!             VReferencePointer<IConversion> smartPtr = result.isText ? NULL : result.pConversion->GetSelf();
  //!             </code>
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL GetTableDefaultValueTextOrConversion(IMDF4LibStructs::TextOrConversion& result) = 0;


};
