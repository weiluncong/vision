/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: ISourceInfo
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
//! \brief Definition of ISourceInfo

///////////////////////////////////////////////////
//! \brief Source of an acquisition or a signal
//!
//! The ISourceInfo object of a channel gives information about the source of the signal, e.g. the device name. 
//! For a channel group gives the ISourceInfo specifies the source of the acquisition, e.g. a bus message or also an ECU/network node.
//! <br><br>
//! For MDF3, the ISourceInfo represents the CE block, which either holds the device name for signals from an ECU, or the bus message name and ID for signal from a bus message.
//! <br>For MDF4, the bus message information is contained in the source info of the channel group.
//! <br>In order to access the message name, use IChannel::GetMessageName.
//! Alternatively use IChannel::GetExtendedNamePrefix in order to either get the message or the device name.
interface ISourceInfo
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ISourceInfo* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //! 
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  /////////////////////////////////////////////////////
  //! Get name of source
  //! \note for MDF3 the message name (ce_type = VCN) or the ECU identification (ce_type = DIM) will be returned
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with source name, NULL on error
  virtual LPCTSTR MDFLIBCALL GetName(ErrorCode* pErrorCode = NULL) = 0;

  /////////////////////////////////////////////////////
  //! Get path of source
  //! \note not available for MDF3
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to string with source path, NULL on error
  virtual LPCTSTR MDFLIBCALL GetPath(ErrorCode* pErrorCode = NULL) = 0;

  /////////////////////////////////////////////////////
  //! Get comment of source
  //! \note for MDF3 the description string of CE block (ce_type = DIM) or the sender name (ce_type = VCN) will be returned
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IComment object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IComment* MDFLIBCALL GetComment(ErrorCode* pErrorCode = NULL) = 0;

  /////////////////////////////////////////////////////
  //! Get type of the source
  //! \note for MDF3 ce_type = DIM is mapped to eSourceEcu and ce_type = VCN to ::eSourceBus
  //! \return source type, see ::SourceTypeEnum, ::eSourceTypeUnspec on error
  virtual SourceType MDFLIBCALL GetSourceType() = 0;

  /////////////////////////////////////////////////////
  //! Get bus type of the source
  //! \note for MDF3 ce_type = DIM is mapped to eBusNone and ce_type = VCN to eBusOther
  //! <br>for MDF4 custom or unknown bus types are mapped to eBusOther
  //! \return bus type, see ::SourceBusTypeEnum, ::eSourceBusTypeUnspec on error
  virtual SourceBusType MDFLIBCALL GetBusType() = 0;

  /////////////////////////////////////////////////////
  //! Get logical bus channel number for the source info
  //! 
  //! The logical bus channel is a 1-based integer number and is used to enumerate bus channels of the same type (e.g. all CAN buses).
  //! It is optional. For MDF4, it typically is contained in the acquisition source. 
  //! For MDF3, it can only be contained in the channel source because there is no acquisition source.<br><br>
  //! For a source type ::eSourceBus, the bus channel number relates to the monitored bus. 
  //! For source type ::eSourceEcu it relates to the bus over which the ECU signals were transported (use case not available for MDF3).
  //!  
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return bus channel number or 0 on error (check error code!)
  virtual UINT32 MDFLIBCALL GetBusChannelNumber(ErrorCode* pErrorCode = NULL) = 0;

  /////////////////////////////////////////////////////
  //! Request if source is only simulated
  //! \note information not available for MDF3
  //! \return true if simulated source, otherwise false   
  virtual bool MDFLIBCALL IsSimulated() = 0;

  ///////////////////////////////////////////////////
  //! Set comment of source
  //! \note only available for newly created or writable file, not available for MDF3
  //! \param[in] pComment pointer to IComment object, NULL to remove reference to a comment object. 
  //!            <br>Note: object must be in same file and if comment is XML for MDF4, it should match the respective XML schema (si_comment.XSD).
  //! \return error code for the operation
  virtual ErrorCode MDFLIBCALL SetComment(IComment* pComment) = 0;

};
