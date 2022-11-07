/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IEvent
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
//! \brief Definition of IEvent

///////////////////////////////////////////////////
//! \brief Description of an event
//!
//! The IEvent object describes an event and its properties. There are different types of events:
//! <ul>
//! <li>events that influenced the recording, such as begin, end or an interruption
//! <li>conditional events, i.e. triggers
//! <li>markers, i.e. comments related to a point in time or a time range
//! </ul>
//! Each event contains a time stamp (or - more generally - a synchronization value) for synchronization
//! with other events or channel groups.
//! Usually an event defines a "point" in time, but for some event types, two events can be linked to define a time "range".
//! See IEvent::GetRangeType() and IEvent::GetRangeEvent().
//! <br><br>
//! The "Scope" of the event defines which channels it applies to. An event can have global scope (event applies to the complete file)
//! or it can only apply to one or more channels or channel groups (i.e. then to all channels in the respective group). See IEvent::GetScopeType().
//! There are special rules for the scope of events that have a parent event or that are the end of a range (range type ::eRangeEnd).
interface IEvent
{

public:

  ///////////////////////////////////////////////////
  //! Get a new reference to this object.
  //!
  //! \return interface pointer for this object.
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IEvent* MDFLIBCALL GetSelf() = 0;

  ///////////////////////////////////////////////////
  //! Release reference to this object.
  //!
  //! See also description for \ref ref_counting.
  //! \return = 0 if this was the last reference.<BR>
  //! > 0 if the object still is referenced by some other object.<BR>
  //! < 0 on error.
  virtual INT32 MDFLIBCALL Release() = 0;

  /////////////////////////////////////////////////////
  //! Get name of event.
  //!
  //! \note Only available for MDF version >= 4.0.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return pointer to string with event name, NULL on error.
  virtual LPCTSTR MDFLIBCALL GetName(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get comment of event.
  //!
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return pointer to IComment object, NULL on error.
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IComment* MDFLIBCALL GetComment(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get type of event.
  //!
  //! \return event type, see ::EventTypeEnum, ::eEventTypeUnspec on error.
  virtual EventType MDFLIBCALL GetType() = 0;

  ///////////////////////////////////////////////////
  //! Get cause of event.
  //!
  //! \return event type, see ::EventCauseEnum, ::eEventCauseUnspec on error.
  virtual EventCause MDFLIBCALL GetCause() = 0;

  ///////////////////////////////////////////////////
  //! Get range type of event.
  //!
  //! The range type specifies if this is a single event (ePoint)
  //! or if it is the begin or end of a range. See also GetRangeEvent().
  //! \return range type, see ::RangeTypeEnum, ::eRangeTypeUnspec on error.
  virtual RangeType MDFLIBCALL GetRangeType() = 0;

  ///////////////////////////////////////////////////
  //! Get synchronization type of event.
  //!
  //! \return sync type, see ::SyncTypeEnum, ::eSyncTypeUnspec on error.
  virtual SyncType MDFLIBCALL GetSyncType() = 0;

  ///////////////////////////////////////////////////
  //! Get synchronization value of event.
  //!
  //! The synchronization value (or sync value) is calculated by multiplication of the sync base value and the sync factor
  //! (see GetSyncBaseValue() and GetSyncFactor()). Its unit depends on the synchronization type (GetSyncType()).
  //! The sync value specifies when the event occurred (relative to start sync value of file). Typically time is used for synchronization.
  //! \note The separation into a base value and a factor has the advantage that a higher precision can be achieved.
  //! For example, when using for time master channels nanoseconds as raw value, the sync base value could be in nanoseconds
  //! and the sync factor 1e-9. Thus it is possible to compare the nanoseconds directly.
  //! Vice versa, a sync base value of 1 could be used to store the time stamp in seconds directly as REAL as sync factor.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return sync value (check error code for warnings).
  virtual REAL MDFLIBCALL GetSyncValue(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get sync base value of event.
  //!
  //! The sync base value is used for calculation of the synchronization value (see GetSyncValue()).
  //! \return sync base value.
  virtual INT64 MDFLIBCALL GetSyncBaseValue() = 0;

  ///////////////////////////////////////////////////
  //! Get sync factor of event.
  //!
  //! The sync factor is used for calculation of the synchronization value (see GetSyncValue()).
  //! \return sync factor.
  virtual REAL MDFLIBCALL GetSyncFactor() = 0;

  ///////////////////////////////////////////////////
  //! Get pre-trigger interval for trigger event as specified in the tool.
  //!
  //! \note The pre-trigger interval is optional and its unit depends on the synchronization type (see GetSyncType()).
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return pre-trigger interval, 0.0 on error or if not available (check error code).
  virtual REAL MDFLIBCALL GetPreTrigInterval(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get post-trigger interval for trigger event as specified in the tool.
  //!
  //! \note The post-trigger interval is optional and its unit depends on the synchronization type (see GetSyncType()).
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return post-trigger interval, 0.0 on error or if not available (check error code).
  virtual REAL MDFLIBCALL GetPostTrigInterval(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get information about creator of event.
  //!
  //! \see IFile::GetCreatorInfo(), IFile::GetFileHistoryLog()
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return pointer to ICreatorInfo object, NULL on error or for MDF version < 4.0.
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual ICreatorInfo* MDFLIBCALL GetCreatorInfo(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get the range partner event for a range begin or end event.
  //!
  //! \note Two events can form a range (see GetRangeType()). A range begin or range end event may also occur without a partner event.
  //! If there is a range partner, the range end event will inherit the scope of the range
  //! begin event and both events must have the same parent event.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return pointer to IEvent object, NULL on error or if not available (check error code).
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IEvent* MDFLIBCALL GetRangeEvent(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get the parent event of current event.
  //!
  //! Event parents can be used to structure events. Another use case is to avoid settings the scope for
  //! each of the child events because if no scope is defined for the child event (empty scope list), then
  //! the child event inherits the scope of its parent event.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return pointer to IEvent object, NULL on error or if not available (check error code).
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IEvent* MDFLIBCALL GetParentEvent(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Check if event is a post processing event.
  //!
  //! \note Post processing flag is only available for MDF version >= 4.0.
  //! \return true if this event was generated by post processing, false otherwise.
  virtual bool MDFLIBCALL IsPostProcessing() = 0;

  ///////////////////////////////////////////////////
  //! Check if event has global scope, i.e. applies to the complete file.
  //!
  //! This is the same as when GetScopeType() returns ::eScopeFileGlobal.
  //! \return true if event has a global scope, false otherwise.
  virtual bool MDFLIBCALL HasGlobalScope() = 0;

  ///////////////////////////////////////////////////
  //! Get scope type of event.
  //!
  //! In MDF4, an event can have global scope (::eScopeFileGlobal) or its
  //! scope is restricted to certain channels or channels groups. The scope is
  //! defined by either a list of channels (::eScopeChannel)
  //! or a list of channel groups (::eScopeGroup).
  //! Depending on the scope type, either use GetScopeListChannelItem() or GetScopeListGroupItem()
  //! to retrieve the channels or channel groups this event applies to.
  //! \return scope type, see ::ScopeTypeEnum, ::eScopeTypeUnspec on error.
  virtual ScopeType MDFLIBCALL GetScopeType() = 0;

  ///////////////////////////////////////////////////
  //! Get the number of elements in the scope list.
  //!
  //! \see GetScopeType()
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return number of elements in scope list, 0 on error or if event has a global scope (check error code).
  virtual UINT32 MDFLIBCALL GetScopeListCount(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get channel in scope list of this event for given index.
  //!
  //! \note This method only returns a valid value if the scope type is ::eScopeChannel.
  //! \see GetScopeListCount() and GetScopeType()
  //! \param[in]  index index of element in scope list.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return pointer to IChannel object, NULL on error.
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannel* MDFLIBCALL GetScopeListChannelItem(UINT32 index, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get channel group in scope list of this event for given index.
  //!
  //! \note This method only returns a valid value if the scope type is ::eScopeGroup.
  //! \see GetScopeListCount() and GetScopeType()
  //! \param[in]  index index of element in scope list.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return pointer to IChannelGroup object, NULL on error.
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IChannelGroup* MDFLIBCALL GetScopeListGroupItem(UINT32 index, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Check if given channel is in scope of event.
  //!
  //! \note This works also if channel has scope type ::eScopeGroup.
  //! \see GetScopeType()
  //! \param[in]  pChannel channel to use for this operation.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return true if channel is in scope of the event or if event has global scope, otherwise or on error false (check error code).
  virtual bool MDFLIBCALL IsInScope(IChannel* pChannel, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get number of attachments related to this event.
  //!
  //! \note There can be attachments for event for MDF version >= 4.0.
  //! \see GetAttachment()
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return number of attachments (0 on error or if not available, e.g. for MDF version < 4.0).
  virtual UINT64 MDFLIBCALL GetAttachmentCount(ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Get attachment related to this event for given index.
  //!
  //! \note Attachments for events are only available for MDF version >= 4.0.
  //! \see GetAttachmentCount()
  //! \param[in]  index index of attachment.
  //! \param[out] pErrorCode if not NULL the error code of the operation will be returned.
  //! \return pointer to IAttachment object, NULL on error.
  //! <br><br>Please remember to call Release() for the returned object or use a smart pointer (see \ref ref_counting).
  virtual IAttachment* MDFLIBCALL GetAttachment(UINT64 index, ErrorCode* pErrorCode = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Set the name for event.
  //!
  //! \note Only available for newly created or writable file, not available for MDF version < 4.0.
  //! \param[in] name string to be used as name, 0 or empty to delete the name.
  //! \return error code for the operation.
  virtual ErrorCode MDFLIBCALL SetName(LPCTSTR name) = 0;

  ///////////////////////////////////////////////////
  //! Set comment of event.
  //!
  //! \note Only available for newly created or writable file.
  //! \param[in] pComment pointer to IComment object, NULL to remove reference to a comment object.
  //!            <br>Note: object must be in the same file and if comment is XML for MDF version >= 4.0, it should match the respective XML schema (ev_comment.XSD).
  //! \return error code for the operation.
  virtual ErrorCode MDFLIBCALL SetComment(IComment* pComment) = 0;

  ///////////////////////////////////////////////////
  //! Set/change synchronization type of event.
  //!
  //! \note Only available for newly created or writable file.
  //! \param[in] syncType synchronization type to use (see ::SyncTypeEnum).
  //!            Note that only values ::eTimeSync, ::eAngleSync, ::eDistanceSync, ::eIndexSync are allowed, but ::eIndexSync cannot be used for
  //!            events with global scope.
  //! \return error code for the operation.
  virtual ErrorCode MDFLIBCALL SetSyncType(SyncType syncType) = 0;

  ///////////////////////////////////////////////////
  //! Set/change synchronization value of event.
  //!
  //! \note Only available for newly created or writable file.
  //! \note It is not allowed that both syncBase and syncFactor are zero.
  //! \see GetSyncValue()
  //! \param[in] syncBase base value to use (see GetSyncBaseValue()).
  //! \param[in] syncFactor factor to use (see GetSyncFactor()).
  //! \return error code for the operation.
  virtual ErrorCode MDFLIBCALL SetSyncValueEx(INT64 syncBase, REAL syncFactor) = 0;

  ///////////////////////////////////////////////////
  //! Set/change synchronization value of event.
  //!
  //! \note Only available for newly created or writable file.
  //! \note The unit of syncValue depends on the synchronization type (see GetSyncType()).
  //! \see GetSyncValue()
  //! \param[in] syncValue value to use as synchronization value.
  //!            <br>Internally, this value will be set as syncFactor and syncBase will be 1.
  //! \return error code for the operation.
  virtual ErrorCode MDFLIBCALL SetSyncValue(REAL syncValue) = 0;

  ///////////////////////////////////////////////////
  //! Change post processing flag of event.
  //!
  //! \note Only available for newly created or writable file.
  //! \param[in] b boolean value for the flag (true to mark this event as post processing event).
  //! \return error code for the operation.
  virtual ErrorCode MDFLIBCALL SetIsPostProcessing(bool b = true) = 0;

  ///////////////////////////////////////////////////
  //! Change this event to a range begin event.
  //!
  //! This changes the range type of event to ::eRangeBegin (see GetRangeType()).
  //! \note Only available for newly created or writable file, not possible for a range end event (range type ::eRangeEnd).
  //! \return error code for the operation.
  virtual ErrorCode MDFLIBCALL SetRangeBegin() = 0;

  ///////////////////////////////////////////////////
  //! Change this event to a range end event and optionally assign a reference to a matching range begin event.
  //!
  //! This changes the range type of the event to ::eRangeEnd (see GetRangeType()).
  //! <br>Optionally a matching range begin event can be assigned (see GetRangeEvent()).
  //! If a range begin event is specified, the current event must not have a scope list because it
  //! will inherit the scope of the range begin event.
  //! In addition, the range begin event must have:
  //! <ul>
  //! <li>range type ::eRangeBegin.
  //! <li>the same event type as the current event (see GetType()).
  //! <li>the same synchronization type as the current event (see GetSyncType()).
  //! <li>a sync value less or equal to the current event (see GetSyncValue()).
  //! <li>the same parent event as the current event.
  //! </ul>
  //! \note Only available for newly created or writable file, not possible for a range begin event (range type ::eRangeBegin).
  //! \param[in] pRangeBegin pointer to IEvent object to assign as begin event, NULL to remove the reference to the range begin event.
  //!            <br>Note: object must be in the same file.
  //! \return error code for the operation.
  virtual ErrorCode MDFLIBCALL SetRangeEnd(IEvent* pRangeBegin) = 0;

  ///////////////////////////////////////////////////
  //! Set or remove a reference to parent event.
  //!
  //! If the event has a range event (see GetRangeEvent()), the parent of the range event is also set or removed.
  //! If a parent event is specified, the current event (and its range event) will inherit the scope of the parent event
  //! unless they have a own scope list.
  //! The scope of the parent event must be larger or equal to the scope of its child events.
  //! There must be no circular dependencies for the parent hierarchy.
  //! \note Only available for newly created or writable file.
  //! \param[in] pEvent pointer to IEvent object to assign as parent event, NULL to remove the reference to the parent event.
  //!            <br>Note: object must be in the same file.
  //! \return error code for the operation.
  virtual ErrorCode MDFLIBCALL SetParentEvent(IEvent* pEvent) = 0;

  ///////////////////////////////////////////////////
  //! Add attachment reference to this event.
  //!
  //! \note Only available for newly created or writable file, not available for MDF version < 4.0.
  //! \param[in] pAttachment pointer to IAttachment object (must not be NULL).
  //!            <br>Note: attachment object must be in the same file and in global list of attachments.
  //!            <br>It is not possible to add an attachment a second time for an event.
  //! \param[out] pIndex if not NULL the zero-based index of the added attachment in the list of event attachments will be returned.
  //! \return error code of the operation.
  virtual ErrorCode MDFLIBCALL AddAttachment(IAttachment* pAttachment, UINT64* pIndex = NULL) = 0;

  ///////////////////////////////////////////////////
  //! Remove attachment reference of given index.
  //!
  //! \note Only available for newly created or writable file, not available for MDF < 4.0.
  //! Keep in mind that the indices of other event attachments may change.
  //! \param[in]  index index of attachment in event attachments.
  //! \return error code of the operation.
  virtual ErrorCode MDFLIBCALL RemoveAttachmentRef(UINT64 index) = 0;

  ///////////////////////////////////////////////////
  //! Remove all references to the given attachment.
  //!
  //! \note Only available for newly created or writable file, not available for MDF version < 4.0.
  //! Keep in mind that the indices of other event attachments may change.
  //! \param[in] pAttachment pointer to IAttachment object (must not be NULL).
  //!            <br>Note: attachment object must be in the same file and in global list of attachments.
  //! \return error code of the operation, \ref eNotFound if the attachment was not referenced by this event.
  virtual ErrorCode MDFLIBCALL RemoveAllAttachmentRefsTo(IAttachment* pAttachment) = 0;

  ///////////////////////////////////////////////////
  //! Set or change the channel scope of event.
  //!
  //! This sets or changes the scope list of an event to scope type ::eScopeChannel.
  //! \note Only available for newly created or writable file,
  //! not possible for a range end event that reference a range begin event or if there is a parent event with global scope.
  //! \param[in] ppChannelList list of pointers to IChannel objects to assign as in scope list (must not be NULL).
  //!            <br>Note: all channel objects must be in the same file
  //! \param[in] listSize size of channel list (must not be 0).
  //! \return error code for the operation.
  virtual ErrorCode MDFLIBCALL SetChannelScope(IChannel** ppChannelList, UINT32 listSize) = 0;

  ///////////////////////////////////////////////////
  //! Set or change the group scope of event
  //!
  //! This sets or changes the scope list of an event to scope type ::eScopeGroup.
  //! \note Only available for newly created or writable file,
  //! not possible for a range end event that reference a range begin event or if there is a parent event with global scope.
  //! \param[in] ppChannelGroupList list of pointers to IChannelGroup objects to assign as in scope list (must not be NULL).
  //!            <br>Note: all channel group objects must be in the same file
  //! \param[in] listSize size of channel group list (must not be 0).
  //! \return error code for the operation.
  virtual ErrorCode MDFLIBCALL SetGroupScope(IChannelGroup** ppChannelGroupList, UINT32 listSize) = 0;

  ///////////////////////////////////////////////////
  //! Get start addresses of block representing the event and optional internal index of event within this block
  //!
  //! Note: for MDF4, this is the address of the respective EV block, for MDF3 it can be the address of a TR or a CN block, depending on the event type
  //! \param[out] pIndexOfEventInBlock if not NULL the zero-based index of the event within the block will be returned.
  //!             <br>This is only relevant for MDF3 where a TR or CN block may contain several events.
  //!             <br>For MDF4 EV block, this will always return 0.
  //! \return returns start address of respective EV/TR/CN block for this event, -1 on error.
  virtual INT64 MDFLIBCALL GetBlockStartAddress(UINT32* pIndexOfEventInBlock = NULL) = 0;

};

