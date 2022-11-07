/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IArrayInfo
-------------------------------------------------------------------------------
MDF4Lib public unmanaged C++ interface
See API documentation for details
-------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
-----------------------------------------------------------------------------*/
#pragma once 

#include "MDF4LibDataTypes.h"

//! \file
//! \brief Definition of IArrayInfo

///////////////////////////////////////////////////
//! \brief Information about array properties of an array channel
//!
//! The IArrayInfo object serves to access or modify special array properties of a channel.
//! <br><br> 
//! For MDF3, arrays are stored with a CD block, for MDF4 with one or more CA blocks.
//! <br><br> 
interface IArrayInfo
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IArrayInfo* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //! 
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Get type of array
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return type of array, see ArrayTypeEnum
  virtual ArrayType MDFLIBCALL GetType(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get number of dimensions of the array
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return number of dimensions, 0 on error
  virtual UINT16 MDFLIBCALL GetDimCount(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get number of elements for given dimension
  //! \note in case there is a dynamic size channel for this dimension, then this is the maximum number of elements possible.
  //! The actual number of elements is specified by the physical value of the dynamic size channel at the same time.
  //! \param[in]  dimension index of dimension
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return number of elements for given dimension, 0 on error
  virtual UINT64 MDFLIBCALL GetDimSize(UINT16 dimension, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get axis channel for given dimension
  //! \note in case of fixed axis, an axis channel is not available
  //! Remember to check if there is a separate conversion rule specified for this axis (see GetAxisConversion).
  //! \param[in]  dimension index of dimension
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IChannel object for the axis, NULL on error or if not available
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL GetAxisChannel(UINT16 dimension, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set axis channel for given dimension
  //! \note only available for newly created or writable file and if the ::eHasAxis flag was set and ::eHasFixedAxis flag is not set for given dimension.
  //! For MDF4, if the axis has a conversion, the same rule will be set as axis conversion (see GetAxisConversion).
  //! \param[in] dimension index of dimension
  //! \param[in] pChannel pointer to IChannel object to set as axis, NULL to remove reference for this axis.
  //!            <br>Note: channel object must be in the same file and must be a 1-dimensional array of type ::eIsScalingAxis or ::eIsIntervalAxis (later only for ::eIsClassificationResult array type)
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetAxisChannel(UINT16 dimension, IChannel* pChannel) = 0;

  ///////////////////////////////////////////////////
  //! Get conversion of axis for given dimension
  //! \note only available for MDF4 and if there are axes for this array
  //! In case of a fixed axis, the axis conversion is used to get the physical axis values. If not specified, the physical values are identical to the raw values.<br>
  //! In case of a dynamic axis, the axis conversion must be applied to the raw values of the axis channel (overrules the conversion of the axis channel). 
  //! If not specified, the raw axis values can be used as physical value (1:1 conversion). 
  //! However, typically, the axis conversion is identical to the conversion of the axis channel.
  //! \param[in]  dimension index of dimension
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IConversion object to be used for the axis, NULL on error or if not available
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IConversion* MDFLIBCALL GetAxisConversion(UINT16 dimension, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set conversion of axis for given dimension
  //! \note only available for MDF4 and if there are axes for this array
  //! In case of a fixed axis, the axis conversion is used to get the physical axis values. If not specified, the physical values are identical to the raw values.<br>
  //! In case of a dynamic axis, the axis conversion must be applied to the raw values of the axis channel (overrules the conversion of the axis channel). 
  //! If not specified, the raw axis values can be used as physical value (1:1 conversion). 
  //! However, typically, the axis conversion is identical to the conversion of the axis channel.
  //! \param[in] dimension index of dimension
  //! \param[in] pConversion pointer to IConversion object to be set for the axis, NULL to remove reference for this axis conversion.
  //!            <br>Note: conversion object must be in the same file
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetAxisConversion(UINT16 dimension, IConversion* pConversion) = 0;

  ///////////////////////////////////////////////////
  //! Set raw values of fixed axis for given dimension
  //! \note only available if there are fixed axes for this array
  //! \note creating an array object (see IChannel::ConverterToArray) with fixed axis, the fixed axis values are automatically initialized with the indizes of the axis (0, 1, 2, ...).
  //! Axis values are given as raw values. In addition, a conversion rule can be specified for conversion to physical values (alternatively use SetAxisConversion).
  //! <br>If no a axis conversion is present, the physical values are identical to the raw values (1:1 conversion).
  //! \param[in] dimension index of dimension
  //! \param[in] pRawValueArray pointer to array with raw values to be set for the fixed axis (must not be NULL).
  //! \param[in] arrayElements length of pRawValueArray array, i.e. maximum number of values contained (must not be 0).
  //!            <br>If arrayElements < dimSize, only the first arrayElements values are set and the method returns ::eInputBufferTooSmall.
  //!            <br>If arrayElements > dimSize, only the first dimSize values are set.
  //! \param[in] pConversion pointer to IConversion object to be set for the axis. If NULL, the axis conversion is not changed (leave existing link).
  //!            <br>Note: conversion object must be in the same file
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetFixAxisRawValues(UINT16 dimension, REAL* pRawValueArray, UINT64 arrayElements, IConversion* pConversion = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get raw values of fixed axis for given dimension
  //! \note only available if there are fixed axes for this array.
  //! <br>Axis values are given as raw values. In addition, a conversion rule can be specified for conversion to physical values (see GetAxisConversion).
  //! <br>If no a axis conversion is present, the physical values are identical to the raw values (1:1 conversion).
  //! \param[in] dimension index of dimension
  //! \param[in] pRawValueArray pointer to array buffer to receive the result values (must not be NULL).
  //! \param[in] arrayElements length of pRawValueArray buffer, i.e. maximum number of values that can be stored (must not be 0).
  //!            <br>If arrayElements < dimSize, only the first arrayElements values are set and the method returns ::eInputBufferTooSmall.
  //!            <br>If arrayElements > dimSize, only the first dimSize values are set.
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL GetFixAxisRawValues(UINT16 dimension, REAL* pRawValueArray, UINT64 arrayElements) = 0;

  ///////////////////////////////////////////////////
  //! Get dynamic size channel for given dimension
  //! A dynamic size channel is used if the number of elements for a dimension can vary over time. 
  //! Its (physical) value at a specific instant of time specifies the number of elements. 
  //! This is valid until the value of dynamic size channel changes. See also GetDimSize().
  //! \note only available for MDF4
  //! \param[in]  dimension index of dimension
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IChannel object for the dynamic size channel, NULL on error or if not available
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL GetDynamicSizeChannel(UINT16 dimension, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set dynamic size channel for given dimension
  //! \note only available for MDF4
  //! The physical value of a dynamic size channel must be numeric  
  //! (Ideally the channel has no conversion rule and an unsigned integer data type)
  //! It should not exceed the maximum number of dimensions as given by GetDimSize 
  //! (currently not checked by MDF4 Lib).
  //! \param[in] dimension index of dimension
  //! \param[in] pChannel pointer to IChannel object to set as dynamic size channel, NULL to remove reference for it.
  //!            <br>Note: channel object must be in the same file. 
  //!            <br>Since the dynamic size channel and the array channel must be synchronized, their channel 
  //!            groups must contain at least one common master channel type. 
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetDynamicSizeChannel(UINT16 dimension, IChannel* pChannel) = 0;
  
  ///////////////////////////////////////////////////
  //! Get input quantity ("working point") channel for given dimension
  //! \note only available for MDF4
  //! \param[in]  dimension index of dimension
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IChannel object for the input quantity channel, NULL on error or if not available
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL GetInputQuantityChannel(UINT16 dimension, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set input quantity ("working point") channel for given dimension
  //! \note only available for MDF4
  //! The input quantity channel should have the same physical unit as the axis it is applied to.
  //! (currently not checked by MDF4 Lib).
  //! \param[in] dimension index of dimension
  //! \param[in] pChannel pointer to IChannel object to set as input quantity, NULL to remove reference for it.
  //!            <br>Note: channel object must be in the same file. 
  //!            <br>Since the input quantity channel and the array channel must be synchronized, their channel 
  //!            groups must contain at least one common master channel type. 
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetInputQuantityChannel(UINT16 dimension, IChannel* pChannel) = 0;
  
  ///////////////////////////////////////////////////
  //! Get output quantity channel for the array
  //! \note only available for MDF4
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IChannel object for the output quantity channel, NULL on error or if not available
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL GetOutputQuantityChannel(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set output quantity channel for the array
  //! \note only available for MDF4
  //! The output quantity channel should have the same physical unit as the array elements.
  //! (currently not checked by MDF4 Lib).
  //! \param[in] pChannel pointer to IChannel object to set as output quantity, NULL to remove reference for it.
  //!            <br>Note: channel object must be in the same file. 
  //!            <br>Since the output quantity channel and the array channel must be synchronized, their channel 
  //!            groups must contain at least one common master channel type. 
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetOutputQuantityChannel(IChannel* pChannel) = 0;
  
  ///////////////////////////////////////////////////
  //! Get comparison quantity channel for the array
  //! \note only available for MDF4
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IChannel object for the comparison quantity channel, NULL on error or if not available
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL GetComparisonQuantityChannel(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set comparison quantity channel for the array
  //! \note only available for MDF4
  //! The comparison quantity channel should have the same physical unit as the array elements.
  //! (currently not checked by MDF4 Lib).
  //! \param[in] pChannel pointer to IChannel object to be set as comparison quantity, NULL to remove reference for it.
  //!            <br>Note: channel object must be in the same file. 
  //!            <br>Since the comparison quantity channel and the array channel must be synchronized, their channel 
  //!            groups must contain at least one common master channel type. 
  //! \return error code of the operation
  virtual ErrorCode MDFLIBCALL SetComparisonQuantityChannel(IChannel* pChannel) = 0;

};
