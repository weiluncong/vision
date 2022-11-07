/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces:
-------------------------------------------------------------------------------
MDF4Lib public unmanaged C++ interface: forward declarations
-------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
-----------------------------------------------------------------------------*/

///////////////////////////////////////////////////
//! \file
//! \brief Forward declarations

#pragma once

  interface IFileManager;
  interface IFile;
  interface IFileSizeObserver;
  interface IFactory;

  interface IChannelGroup;
  interface IChannelSet;
  interface IChannel;
  interface IArrayInfo;

  interface IEventSet;
  interface IEvent;

  interface IDataPointer;
  interface IDataWriter;
  interface IGroupDataPointer;

  interface ICreatorInfo;
  interface IAttachment;

  interface ISourceInfo;
  interface IConversion;

  interface IFormatInfo;
  interface ITimeStamp;

  interface IComment;
  interface IUnit;

  interface ICommentNodeSet;
  interface ICommentNode;
  interface ICommentFieldSet;
  interface ICommentField;

  // other
  interface IAssertCallback;
  interface IProgressCallback;
  interface ILogSink;
  interface IValidateFileCallback;
  interface IUnloadDllCallback;
