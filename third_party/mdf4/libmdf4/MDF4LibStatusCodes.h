/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces:
-------------------------------------------------------------------------------
MDF4Lib public unmanaged C++ interface: status code enumerations
See API documentation for details
-------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
-----------------------------------------------------------------------------*/
#pragma once

// RULE: never change existing enums. Extend only!

///////////////////////////////////////////////////
//! \file
//! \brief Enumerations for status/error codes

///////////////////////////////////////////////////
//! Error/result codes for MDF4Lib operations
//!
enum ErrorCodeEnum
{
  // general
    eErrorCodeUnspec                 = -1  //!< unspecified error code
  , eOK                              =  0  //!< OK
  , eGeneralError                    =  1  //!< general error
  , eNotImplemented                  =  2  //!< functionality is not implemented / not supported (yet)
  , eNotAvailable                    =  3  //!< functionality / element is available for this format / this file / this object
  , eOutOfMemoryError                =  4  //!< out of memory (new failed)
  , eInvalidArgument                 =  5  //!< invalid argument
  , eIndexOutOfBound                 =  6  //!< index is out of boundary
  , eNotCalledFromSameThread         =  7  //!< method not called from same thread
  , eOverflowError                   =  8  //!< data type overflow error
  , eOperationCancel                 =  9  //!< operation was canceled by user

  // file errors / warnings
  , eFileNotFound                    = 10  //!< file or file path not found
  , eFileOpenError                   = 11  //!< file could not be opened
  , eFileReadError                   = 12  //!< unable to read from file
  , eFileCreateError                 = 13  //!< file could not be created
  , eFileWriteError                  = 14  //!< unable to write to file
  , eFileAlreadyOpen                 = 15  //!< file already is open (warning)
  , eFileExists                      = 16  //!< file already exists, must not overwrite!
  , eFileAlreadySorted               = 17  //!< file already was sorted
  , eFileSortError                   = 18  //!< file could not be sorted
  , eFileEmpty                       = 19  //!< file is empty

  // format errors / warnings
  , eFormatNotSupported              = 20  //!< format in file not supported (no MDF, or future MDF version)
  , eFatalFormatError                = 21  //!< file has invalid format
  , eParsingError                    = 22  //!< some part of the format or the formula could not be parsed (format or IO error)
  , eNoXmlParser                     = 23  //!< XML parser not available
  , eNoConverter                     = 24  //!< no converter DLL available
  , eReverseTimeStamp                = 25  //!< reverse time stamp detected during Flush or SetTime (warning only)

  // Read / write value errors / warnings
  , eNoDataAvailable                 = 30  //!< no data available
  , eReachedEndOfData                = 31  //!< reached end of data
  , eInvalidDataTypeForOperation     = 32  //!< data type cannot be used for this operation
  , eResultBufferTooSmall            = 33  //!< result buffer too small
  , eInvalidChannelForOperation      = 34  //!< given channel does not apply to data pointer
  , eNotInitialized                  = 35  //!< not initialized
  , eBeforeStartOfData               = 36  //!< no data available because before first data point (value of first data point is returned)
  , eContainsVLSD                    = 37  //!< channel group contains a VLSD channel
  , eFlushNotAllValuesWritten        = 38  //!< not all channels have been written yet (detected during FlushRecord)
  , eNoFlushSinceLastValue           = 39  //!< channel value already has been written before (Write/SetTime)

  // conversion errors / warnings
  , eDifferentCoversionResultType    = 40  //!< result type does not match
  , eInvalidConversionResult         = 41  //!< conversion result is not valid
  , eEvaluationError                 = 42  //!< conversion could not be evaluated
  , eDivisionByZero                  = 43  //!< division by zero
  , eLogarithmOfZero                 = 44  //!< logarithm of zero
  , eStatusStringTableEntry          = 45  //!< status string table entry found for raw value (numerical value returned as phys value might not make sense)
                                           //!< note: numerical value returned is raw value converted by main conversion
  , eNoInverseConversion             = 46  //!< no inverse conversion available

  // other errors
  , eLogSinkNotRegistered            = 50  //!< log sink was not registered
  , eNoLogSinkRegisteredForLevel     = 51  //!< no Log sink registered for this logging level
  , eLibraryNotLicensed              = 52  //!< library is not licensed, must call GetFileManager with valid license info
  , eRequiredDllMissing              = 53  //!< a required Mdf4Lib dll is not found in the installed folder. Please copy all corresponding dlls in same folder
  , eInterfaceNotCompatible          = 54  //!< interface compatibility check has failed. The installed libraries are not compatible with each other
  , eAlreadyClosed                   = 55  //!< object is already closed
  , eWrongMode                       = 56  //!< wrong mode for this operation
  , eNoChange                        = 57  //!< warning indicating that no change was done/necessary (e.g. during ReWriteFile)

  // warnings during sorting
  , eFileSortCycleCountIncreased     = 60  //!< cycle counters increased during sorting (can only occur for MDF3 and eFixMdf3AddRecords)
  , eFileSortCycleCountDecreased     = 61  //!< cycle counters decreased during sorting (can only occur for MDF3, possibly indicates corrupt file)

  // object creation
  , eDataPointerExists               = 66  //!< a data pointer object (IDataPointer or IGroupDataPointer) uses this channel group. Operation only possible after release of the data pointer object.
  , eDataWriterExists                = 67  //!< a data writer object (IDataWriter) uses this channel group. Operation only possible after release of the data writer object.
  , eFileIsReadOnly                  = 68  //!< file is read-only and cannot be modified (not newly created nor opened with eReadWriteExclusive)
  , eObjectNotFromSameFile           = 69  //!< a given interface pointer object is not contained in this file
  , eParentByteRangeExceeded         = 70  //!< channel cannot be added because the Byte range of the parent element is fixed (cannot be exteded) and is smaller than the required Byte range of channel to be added
  , eNoUniqueIdentification          = 71  //!< channel or channel group with same identification already contained
  , eNoUniqueMasterChannel           = 73  //!< master channel with same synchronization type already contained in channel group, cannot add a second one
  , eNoMatchingMasterChannel         = 72  //!< no master channel with required synchronization type found
  , eNoStructureParent               = 74  //!< unable to add child channels because parent channel cannot be a structure (array or no fixed length data type eByteArray)
  , eInvalidDataTypeSize             = 75  //!< size for data type not allowed in MDF format or not supported by MDF4 Lib
  , eInvalidConversionType           = 76  //!< conversion type not allowed here
  , eInputBufferTooSmall             = 77  //!< input buffer too small, rest filled with zeros
  , eInputBufferTooLarge             = 78  //!< input buffer too large, data truncated
  , eInvalidContextForOperation      = 79  //!< context does not allow this operation, see log file for details
  , eConversionNotMonotone           = 80  //!< conversion may not be monotone
  , eStringTruncated                 = 81  //!< input string was too long and has been truncated (warning)
  , eInvalidChannelType              = 82  //!< invalid channel type for operation
  , eObjectStillExists               = 83  //!< object instance still exists and cannot be removed
  , eNotFound                        = 84  //!< not found

  // finalization
  , eUnfinalizedMdf                  = 85  //!< unfinalized MDF file, must be finalized first (e.g. using IFileManager::FinalizeFile)
  , eFileAlreadyFinalized            = 86  //!< file already is finalized
  , eUnknownFinalizationStep         = 87  //!< file is unfinalized, but cannot be finalized due to some unknown finalization step (custom step of unknown application or new standard step)
  , eFinalizationStepNotImplemented  = 88  //!< file is unfinalized, but cannot be finalized because a required finalization step is not implemented / not supported (yet)

  // events
  , eInvalidEventType                = 100  //!< invalid event type for operation
  , eInvalidEventRangeType           = 101  //!< invalid event range type for operation
  , eInvalidEventScope               = 102  //!< invalid event scope for operation
  , eInvalidSyncType                 = 103  //!< invalid sync type
  , eInvalidEventProperties          = 104  //!< invalid event properties for operation
  , eInvalidEventHierarchy           = 105  //!< invalid event hierarchy for operation
};

