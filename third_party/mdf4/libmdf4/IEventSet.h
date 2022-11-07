/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IEventSet
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
//! \brief Definition of IEventSet

///////////////////////////////////////////////////
//! \brief Collection of events
//!
//! The IEventSet object contains a number of events. 
//! It can be used to filter events associated to a file, a channel group or a channel.
//! <br><br>
//! The events in the set are always sorted by their synchronization value (usually time stamp). 
//! They can be accessed by zero-based index in the set.
//! Further methods allow to filter the events from the set into a new subset.
interface IEventSet
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IEventSet* MDFLIBCALL GetSelf() = 0;  

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //! 
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;
  
  ///////////////////////////////////////////////////
  //! Get number of events in set
  //! \return number of events, 0 if empty or on error
  virtual UINT64 MDFLIBCALL GetCount() = 0;

  ///////////////////////////////////////////////////
  //! Get event from set for given index 
  //! \param[in]  index index of element
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IEvent object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IEvent* MDFLIBCALL GetEventByIndex(UINT64 index, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get a sub set with all events matching the given parameters
  //! \param[in]  eventName name of event to find. If NULL, the name is not considered for filtering.
  //!             <br>If the ::eUseEventNameAsMask bit is set in filterFlags, the string is a search mask with wild cards. 
  //!             <br>The wildcards can be ? (question mark character) for any single character and * (asterisk character) for any number of various characters.
  //              <br>If the ::eAcceptIfNoEventName bit is set in options, events without an extended name prefix are accepted.
  //! \param[in]  filterFlags flags for filtering, value is a bit combination of ::EventFilterFlagsEnum
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IEventSet object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IEventSet* MDFLIBCALL GetEventSubSet(LPCTSTR eventName, UINT32 filterFlags = 0, ErrorCode* pErrorCode = NULL) = 0;

};
