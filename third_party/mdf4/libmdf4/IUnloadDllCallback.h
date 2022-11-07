/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IUnloadDllCallback
-------------------------------------------------------------------------------
MDF4Lib public unmanaged C++ interface
See API documentation for details

This special interface is implemented by the client and registered in via
the MDF4Lib exported function ::RegisterUnloadDllCallback and unregistered via
::UnregisterUnloadDllCallback.

If the interface is registered, its method will be called before the IFileManager
singleton object will be destroyed.

See API documentation for details.
-------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
-----------------------------------------------------------------------------*/
#pragma once

#ifndef LINUX
#include <tchar.h>

#ifndef MDFLIBCALL
#define MDFLIBCALL // __cdecl
#endif
#endif


///////////////////////////////////////////////////
//! \file
//! \brief Definition of IUnloadDllCallback

///////////////////////////////////////////////////
//! \brief Callback interface to be called before IFileManager singleton object will be destroyed
//!
//! This may be used in scenarios where a process unloads MDF4 Lib without calling the destructor of
//! the VMdf4Lib helper class or other custom objects holding pointers to MDF4Lib objects.
//! Implementing a class that derives from this interface and registration of this
//! class using the exported function "RegisterUnloadDllCallback".
//!\code
//!#include "MDF4LibAPI.h"
//!
//!HANDLE hMdf4Lib = ::LoadLibrary(MDF4LIB_DLL_NAME); // possibly path to DLL must be specified, too!
//!
//!RegisterUnloadDllCallbackPtr RegisterUnloadDllCallback = (hMdf4Lib)
//!          ? (RegisterUnloadDllCallbackPtr)::GetProcAddress(hMdf4Lib, "RegisterUnloadDllCallback")
//!          : NULL;
//!
//!// assume a global instance gUnloadDllCallback of a class that derives from IUnloadDllCallback
//!if (RegisterUnloadDllCallback)
//!  RegisterUnloadDllCallback(&gUnloadDllCallback);
//!
//!\endcode
//!
//! You also can unregister a registered callback using the exported method UnregisterUnloadDllCallback.
//! For a sample implementation, see MdfUse.h
interface IUnloadDllCallback
{
public:
  ///////////////////////////////////////////////////
  //! will be called before destruction of IFileManager
  //!
  //! \param[in] refCountIsZero true, if reference counter of IFileManager is zero,
  //!            otherwise the calls to IFileMananger::Release() are unbalanced
  virtual void MDFLIBCALL OnDestroyFileManager(bool refCountIsZero) = 0;
};



