/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IChannelSet
-------------------------------------------------------------------------------
MDF4Lib public unmanaged C++ interfaceIChannelGroup
See API documentation for details
-------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
-----------------------------------------------------------------------------*/
#pragma once

#include "MDF4LibDataTypes.h"

///////////////////////////////////////////////////
//! \file
//! \brief Definition of IChannelSet

///////////////////////////////////////////////////
//! \brief Collection of channels
//!
//! The IChannelSet object contains a number of channels.
//! It can be used to filter channels from a file, a channel group, or from the children of a channel (i.e. structure members or array elements).
//! <br><br>
//! The channels can be accessed by zero-based index in the set or by name.
//! A further method filters the channels from the set into a new subset.
interface IChannelSet
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object
  //!
  //! \return interface pointer for this object
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannelSet* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object
  //!
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference<BR>
  //! > 0 if the object still is referenced by some other object<BR>
  //! < 0 on error
  virtual INT32 MDFLIBCALL Release() = 0;

  ///////////////////////////////////////////////////
  //! Get number of channels in set
  //! \return number of channels, 0 if empty or on error
  virtual UINT64 MDFLIBCALL GetCount() = 0;

  ///////////////////////////////////////////////////
  //! Get channel from set for given index
  //! \param[in]  index index of element
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IChannel object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL GetChannelByIndex(UINT64 index, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get first channel from set that matches the given name
  //! \note: this method uses case-sensitive comparison. For case-insensitive comparison use GetChannelSubSet() with flag ::eCompareNoCase.
  //! <br>If ::eUseBinarySearchForChannelName has been specified for Get(Child)ChannelSet method (or ::eUseBinarySearchForResultSet for GetChannelSubSet()),
  //! this method uses a binary search to find the channel name. In this case the first call to GetChannelName() takes longer
  //! because all channels first must be sorted according to their names.
  //! If there are several channel with equal name, the binary search will find one of them but not necessarily the first one.
  //! \param[in]  channelName name of channel to find
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IChannel object, NULL if no channel found or on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL GetChannelByName(LPCTSTR channelName, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get a sub set with all channels matching the given parameters
  //! \param[in]  channelName name of channel to find.
  //!             <br>If the ::eUseChannelNameAsMask bit is set in filterFlags, the string is a search mask with wild cards.
  //!             <br>The wildcards can be ? (question mark character) for any single character and * (asterisk character) for any number of various characters.
  //! \param[in]  extendedNamePrefix if given, the extended name prefix also must match (see IChannel::GetExtendedNamePrefix).
  //!             <br>If the ::eUseNamePrefixAsMask bit is set in filterFlags, the string is a search mask with wild cards.
  //!             <br>Like for channelName, the wildcards can be ? (question mark character) for any single character and * (asterisk character) for any number of various characters.
  //!             <br>If the ::eAcceptIfNoExtension bit is set in options, channels without an extended name prefix are accepted.
  //! \param[in]  filterFlags flags for filtering, value is a bit combination of ::ChannelSubSetFilterFlagsEnum
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned
  //! \return pointer to IChannelSet object, NULL on error
  //! <br><br>Please remember to call Release for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannelSet* MDFLIBCALL GetChannelSubSet(LPCTSTR channelName, LPCTSTR extendedNamePrefix = NULL, UINT32 filterFlags = 0, ErrorCode* pErrorCode = NULL) = 0;

};
