/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces:
-------------------------------------------------------------------------------
MDF4Lib public unmanaged C++ interface: enumerations
See API documentation for details
-------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
-----------------------------------------------------------------------------*/

#pragma once

// RULE: never change existing enums. Extend only!

//! \file
//! \brief Enumerations for settings / result information
//! contains all enumerations except of ErrorCode

///////////////////////////////////////////////////
//! XML data types for IMetaInfo
enum XmlDataTypeEnum
{
    eXmlDataTypeUnspec       = -1   //!< Unspecified
  , eXmlString               =  0   //!< String
  , eXmlDecimal              =  1   //!< Decimal
  , eXmlInteger              =  2   //!< Integer
  , eXmlFloat                =  3   //!< Float
  , eXmlBoolean              =  4   //!< Boolean
  , eXmlDate                 =  5   //!< Date
  , eXmlTime                 =  6   //!< Time
  , eXmlDateTime             =  7   //!< DateTime

};

///////////////////////////////////////////////////
//! MDF channel data types
//!
//! LE = Little Endian (Intel) Byte order
//! BE = Big Endian (Motorola) Byte order
//! enum values are equal to MDF4 enum values for cn_data_type
enum ChannelDataTypeEnum
{
    eChannelDataTypeUnspec   = -1   //!< Unspecified

  // Integer data types
  , eInteger_Unsigned_LE     =  0   //!< unsigned integer with LE Byte order (Intel)
  , eInteger_Unsigned_BE     =  1   //!< unsigned integer with BE Byte order (Motorola)
  , eInteger_Signed_LE       =  2   //!< signed integer (two's complement) with LE Byte order (Intel)
  , eInteger_Signed_BE       =  3   //!< signed integer (two's complement) with BE Byte order (Motorola)

  // Floating-point data types
  , eReal_LE                 =  4   //!< IEEE 754 floating-point format with LE Byte order (Intel)
  , eReal_BE                 =  5   //!< IEEE 754 floating-point format with BE Byte order (Motorola)

  // String data types
  , eString_SBC              =  6   //!< String (SBC, standard ASCII encoded (ISO-8859-1 Latin), NULL terminated)
  , eString_UTF8             =  7   //!< String (UTF-8 encoded, NULL terminated)
  , eString_UTF16_LE         =  8   //!< String (UTF-16 encoded LE Byte order (Intel), NULL terminated)
  , eString_UTF16_BE         =  9   //!< String (UTF-16 encoded BE Byte order (Motorola), NULL terminated)

  // Complex data types
  , eByteArray               = 10   //!< Byte Array with unknown content (e.g. structure)
  , eMimeSample              = 11   //!< MIME sample (sample is Byte Array with MIME content-type specified in unit of IChannel)
  , eMimeStream              = 12   //!< MIME stream (all samples of channel represent a stream with MIME content-type specified in unit of IChannel)
  , eCANOpenDate             = 13   //!< CANopen date (Based on 7 Byte CANopen Date data structure)
  , eCANOpenTime             = 14   //!< CANopen time (Based on 6 Byte CANopen Time data structure)
  , eComplexReal_LE          = 15   //!< Complex number with two consecutive values (re,im), each in IEEE 754 floating-point format with LE Byte order (Intel)   => introduced in MDF 4.2
  , eComplexReal_BE          = 16   //!< Complex number with two consecutive values (re,im), each in IEEE 754 floating-point value with BE Byte order (Motorola) => introduced in MDF 4.2

};

///////////////////////////////////////////////////
//! MDF channel types
//! enum values are equal to MDF4 enum values for cn_type
enum ChannelTypeEnum
  {
    eChannelTypeUnspec       = -1   //!< Unspecified
  , eFixedLength             =  0   //!< Fixed length data channel
  , eVariableLength          =  1   //!< Variable length data channel
  , eMaster                  =  2   //!< Master channel for all signals of this group
  , eVirtualMaster           =  3   //!< Virtual master channel
  , eSynchronization         =  4   //!< Synchronization channel
  , eMaximumLength           =  5   //!< Maximum length data channel (introduced in MDF 4.1)
  , eVirtualData             =  6   //!< Virtual data channel (introduced in MDF 4.1)

};

///////////////////////////////////////////////////
//! MDF synchronization types
//!
//! enum values are equal to MDF4 enum values for cn_sync_type
enum SyncTypeEnum
{
    eSyncTypeUnspec          = -1   //!< Unspecified
  , eNoSync                  =  0   //!< No synchronization
  , eTimeSync                =  1   //!< Synchronization by time     (physical values must be seconds)
  , eAngleSync               =  2   //!< Synchronization by angle    (physical values must be radiants)
  , eDistanceSync            =  3   //!< Synchronization by distance (physical values must be meters)
  , eIndexSync               =  4   //!< Synchronization by index    (physical values must be zero-based index)

};


///////////////////////////////////////////////////
//! MDF channel array type
//!
//! enum values are equal to MDF4 enum values for ca_type
enum ArrayTypeEnum
{
    eArrayTypeUnspec         = -1    //!< Unspecified
  , eIsValueArray            =  0    //!< The array is a simple D-dimensional value array (value block)
                                     //!< without axes and without input/output/comparison quantities.
  , eIsScalingAxis           =  1    //!< The array is a scaling axis (1-dimensional vector), possibly referenced by one or more arrays.<br>
                                     //!< If referenced by an array of type ::eIsLookUp, the axis itself may have a scaling axis (e.g. for a "curve axis") and an own input quantity.
  , eIsLookUp                =  2    //!< The array is a D-dimensional array with axes.
                                     //!< It can have input/output/comparison quantities.
  , eIsIntervalAxis          =  3    //!< The array is an axis (1-dimensional vector) defining interval ranges as "axis points". <br>
                                     //!< It can be referenced by one or more arrays of type ::eIsClassificationResult.
                                     //!< In contrast to a scaling axis (::eIsScalingAxis), an interval axis always has one element more than the number<br>
                                     //!< of elements for the respective dimension of the classification result array which references it.<br>
                                     //!< The elements of the class interval axis define the borders of interval ranges that are seen as axis points.<br>
                                     //!< Depending on the ::eHasLeftOpenInterval flag, the intervals are defined as left-open/right-closed or left-closed/right-open.<br>
                                     //!< This type is not valid for MDF < 4.1
  , eIsClassificationResult  =  4    //!< The array is a D-dimensional array containing classification results.<br>
                                     //!< It can have scaling axes (::eIsScalingAxis) or interval axes (::eIsIntervalAxis), even mixed. <br>
                                     //!< This type is not valid for MDF < 4.1
};

///////////////////////////////////////////////////
//! Bit flags for array properties
//!
//! enum values are equal to MDF4 bit positions for ca_flags
enum ArrayPropertyFlagsEnum
{
    eHasDynamicSize        = 1 << 0  //!< If set, the number of scaling points for the array is not fixed but can vary over time. <br>
                                     //!< In this case each dimension can have a size signal (IArrayInfo::GetDynamicSizeChannel / IArrayInfo::SetDynamicSizeChannel). <br>
                                     //!< Can only be set for array types ::eIsLookUp and ::eIsScalingAxis.
  , eHasInputQuantity      = 1 << 1  //!< If set, a channel for the input quantity can be specified for each dimension. <br>
                                     //!< In this case each dimension can have an input quantity signal (IArrayInfo::GetInputQuantityChannel / IArrayInfo::SetInputQuantityChannel). <br>
                                     //!< Can only be set for array types ::eIsLookUp and ::eIsScalingAxis.
  , eHasOutputQuantity     = 1 << 2  //!< If set, a channel for the output quantity can be specified (IArrayInfo::GetOutputQuantityChannel / IArrayInfo::SetOutputQuantityChannel). <br>
                                     //!< Can only be set for array types ::eIsLookUp.
  , eHasComparisonQuantity = 1 << 3  //!< If set, a channel for the comparison quantity can be specified (IArrayInfo::GetComparisonQuantityChannel / IArrayInfo::SetComparisonQuantityChannel). <br>
                                     //!< Can only be set for array types ::eIsLookUp.
  , eHasAxis               = 1 << 4  //!< If set, a scaling axis can be given for each dimension of the array, either as fixed or as dynamic axis, depending on ::eHasFixedAxis flag.<br>
                                     //!< Can only be set for array types ::eIsLookUp, ::eIsScalingAxis and ::eIsClassificationResult.
  , eHasFixedAxis          = 1 << 5  //!< If set, the scaling axis is fixed and the axis points are stored directly in the IArrayInfo (IArrayInfo::GetFixAxisRawValues / IArrayInfo::SetFixAxisRawValues).<br>
                                     //!< If not set, the scaling axis may vary over time and the axis points are stored as channel for each dimension (IArrayInfo::GetAxisChannel / IArrayInfo::SetAxisChannel). <br>
                                     //!< Only relevant if ::eHasAxis is set. Can only not be set for array types ::eIsLookUp and ::eIsScalingAxis.
  , eHasInverseLayout      = 1 << 6  //!< If set, the record layout is "column oriented" instead of "row oriented".<br>
                                     //!< Only of the number of dimensions is more than 1.
  , eHasLeftOpenInterval   = 1 << 7  //!< If set, the interval ranges for the class interval axes are left-open and right-closed,
                                     //!< i.e. ]a,b] = {x | a < x <= b}. <br>
                                     //!< If not set, the interval ranges for the class interval axes are left-closed and right-open,
                                     //!< i.e. [a,b[ = {x | a <= x < b}. <br>
                                     //!< Only relevant for array type ::eIsIntervalAxis
};

///////////////////////////////////////////////////
//! MDF channel composition type
enum CompositionTypeEnum
{
    eCompositionTypeUnspec   = -1   //!< Unspecified
  , eNoComposition           =  0   //!< No composition
  , eIsStruct                =  1   //!< Struct (compact structure)
  , eIsArray                 =  2   //!< Array

};

///////////////////////////////////////////////////
//! MDF channel component type
enum ComponentTypeEnum
{
    eComponentTypeUnspec     = -1   //!< Unspecified
  , eNoComponent             =  0   //!< No component
  , eIsStructMember          =  1   //!< Member of a struct (compact structure)
  , eIsArrayElement          =  2   //!< Element of an array

};


///////////////////////////////////////////////////
//! Source type
//!
//! enum values are equal to MDF4 enum values for si_type
enum SourceTypeEnum
{
    eSourceTypeUnspec        = -1   //!< Unspecified
  , eSourceOther             =  0   //!< Source type does not fit into given categories or is unknown
  , eSourceEcu               =  1   //!< Source is an ECU
  , eSourceBus               =  2   //!< Source is a bus (eg. for bus monitoring)
  , eSourceIO                =  3   //!< Source is an I/O device (eg analog I/O)
  , eSourceTool              =  4   //!< Source is a software tool (eg for tool generated signals/events)
  , eSourceUser              =  5   //!< Source is a user interaction/input (eg for user generated events)

};

///////////////////////////////////////////////////
//! Source bus type
//!
//! enum values are equal to MDF4 enum values for si_bus_type
enum SourceBusTypeEnum
{
    eSourceBusTypeUnspec     = -1   //!< Unspecified
  , eBusNone                 =  0   //!< No bus
  , eBusOther                =  1   //!< Bus type does not fit into given categories or is unknown
  , eBusCAN                  =  2   //!< CAN bus
  , eBusLIN                  =  3   //!< LIN bus
  , eBusMOST                 =  4   //!< MOST bus
  , eBusFLEXRAY              =  5   //!< FLEXRAY bus
  , eBusKLine                =  6   //!< K-Line bus
  , eBusEthernet             =  7   //!< Ethernet bus
  , eBusUSB                  =  8   //!< USB bus

};

///////////////////////////////////////////////////
//! MDF event type
//!
//! enum values are equal to MDF4 enum values for ev_type
enum EventTypeEnum
{
    eEventTypeUnspec         = -1   //!< Unspecified
  , eRecording               =  0   //!< This event type specifies a recording period, i.e. the first and last time a signal value could theoretically be recorded.
  , eRecordingInterrupt      =  1   //!< This event type indicates that the recording has been interrupted.
  , eAcquisitionInterrupt    =  2   //!< This event type indicates that not only the recording, but already the acquisition of the signal values has been interrupted.
  , eStartRecordingTrigger   =  3   //!< This event type specifies an event which started the recording of signal values due to some condition
  , eStopRecordingTrigger    =  4   //!< Symmetrically to the "start recording trigger" event type, this event type specifies an event which stopped the recording of signal values due to some condition.
  , eTrigger                 =  5   //!< This event type generally specifies an event that occurred due to some condition (except of the special start and stop recording trigger event types).
  , eMarker                  =  6   //!< This event type specifies a marker for a point or a range.

};

///////////////////////////////////////////////////
//! Bit flags to filter events
//! used by GetEventSet and GetEventSubSet methods
enum EventFilterFlagsEnum
{

    eIncludeAllEventTypes               =       0   //!< if none of the Include flags for an event type is set, all event types are included automatically.
                                                    //!< <br>same for eIncludeEventsWithSyncTypeXXX flags: if none of these flags is set, all synchronization types are included.
                                                    //!< <br>same for eIncludeEventsWithEventCauseXXX flags: if none of these flags is set, all event causes are included.

  // flags for including only certain event types
  , eIncludeRecordingEvents             =  1 << 0   //!< If set, events with type ::eRecording will be included
  , eIncludeRecordingInterruptEvents    =  1 << 1   //!< If set, events with type ::eRecordingInterrupt will be included
  , eIncludeAcquisitionInterruptEvents  =  1 << 2   //!< If set, events with type ::eAcquisitionInterrupt will be included
  , eIncludeStartRecordingTriggerEvents =  1 << 3   //!< If set, events with type ::eStartRecordingTrigger will be included
  , eIncludeStopRecordingTriggerEvents  =  1 << 4   //!< If set, events with type ::eStopRecordingTrigger will be included
  , eIncludeTriggerEvents               =  1 << 5   //!< If set, events with type ::eTrigger will be included
  , eIncludeMarkerEvents                =  1 << 6   //!< If set, events with type ::eMarker will be included

  // bit combination for convenience
  , eIncludeAllTriggerEventTypes        = eIncludeStartRecordingTriggerEvents
                                        | eIncludeStopRecordingTriggerEvents
                                        | eIncludeTriggerEvents
                                                     //!< If set, all trigger event types are included (::eStartRecordingTrigger, ::eStopRecordingTrigger, ::eTrigger)

  // flags for including only certain synchronization types
  , eIncludeEventsWithSyncTypeTime      =  1 << 12   //!< If set, events with synchronization type ::eTimeSync will be inlcuded
  , eIncludeEventsWithSyncTypeAngle     =  1 << 13   //!< If set, events with synchronization type ::eAngleSync will be inlcuded
  , eIncludeEventsWithSyncTypeDistance  =  1 << 14   //!< If set, events with synchronization type ::eDistanceSync will be inlcuded
  , eIncludeEventsWithSyncTypeIndex     =  1 << 15   //!< If set, events with synchronization type ::eIndexSync will be inlcuded

  // flags for including only certain event causes
  , eIncludeEventsWithEventCauseOther   =  1 << 16   //!< If set, events with event cause ::eEventCauseOther will be inlcuded
  , eIncludeEventsWithEventCauseError   =  1 << 17   //!< If set, events with event cause ::eEventCauseError will be inlcuded
  , eIncludeEventsWithEventCauseTool    =  1 << 18   //!< If set, events with event cause ::eEventCauseTool will be inlcuded
  , eIncludeEventsWithEventCauseScript  =  1 << 19   //!< If set, events with event cause ::eEventCauseScript will be inlcuded
  , eIncludeEventsWithEventCauseUser    =  1 << 20   //!< If set, events with event cause ::eEventCauseUser will be inlcuded

  // flags considering the event scope
  , eOnlyEventsWithGlobalScope          =  1 << 23   //!< If set, only events with global scope are included (mutual exclusive with ::eOnlyEventsWithGlobalScope)
  , eExcludeEventsWithGlobalScope       =  1 << 24   //!< If set, events with global scope are excluded (mutual exclusive with ::eOnlyEventsWithGlobalScope)

  // flags considering the post processing flag
  , eOnlyPostProcessingEvents          =  1 << 25   //!< If set, only events marked as "post processing" event are included (mutual exclusive with ::eExcludePostProcessingEvents)
  , eExcludePostProcessingEvents       =  1 << 26   //!< If set, events marked as "post processing" event are excluded (mutual exclusive with ::eOnlyPostProcessingEvents)

  // flags only relevant for IEventSet::GetEventSubSet method
  , eUseEventNameAsMask                 =  1 << 28   //!< If set, the name is treated as search mask with wild cards.
                                                     //!< Only relevant for GetEventSubSet method and if the eventName is not NULL.
                                                     //!< <br>The wildcards can be ? (question mark character) for any single character and * (asterisk character) for any number of various characters.
  , eCompareEventNameNoCase             =  1 << 29   //!< If set, the name comparison is not done case sensitive.
                                                     //!< Only relevant for GetEventSubSet method and if the eventName is not NULL.
  , eAcceptIfNoEventName                =  1 << 30   //!< If set, events without name are accepted in case a name filter is given, otherwise they are excluded.
                                                     //!< Only relevant for GetEventSubSet method and if the eventName is not NULL.

};


///////////////////////////////////////////////////
//! Bit flags to filter events
//! used by GetCommentFieldSet
enum CommentFieldFilterFlagsEnum
{
    eIncludeAllFieldTypes     = 0        //!< if none of the Include flags is set, all comment field types are included automatically.

  // flags for including only certain comment field types
  , eIncludeSingleValueFields = 1 << 0   //!< If set, only single-value comment fields will be included (corresponds to &lt;e&gt; tag in MDF 4.x or a field in MDF 3.x)
  , eIncludeMultiValueFields  = 1 << 1   //!< If set, only multi-value comment fields will be included (corresponds to &lt;elist&gt; tag in MDF 4.1 or higher)

};

///////////////////////////////////////////////////
//! MDF scope type for events
enum ScopeTypeEnum
{
    eScopeTypeUnspec         = -1   //!< Unspecified
  , eScopeFileGlobal         =  0   //!< Global scope (complete file)
  , eScopeGroup              =  1   //!< Scope on channel group level
  , eScopeChannel            =  2   //!< Scope on channel level

};

///////////////////////////////////////////////////
//! MDF hierarchy type
//!
//! enum values are equal to MDF4 enum values for ch_type
enum HierarchyTypeEnum
{
    eHierarchyTypeUnspec     = -1   //!< Unspecified
  , eGroup                   =  0   //!< All elements and children of this hierarchy level form a logical group (see ASAM MCD-2 MC keyword GROUP)
  , eFunction                =  1   //!< All children of this hierarchy level form a functional group (see ASAM MCD-2 MC keyword FUNCTION)
  , eStructure               =  2   //!< All elements and children of this hierarchy level form a "fragmented" structure.
  , eMapList                 =  3   //!< All elements of this hierarchy level form a map list (see ASAM MCD-2 MC key word MAP_LIST)
  , eInputVariables          =  4   //!< Input variables of function(see ASAM MCD-2 MC keyword IN_MEASUREMENT)
  , eOutputVariables         =  5   //!< Output variables of function(see ASAM MCD-2 MC keyword OUT_MEASUREMENT)
  , eLocalVariables          =  6   //!< Local variables of function (see ASAM MCD-2 MC keyword LOC_MEASUREMENT)
  , eCalibrationDef          =  7   //!< Calibration objects defined in function (see ASAM MCD-2 MC keyword DEF_CHARACTERISTIC)
  , eCalibrationRef          =  8   //!< Calibration objects referenced in function (see ASAM MCD-2 MC keyword REF_CHARACTERISTIC)

};

///////////////////////////////////////////////////
//! MDF event cause
//!
//! enum values are equal to MDF4 enum values for ev_cause
enum EventCauseEnum
{
    eEventCauseUnspec        = -1   //!< Unspecified
  , eEventCauseOther         =  0   //!< Cause of event is not known or does not fit into given categories
  , eEventCauseError         =  1   //!< Event was caused by some error
  , eEventCauseTool          =  2   //!< Event was caused by tool-internal condition,
                                    //!< e.g. trigger condition or re-configuration
  , eEventCauseScript        =  3   //!< Event was caused by a scripting command
  , eEventCauseUser          =  4   //!< Event was caused directly by user
                                    //<! e.g. user input or some other interaction with GUI

};

///////////////////////////////////////////////////
//! MDF event range type
//!
//! enum values are equal to MDF4 enum values for ev_range_type
enum RangeTypeEnum
{
    eRangeTypeUnspec         = -1   //!< Unspecified
  , ePoint                   =  0   //!< Event defines a point (single event)
  , eRangeBegin              =  1   //!< Event defines the beginning of a range
  , eRangeEnd                =  2   //!< Event defines the end of a range

};

///////////////////////////////////////////////////
//! MDF conversion type
//!
//! enum values are equal to MDF4 enum values for cc_type
//! special enum values for MDF3 types
enum ConversionTypeEnum
{
    eConversionTypeUnspec    = -1   //!< Unspecified
  , eIdentity                =  0   //!< 1:1 conversion
  , eLinear                  =  1   //!< Linear conversion
  , eRational                =  2   //!< Rational conversion
  , eAlgebraic               =  3   //!< Algebraic conversion (ASAM MCD-2 MC text formula)
  , eValue2ValueInterpol     =  4   //!< Value to value tabular look-up with interpolation
  , eValue2ValueNoInterpol   =  5   //!< Value to value tabular look-up without interpolation
  , eValueRange2Value        =  6   //!< Value range to value tabular look-up
  , eValue2Text              =  7   //!< Value to text/scale conversion tabular look-up
  , eValueRange2Text         =  8   //!< Value range to text/scale conversion tabular look-up
  , eText2Value              =  9   //!< Text to value tabular look-up
  , eText2Text               = 10   //!< Text to text tabular look-up (translation)
  , eBitFieldTextTable       = 11   //!< Bitfield text table (introduced in MDF 4.2)

  //!@{ MDF 3 specific formulas
  , ePolynomial              = 97   //!< Polynomial
  , eExponential             = 98   //!< Exponential
  , eLogarithmic             = 99   //!< Logarithmic
  //!@}
};

///////////////////////////////////////////////////
//! Type of conversion result
enum ConversionResultTypeEnum
{
    eConversionResultTypeUnspec        = -1   //!< Unspecified
  , eConversionResultReal              =  0   //!< Conversion result is a floating-point value
  , eConversionResultText              =  1   //!< Conversion result is a string
  , eConversionResultMixed             =  2   //!< Conversion result can be mixed,
                                              //!< i.e. floating-point value or string
  , eConversionResultError             =  3   //!< Conversion result is always an error
                                              //!< (e.g. parsing error for algebraic conversion)
  , eConversionResultZero              =  4   //!< Conversion result is always zero
  , eConversionResultDivByZero         =  5   //!< Conversion result is always division by zero
  , eConversionResultLogZero           =  6   //!< Conversion result is always logarithm of zero error
};

///////////////////////////////////////////////////
//! Type of attachment
//!
//! enum values are equal to MDF4 bit positions for at_flags
enum AttachmentTypeEnum
{
    eAttachmentTypeUnspec    = -1   //!< Unspecified
  , eExternal                =  0   //!< The attachment data it is contained in an external file
  , eEmbedded                =  1   //!< The attachment data is embedded
  , eEmbeddedCompressed      =  3   //!< The attachment data is embedded and is compressed using the deflate algorithm

};

///////////////////////////////////////////////////
//! Type of compression algorithm for data blocks
//!
//! enum values are equal to MDF4 enum values for dz_zip_type/hl_zip_type
enum ZipTypeEnum
{
    eZipTypeUnspec           = -1   //!< Unspecified / no compression
  , eDeflate                 =  0   //!< deflate algorithm
  , eTransposeDeflate        =  1   //!< transposition + deflate algorithm
};

///////////////////////////////////////////////////
//! Type and level of compression algorithm for creation of IDataWriter
//!
//! enum values combine the ::ZipTypeEnum (LOWORD) and the compression level (HIWORD)
enum ZipTypeAndLevelEnum
{
    eNoCompression              = -1                              //!< No compression
  , eDeflate_Default            = eDeflate                        //!< eDeflate with default compression level
  , eDeflate_1                  = eDeflate | (1 << 16)            //!< eDeflate with compression level 1 (best speed)
  , eDeflate_2                  = eDeflate | (2 << 16)            //!< eDeflate with compression level 2
  , eDeflate_3                  = eDeflate | (3 << 16)            //!< eDeflate with compression level 3
  , eDeflate_4                  = eDeflate | (4 << 16)            //!< eDeflate with compression level 4
  , eDeflate_5                  = eDeflate | (5 << 16)            //!< eDeflate with compression level 5
  , eDeflate_6                  = eDeflate | (6 << 16)            //!< eDeflate with compression level 6
  , eDeflate_7                  = eDeflate | (7 << 16)            //!< eDeflate with compression level 7
  , eDeflate_8                  = eDeflate | (8 << 16)            //!< eDeflate with compression level 8
  , eDeflate_9                  = eDeflate | (9 << 16)            //!< eDeflate with compression level 9 (best compression)
  , eTransposeDeflate_Default   = eTransposeDeflate               //!< eTransposeDeflate with default compression level
  , eTransposeDeflate_1         = eTransposeDeflate | (1 << 16)   //!< eTransposeDeflate with compression level 1 (best speed)
  , eTransposeDeflate_2         = eTransposeDeflate | (2 << 16)   //!< eTransposeDeflate with compression level 2
  , eTransposeDeflate_3         = eTransposeDeflate | (3 << 16)   //!< eTransposeDeflate with compression level 3
  , eTransposeDeflate_4         = eTransposeDeflate | (4 << 16)   //!< eTransposeDeflate with compression level 4
  , eTransposeDeflate_5         = eTransposeDeflate | (5 << 16)   //!< eTransposeDeflate with compression level 5
  , eTransposeDeflate_6         = eTransposeDeflate | (6 << 16)   //!< eTransposeDeflate with compression level 6
  , eTransposeDeflate_7         = eTransposeDeflate | (7 << 16)   //!< eTransposeDeflate with compression level 7
  , eTransposeDeflate_8         = eTransposeDeflate | (8 << 16)   //!< eTransposeDeflate with compression level 8
  , eTransposeDeflate_9         = eTransposeDeflate | (9 << 16)   //!< eTransposeDeflate with compression level 9 (best compression)
};


///////////////////////////////////////////////////
//! bus event type contained in channel group / file (available since MDF 4.1)
enum BusEventTypeEnum
{
    eBusEventUnspec             = -1   //!< Unspecified
  , eBusEventNone               =  0   //!< Channel group does not contain a bus event / none of the channel groups in the file contains a bus event
  , eBusEventPlain              =  1   //!< Channel group contains / all channel groups in the file contain only a general bus event but no other signals
  , eBusEventAndSignals         =  2   //!< Channel group contains a bus event and other signals, e.g. data frame or PDU including signal descriptions for payload
                                       //!< / file contains at least one channel group with a bus event, but also at least one channel group that has no bus event (eBusEventNone for CG) or a bus event and other signals (eBusEventAndSignals for CG)

};


///////////////////////////////////////////////////
//! Type of time stamp
enum TimeStampTypeEnum
{
    eTimeStampTypeUnspec     = -1   //!< Unspecified
  , eUtcTime                 =  0   //!< Time stamp is UTC time, offsets are not valid
  , eLocalTime               =  1   //!< Time stamp is local time, offsets are not valid
  , eUtcTimeWithOffsets      =  2   //!< Time stamp is UTC time, offsets are valid
  , eLocalTimeWithOffsets    =  3   //!< Time stamp is local time, offsets are valid (not allowed in MDF4)

};

///////////////////////////////////////////////////
//! Bit flags of an MDF channel
//!
//! enum values are equal to MDF4 bit positions for cn_flags
enum ChannelFlagsEnum
{
    eFlagAllValuesInvalid    =  1 <<  0   //!< If set, all values of this channel are invalid
  , eFlagInvalBitUsed        =  1 <<  1   //!< If set, this channel uses an invalidation bit
  , eFlagPrecisionValid      =  1 <<  2   //!< If set, the precision value for display of floating-point values is valid
  , eFlagValueRangeValid     =  1 <<  3   //!< If set, the value range is valid
  , eFlagLimitRangeValid     =  1 <<  4   //!< If set, the limit range is valid
  , eFlagLimitExtRangeValid  =  1 <<  5   //!< If set, the extended limit range is valid
  , eFlagDiscrete            =  1 <<  6   //!< If set, the signal values of this channel are discrete and must not be interpolated
  , eFlagCalibration         =  1 <<  7   //!< If set, the signal values of this channel correspond to a calibration object, otherwise to a measurement object
  , eFlagCalculated          =  1 <<  8   //!< If set, the values of this channel have been calculated from other channel inputs
  , eFlagVirtual             =  1 <<  9   //!< If set, this channel is virtual, i.e. it is simulated by the recording tool
  , eFlagBusEvent            =  1 << 10   //!< If set, this channel contains information about a bus event (introduced in MDF 4.1 for bus logging)
  , eFlagMonotonous          =  1 << 11   //!< If set, the values for this channel are strictly monotonously increasing/decreasing (introduced in MDF 4.1)
  , eFlagDefaultX            =  1 << 12   //!< If set, the channel references some other channel to be preferably used as X axis by IChannel::GetDefaultXChannel (introduced in MDF 4.1)
  , eFlagEventSignal         =  1 << 13   //!< If set, the channel is used for description of events in an event signal group (introduced in MDF 4.2)
  , eFlagVlsdDataStream      =  1 << 14   //!< If set, the VLSD data is stored in SDBLOCK as data stream, i.e. without gaps and in correct ordering. Only valid for VLSD channels. (introduced in MDF 4.2)
};


///////////////////////////////////////////////////
//! Bit flags to filter channels
//! used by GetChannelSet methods
enum ChannelFilterFlagsEnum
{
    eIncludeMasterChannels      =  1 << 0   //!< If set, master channels will be included in the set, otherwise only value channels are included
  , eIncludeChildStructMembers  =  1 << 1   //!< If set, child channels of structures will be included in the set
  , eIncludeChildArrayElements  =  1 << 2   //!< If set, child channels of arrays will be included in the set
  , eIncludeChildsRecursively   =  1 << 3   //!< If set, the flags eIncludeChildStructMembers and eIncludeChildArrayElements will be applied recursively
  , eExcludeBusEventChannels    =  1 << 4   //!< If set, channels are excluded if flag eFlagBusEvent is set     (MDF 4.1 bus logging)
  , eExcludeNonBusEventChannels =  1 << 5   //!< If set, channels are excluded if flag eFlagBusEvent is not set (MDF 4.1 bus logging)
                                            //!< eExcludeNonBusEventChannels and eExcludeBusEvents must not be set together!

  , eUseBinarySearchForChannelName = 1 << 8 //!< If set, for the IChannelSet result each call to IChannelSet::GetChannelName uses a binary search.
                                            //!< This flag can be used to improve performance in case several calls to GetChannelName are expected.
                                            //!< Note that the first call to GetChannelName takes longer since the channels must be sorted according to the channel names first.


  // bit combination for convenience
  , eAllDataChannels            = eIncludeChildStructMembers
                                | eIncludeChildArrayElements
                                | eIncludeChildsRecursively
                                            //!< bit combination to get all "data" channels, i.e. without master channels
  , eAllChannels                = eIncludeMasterChannels | eAllDataChannels
                                            //!< bit combination to get all channels, i.e. including master channels
};

///////////////////////////////////////////////////
//! Bit flags to filter channels
//! used by GetChannelSubSet methods
enum ChannelSubSetFilterFlagsEnum
{
    eAcceptIfNoExtension         =  1 << 0   //!< If set, channels without an extended name prefix are accepted even if parameter extendedNamePrefix is specified.
  , eUseChannelNameAsMask        =  1 << 1   //!< If set, the name is treated as search mask with wild cards.
                                             //!< <br>The wildcards can be ? (question mark character) for any single character and * (asterisk character) for any number of various characters.
  , eUseNamePrefixAsMask         =  1 << 2   //!< If set, the name prefix is treated as search mask with wild cards.
                                             //!< <br>The wildcards can be ? (question mark character) for any single character and * (asterisk character) for any number of various characters.
  , eCompareNoCase               =  1 << 3   //!< If set, the name comparison is not done case sensitive

  , eUseBinarySearchForResultSet =  1 << 8   //!< If set, for the IChannelSet result each call to IChannelSet::GetChannelName uses a binary search.
                                             //!< This flag can be used to improve performance in case several calls to GetChannelName are expected.
                                             //!< Note that the first call to GetChannelName takes longer since the channels must be sorted according to the channel names first.
};

///////////////////////////////////////////////////
//! Bit flags of an attachment
//!
//! enum values are equal to MDF4 bit positions for at_flags
enum AttachmentFlagsEnum
{
    eFlagEmbedded            =  1 << 0   //!< If set, the attachment data is embedded, otherwise it is contained in an external file
  , eFlagCompressed          =  1 << 1   //!< If set, the embedded data is compressed using the deflate algorithm
  , eFlagMD5CheckSumValid    =  1 << 2   //!< If set, the at_md5_checksum field contains the MD5 check sum of the data

};

///////////////////////////////////////////////////
//! Bit flags for removal of an attachment
//! used by IFile::DeleteAttachment
enum DeleteAttachmentFlagsEnum
{
    eFlagDeleteSecure        =  1 << 0   //!< If set, the removed attachment data is overwritten with zeros to make it unreadable
  , eFlagRemoveAtReferences  =  1 << 1   //!< If set, references to the attachment (in EV or CN block) will be removed, otherwise the respective reference link will only be set to zero
};


///////////////////////////////////////////////////
//! Bit flags of a time stamp
//!
//! enum values are equal to MDF4 bit positions for hd_time_flags
enum TimeFlagsEnum
{
    eFlagLocalTime           =  1 << 0   //!< If set, the start time stamp in nanoseconds represents the local time instead of the UTC time. In this case, time zone and DST offset must not be considered
  , eFlagOffsetsValid        =  1 << 1   //!< If set, the time zone and DST offsets are valid

};

///////////////////////////////////////////////////
//! Modes for OpenFile
enum OpenFileModeEnum
{
    eReadExclusive           =  0   //!< Read only access, locking the file
  , eReadShare               =  1   //!< Read only access, with sharing
  , eReadWriteExclusive      =  2   //!< Read and write access, locking the file.
                                    //!< <br>This allows to modify (extend) an existing MDF file.
                                    //!< Only usable with extended licensing of MDF4 Lib ("write option")

};

///////////////////////////////////////////////////
//!
//! Modes for IDataPointer::Read methods
enum DataReadModeEnum
{
    eGetValue                =  0   //!< Read operation will return the value from the current record
  , eAutoIncrement           =  1   //!< If Read operation for a channel is called a second time for the current record,
                                    //!< the next record will be read and the Read operation will return the value from the newly read record.
                                    //!< This allows subsequently reading all values for a channel (or a number of channels).

};


///////////////////////////////////////////////////
//!
//! Modes for IDataWriter::Write methods
enum DataWriteModeEnum
{
    eSetValue                =  0   //!< Write operation will set/change the value in the current record
  , eAutoFlush               =  1   //!< If SetTime, WriteRecord or Write operation for a channel is called a second time for the current record,
                                    //!< the current record will be written (flushed) to file, a new record will be prepared
                                    //!< and the Write operation will set the value in the new record.
                                    //!< This allows subsequently writing all values for a channel (or a number of channels).
};


///////////////////////////////////////////////////
//!
//! Modes for IDataWriter if and when to check for reverse time stamps
enum TimeCheckModeEnum
{
    eTimeCheckDisabled       =  0   //!< no check for reverse time stamps
  , eTimeCheckDuringFlush    =  1   //!< check during FlushRecord/WriteRecord (returns eReverseTimeStamp in case of reverse time stamps, but record is written nevertheless!)
  , eTimeCheckDuringSetTime  =  2   //!< check during SetTime methods (returns eReverseTimeStamp in case of reverse time stamps, but time value is written nevertheless!)
};


///////////////////////////////////////////////////
//!
//! Modes for IDataWriter if to check for flush errors (no dirty bits set)
enum FlushCheckModeEnum
{
    eFlushCheckDisabled      =  0   //!< no check for flush errors
  , eFlushCheckDuringFlush   =  1   //!< for write mode eAutoFlush, the automatic flush during Write/SetTime fails if not all channels are "dirty".
                                    //!< For calling IDataWriter::FlushRecord directly, the method returns eFlushNotAllValuesWritten as warning in case not all values have been written before, but reocrd is wirtten nevertheless!
  , eFlushCheckDuringWrite   =  2   //!< check during Write/SetTime methods and returns eNoFlushSinceLastValue in case a value has been written before and omits writing the value again.
                                    //!< The only exception is WriteRecord: this method may also report eNoFlushSinceLastValue, but the record will be written nevertheless, thus overwriting previously set values.
  , eFlushCheckAll           =  3   //!< combination of eFlushCheckDuringFlush and eFlushCheckDuringWrite
};



///////////////////////////////////////////////////
//! Modes for IDataPointer::SeekTimeNs and IDataPointer::SeekTimeSec
enum SeekTimeModeEnum
{
    eSeekNearest             =  0   //!< Seek time position that is nearest (can be less or greater)
  , eSeekEqual               =  1   //!< Seek time position that is equal
  , eSeekLess                =  2   //!< Seek time position that is less
  , eSeekLessOrEqual         =  3   //!< Seek time position that is less or equal
  , eSeekGreater             =  4   //!< Seek time position that is greater
  , eSeekGreaterOrEqual      =  5   //!< Seek time position that is greater or equal

};

///////////////////////////////////////////////////
//! Modes for IDataPointer::SeekPos
enum SeekPosModeEnum
{
    eSeekAbsolute            =  0   //!< Seek absolute position (offset = index)
  , eSeekRelative            =  1   //!< Seek relative to current position (current position + offset)

};


///////////////////////////////////////////////////
//!
//! Mode for access of values in IGroupDataPointer::Read methods
enum ValueAccessModeEnum
{
    eGetCurrent      =  0   //!< Get the current value, i.e. the value of the previously read record for this channel group.
  , eGetNext         =  1   //!< Get the next value
  , eGetInterpolated =  2   //!< Get a linear interpolated value for the current time (note: for discrete signals interpolation is not used and the result is equal as for to eGetCurrent).
};


///////////////////////////////////////////////////
//! Categories for state of licensing
enum LicenseLevelEnum
{
    eLicenseLevelUnspec      = -1   //!< Unspecified
  , eInvalidLicense          =  0   //!< Invalid license (expired or lib version not supported)
  , eDemoLicense             =  1   //!< Demo license
  , eTestLicense             =  2   //!< Test license (usually time restricted)
  , eValidLicense            =  3   //!< Valid read license
  , eWriteLicense            =  4   //!< Valid read/write license
};

///////////////////////////////////////////////////
//! Categories for log messages given to ILogSink
enum LogSinkLevelEnum
{
    eLogSinkLevelUnspec      = -1   //!< Unspecified
  , eInfo                    =  0   //!< Information
  , eWarning                 =  1   //!< Warning
  , eError                   =  2   //!< Error (not critical)
  , eFatalError              =  3   //!< Fatal Error (Exception)

};

///////////////////////////////////////////////////
//! Bit flags for options of MDF sorting
//! used by IFileManager::SortFile
enum SortFileOptionFlagsEnum
{
    eOverwriteDestinationFile  =  1 << 0   //!< If set, an existing destination file will be overwritten. If not set and if destination file exists, the operation will fail.
  , eFixMdf3AddRecords         =  1 << 1   //!< If set, an MDF3 file will be corrected during sorting if there are additional records (cycle counter too small / missing update e.g. due to a crash)
  , eFixMdf3GapRecords         =  1 << 2   //!< If set, gaps in an MDF3 file will be closed during sorting if there are missing records (cycle counter too high e.g. when saving records failed due to full disk)
  , eTestSortMode              =  1 << 3   //!< If set, the file will not be sorted immediately but only tested if it requires sorting
                                           //!< <br><b>Note</b>: currently for MDF4 files, arrays with CG template are not considered/supported yet!
  , eDebugLogFile              =  1 << 4   //!< If set, debug logging is activated. A log file will be generated for the sort operation: Default is to create a new log file with the same path and base name of destination file
                                           //!< with postfix "_MdfSort_log.txt" appended, e.g. "A.mdf" => "A_MdfSort_log.txt".
                                           //!< <br>If output file is not specified or output dir is not valid, then the path and name of the MdfSort.DLL will be used with postfix "_log.txt", e.g. "MdfSort_log.txt".
  , eDebugLogSink              =  1 << 5   //!< If set, all output messages for debug logging of MdfSort will be sent to the log sinks registered for IFileManager.
  , eCheckTime                 =  1 << 7   //!< If set, reverse time stamps detected during sorting are treated as error
};

///////////////////////////////////////////////////
//! Bit flags for options of MDF finalization
//! used by IFileManager::FinalizeFile
enum FinalizeFileOptionFlagsEnum
{
    eOverwriteDestFile         =  1 << 0   //!< If set, an existing destination file will be overwritten. If not set and if destination file exists, the operation will fail.
  , eAlwaysSortFile            =  1 << 1   //!< If set, an unsorted (and unfinalized) file will be sorted during finalization, independent if sorting is necessary for finalization or not.
  , eTestFinalizationMode      =  1 << 2   //!< If set, the file will not be finalized immediately but only tested if it is unfinalized and if it is possible to finalize it.
};

///////////////////////////////////////////////////
//! Bit flags for options when creating a new file
//! used by IFileManager::CreateFile
enum CreateFileOptionFlagsEnum
{
    eOverwriteExistingFile     =  1 << 0   //!< If set, an existing destination file will be overwritten. If not set and if destination file exists, the operation will fail.
  , eUseAsyncFileWrite         =  1 << 1   //!< If set, writing to file will be done asynchronously
};

///////////////////////////////////////////////////
//! Bit flags for options when validating a file
//! used by IFileManager::ValidateFile
enum ValidateFileOptionFlagsEnum
{
    eReportWarnings            =  1 << 0   //!< If set, warnings are reported, too. Otherwise only errors are reported.
  , eCheckForGaps              =  1 << 1   //!< If set, validation will check for gaps and overlapping of blocks.
  , eCheckXmlFragments         =  1 << 2   //!< If set, validation will check for valid XML fragments (MDF 4.x only).<br>
                                           //!< Attention: XML fragments can only be checked if the respective Schema files are present in subfolder Schema/v4xx.<br>
                                           //!< In case one of the Unicode variants of MDF4 Lib is used, the non-Unicode variant of XmlParser DLL is required in addition.
  , eCheckUnfinalized          =  1 << 3   //!< If set, validation will also be done for unfinalized MDF files.<br>
                                           //!< Otherwise unfinalized MDF files are rejected (error code ::eUnfinalizedMdf).
};

///////////////////////////////////////////////////
//! Bit flags for options when re-writing a file
//! used by IFileManager::RewriteFile
enum RewriteFileOptionFlagsEnum
{
    eOverwriteRewriteFile = 1 << 0   //!< If set, an existing destination file will be overwritten. If not set and if destination file exists, the operation will fail with ::eFileExists.
  , eReportNoChange       = 1 << 1   //!< If set, and if it is possible to determine that the file would not change at all, the operation returns ::eNoChange instead of ::eOK on success.
                                     //!< The destination file then is only written if ::eCopyFileIfNoChange is set, too. Otherwise the destination file is not written.
                                     //!< <br>If not set, the destination file will be written in any case and the method returns ::eOK on success.
                                     //!< Note: if the data blocks must be re-written, e.g. if bit flags ::eCompressSignalData or ::eDeCompressSignalData are set,
                                     //!< it is not possible to determine ::eNoChange ahead.
  , eCopyFileIfNoChange   = 1 << 2   //!< If set together with ::eReportNoChange, then the destination file will be written in any case, i.e. if the method returns ::eNoChange,
                                     //!< the source file will simply be copied to the destination file.
  , eCompressSignalData   = 1 << 3   //!< If set, for MDF 4.x files the signal data will be compressed during the rewrite operation, and (if MDF version &lt; 4.1) the MDF version will be increased to MDF 4.1.
                                     //!< <br>Note: this flag is mutual exclusive to ::eDeCompressSignalData
  , eDeCompressSignalData = 1 << 4   //!< If set, for MDF 4.x files the signal data will be decompressed during the rewrite operation.
                                     //!< <br>Note: this flag is mutual exclusive to ::eCompressSignalData
  , eOmitSampleReduction  = 1 << 5   //!< If set, sample reduction data (SR block sub trees) will be removed during re-write.
                                     //!< <br>Note: the sample reduction is calculated from the original signal data and typically is used for faster drawing of signals.
                                     //!< It can be stored in the MDF file to avoid re-calculation, which increases performance for drawing, but also increases the file size (approx. by a third).
                                     //!< Removing sample reduction data thus can be used to reduce the file size without loosing relevant information since it can be re-calculated again.

};

///////////////////////////////////////////////////
//! Bit flags for options when creating an IDataPointer object
//! used by IChannelGroup::CreateDataPointerEx and IChannel::CreateDataPointerEx, some options can be used for IFile::CreateGroupDataPointerEx
enum DataPointerOptionsFlagsEnum
{
    eUseReadBuffer           =  1 << 0     //!< If set, the IDataPointer will use a read buffer for reading the records (if possible).<br>
                                           //!< If not set, the records are read directly from file (less memory consumption, but possibly slower).<br>
                                           //!< See also IFile::SetLimit.
                                           //!< This is especially useful when using the MultRead methods or for simple forward reading all all records.
  , eUseAutoIncrement        =  1 << 1     //!< If set, the eAutoIncrement read mode will be activated immediately
  , eDisableAutoRestorePos   =  1 << 2     //!< If set, the automatic restoration of the record position after a MultiRead operation will be disabled
  , eDisableStatusTabWarning =  1 << 3     //!< If set, ::eStatusStringTableEntry will be ignored/not returned by ReadPhysValueDouble and all MultiReadPhysValueDouble methods and the numeric raw value will be used as phys value
  , eDisableVerbalTableError =  1 << 4     //!< If set, ::eDifferentCoversionResultType will be ignored/not returned by ReadPhysValueDouble and all MultiReadPhysValueDouble methods and the numeric raw value will be used as phys value
  , eShowVerbalTableErrorForMultiRead = 1 << 5 //!< If set, MultiReadPhysValueDouble methods will return error code ::eDifferentCoversionResultType if one of the channels has a verbal table that returns a string (unless ::eDisableVerbalTableError is specified)
                                           //!< <br>Note: the option was introduced for backward compatibility since this error has not been reported by MultiReadPhysValueDouble methods before.

 };

///////////////////////////////////////////////////
//! Bit flags for options when checking if channel values can be safely read
//! used by IDataPoiner::CanReadChannelValueAsPhysDouble
enum CanReadChannelValueFlagsEnum
{
  eCheckMultiReadUseCase     = 1 << 0      //!< If set, the check will be executed especially for MultiRead use case, otherwise only for ordinary Read use case.
};

///////////////////////////////////////////////////
//! Limit type used for IFile::SetLimit
enum LimitTypeEnum
{
    eLimitTypeUnspec               = -1   //!< Unspecified
  , eTotalSizeOfBuffer             =  0   //!< Maximum size to be used for all read and write buffers of this file.
                                          //!< The default size for this buffer type is 64 MByte.
};

///////////////////////////////////////////////////
//! Syntax version for algebraic formula
//! used for IFactory::CreateAlgebraicConversionEx
//! \note Most operators are equal in both syntax versions, but some have a different meaning or a different notation.
//! <br>For instance, ^ is used for "bitwise exclusive OR" in MDF4 whereas it previously was used as power operator in MDF3.
//! <br><br>For all differences, please refer to ASAM MCD 2MC 1.6.0 specification, section "Incompatibility of keyword FORMULA".
enum SyntaxVersionEnum
{
    eSyntaxVersionUnspec = -1   //!< Unspecified. In this case, the formula is used "as is",
                                //!< i.e. it is assumed that it is correct for the current MDF version.
  , eSyntaxVersionMDF3   =  0   //!< Syntax for MDF3 (ASAP2 V 1.5)
  , eSyntaxVersionMDF4   =  1   //!< Syntax for MDF4 (ASAP2 V 1.6 / ASAM GES 1.0)
};

///////////////////////////////////////////////////
//! Data type of a comment field
//! used in structure IMDF4LibStructs::FieldProperties
//! \note The types are a subset of the primitive data types available in XML Schema,
//! see http://www.w3schools.com/schema/default.asp
enum FieldDataTypeEnum
{
    eFieldDataTypeUnspec    = -1  //!< Unspecified
  , eFieldDataTypeString    = 0   //!< xs:string for strings that can contain characters, line feeds, carriage returns, and tab characters
  , eFieldDataTypeDecimal   = 1   //!< xs:decimal for a numeric value in general
  , eFieldDataTypeInteger   = 2   //!< xs:integer for a numeric value without a fractional component
  , eFieldDataTypeFloat     = 3   //!< xs:float for a numeric value without a fractional component
  , eFieldDataTypeBoolean   = 4   //!< xs:boolean for a Boolean value, i.e. to specify a true or false
  , eFieldDataTypeDate      = 5   //!< xs:date to specify a date in the form "YYYY-MM-DD" (time zone can be added)
  , eFieldDataTypeTime      = 6   //!< xs:time to specify a time in the form "hh:mm:ss" (seconds can contain fractional component, time zone can be added)
  , eFieldDataTypeDateTime  = 7   //!< xs:dateTime to specify a date and a time in the form "YYYY-MM-DDThh:mm:ss" (seconds can contain fractional component, time zone can be added)
};

///////////////////////////////////////////////////
//! Standard field type
//! used for IFile::GetStandardCommentFieldProperties
enum StandardFieldTypeEnum
{
    eStandardFieldTypeUnspec = -1  //!< Unspecified
  , eAuthor                  = 0   //!< author field (header field in MDF3, &lt;e&gt; tag with name "author" in MDF4)
  , eDepartment              = 1   //!< department field (header field in MDF3, &lt;e&gt; tag with name "department" in MDF4)
  , eProject                 = 2   //!< project field (header field in MDF3, &lt;e&gt; tag with name "project" in MDF4)
  , eSubject                 = 3   //!< subject field (header field in MDF3, &lt;e&gt; tag with name "subject" in MDF4)

};


///////////////////////////////////////////////////
//! Type of a ICommentField object
enum CommentFieldTypeEnum
{
    eCommentFieldTypeUnspec = -1  //!< Unspecified
  , eSingleValueField       = 0   //!< single-value field, corresponds to &lt;e&gt; tag in MDF 4.x or a field in MDF 3.x (Vector proprietary extension)
  , eMultiValueField        = 1   //!< multi-value field, corresponds to &lt;elist&gt; tag in MDF 4.1 or higher

};

///////////////////////////////////////////////////
//! Parent of the comment
//! mainly relevant for MDF 4.x in case of XML comments
enum CommentParentTypeEnum
{
    eCommentParentTypeUnspec   = -1 //!< Unspecified or not relevant/available (e.g. for plain text comments or MDF 3.x)
  , eCommentParentFile         =  0 //!< Parent is the IFile object (HD block)
  , eCommentParentDataGroup    =  1 //!< Parent is a data group (DG block) => only possible for MDF 4.x,
                                    //!< <br>access via IChannelGroup::GetDataGroupComment / IChannelGroup::SetDataGroupComment.
  , eCommentParentChannelGroup =  2 //!< Parent is an IChannelGroup object (CG block)
  , eCommentParentChannel      =  3 //!< Parent is an IChannel object (CN block)
  , eCommentParentConversion   =  4 //!< Parent is an IConversion object (CC block) => only possible for MDF 4.x
  , eCommentParentSourceInfo   =  5 //!< Parent is an ISourceInfo object (SI block) => only possible for MDF 4.x
  , eCommentParentEvent        =  6 //!< Parent is an IEvent object (EV/TR block)
  , eCommentParentAttachment   =  7 //!< Parent is an IAttachment object (AT block) => only possible for MDF 4.x
  , eCommentParentCreatorInfo  =  8 //!< Parent is an ICreatorInfo object (FH block)  => only possible for MDF 4.x

};


///////////////////////////////////////////////////
//! Control of merging of comment information
//! used by IFile::SetCommentTextAndFields and IFactory::CreateCommentEx
enum CommentMergeFlagsEnum
{
    eMergeCommentFields             = 1 << 0  //!< If set, and if list of comment fields is given (pCommentFields != NULL), replace existing fields and append new fields
                                              //!< <br>If not set, and if list of comment fields is given, existing fields are removed and the new fields are written
                                              //!< <br>If comment fields are not given (pCommentFields == NULL), existing fields are always preserved
  , eRemoveCommentFields            = 1 << 1  //!< If set, and if list of comment fields is given, all fields specified in the list will be removed from the existing fields;
                                              //!< all other fields are preserved. This flag cannot be set together with ::eMergeCommentFields.
  , eAppendCommentText              = 1 << 2  //!< If set, and if plain comment text is given (plainText != NULL), the text will be appended to an existing text.
                                              //!< <br>If not set, and if plaingText is not NULL, the original text will be replaced.
                                              //!< See also ::eAppendTextWithLineBreak
  , eAppendTextWithLineBreak        = 1 << 3  //!< If set and if ::eAppendCommentText is set and if the orginal text is not empty, then a line break (\n) will be inserted before appending the plainText
  , ePreserveAdditionalInformation  = 1 << 4  //!< If set, additional information in the original comment will be preserved
                                              //!< (i.e. $-encoded information in MDF 3.x that is not mapped to comment fields or additional XML information in MDF 4.x).
                                              //!< Otherwise, the additional information will be removed.
  , eUpdateCreatorInformation       = 1 << 5  //!< If set, creator information (ci attributes) are updated for all element in input XML string.
                                              //!< <br>This flag should be set in case the input XML string originates from a different file.
                                              //!< <br>Only relevant for IFactory::CreateCommentEx if isXml is true.

  , eKeepExstingInformation = eMergeCommentFields | ePreserveAdditionalInformation
  , eKeepExstingInformationAppendText = eMergeCommentFields | eAppendCommentText | eAppendTextWithLineBreak | ePreserveAdditionalInformation
};

///////////////////////////////////////////////////
//! Time Source Class / Quality
//!
//! enum values are equal to MDF enum values for hd_time_class (MDF 3.2 or higher)
enum TimeSrcClassEnum
{
    eTimeSrcClassUnspec   = -1    //!< Unspecified
  , eTimeSrcClassLocal    =  0    //!< local PC reference time (Default)
  , eTimeSrcClassExternal = 10    //!< external time source
  , eTimeSrcClassAbsSync  = 16    //!< external absolute synchronized time

};

///////////////////////////////////////////////////
//! Bit flags to control how to retrieve first and/or last time stamp from a file
//! used by IFile::GetFirstAndLastTimeStampNsEx
enum GetFirstAndLastTimeStampOptionsFlagsEnum
{
    eIgnoreUnsortedGroups         = 1 << 0  //!< If set, unsorted groups are ignored (unless min/max info in meta data can be used).
                                            //!< <br>Note that determining min/max values for unsorted groups requires to read all records which may take a long time.
  , eIgnoreVirtualTimeChannel     = 1 << 1  //!< If set, channel groups with virtual time master channel (using the record index as raw time value) are ignored.
  , eUseIndexIfNoTimeChannel      = 1 << 2  //!< If set, channel groups with no time master channel are not ignored but considered as virtual time master channel with 1:1 conversion, i.e. record index = time in seconds.
  , eIgnoreChannelMinMaxInfo      = 1 << 3  //!< If set, the min/max raw values stored in meta data of time master channel are not considered.
                                            //!< <br>Use this option if you do not trust the stored min/max values, or if you want to avoid rounding errors due to storage of raw value as double value.
  , eIgnoreConversionMinMaxInfo   = 1 << 4  //!< If set, the min/max phys values stored in meta data of conversion rule of time master channel are not considered.
                                            //!< <br>Use this option if you do not trust the stored min/max values (e.g. due to sharing of conversion rule), or if you want to avoid rounding errors due to storage of seconds as double value.
  , ePreferChannelMinMaxInfo      = 1 << 5  //!< If set, the min/max raw values stored in master channel are preferred to using the min/max phys values stored in its conversion rule (if available).
                                            //!< <br>Use this option if you expect the raw values to be more accurate than the phys values, or if you expect that there are shared conversion rules.
                                            //!< <br>Note: this option is not relevant if one of the bit flags ::eIgnoreChannelMinMaxInfo or ::eIgnoreConversionMinMaxInfo is set.
};
