/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: IAssertCallback
-------------------------------------------------------------------------------
MDF4Lib public unmanaged C++ interface
See API documentation for details

This special interface is implemented by the client and registered in via
the MDF4Lib exported method ::SetAssertCallback. If the interface is registered,
each assertion is delegated to the client.
-------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
-----------------------------------------------------------------------------*/
#pragma once

#ifndef LINUX
#include <tchar.h>

#ifndef MDFLIBCALL
#define MDFLIBCALL __cdecl
#endif
#else // LINUX
#ifndef _TCHAR
  #define _TCHAR char
#endif
#endif

///////////////////////////////////////////////////
//! \file
//! \brief Definition of IAssertCallback

///////////////////////////////////////////////////
//! \brief Callback interface to report assertions
//!
//! For debugging, it may be useful to log the assertions to a file. This can be done
//! by implementing a class that derives from this interface and registration of this
//! class using the exported function "SetAsssertCallback":
#ifndef LINUX
//!\code 
//!#include "MDF4LibAPI.h"
//!
//!HANDLE hMdf4Lib = ::LoadLibrary(MDF4LIB_DLL_NAME); // possibly path to DLL must be specified, too!
//!
//!SetAssertCallbackPtr SetAssertCallback = hMdf4Lib ? (SetAssertCallbackPtr)::GetProcAddress(hMdf4Lib, "SetAssertCallback") : NULL;
//!
//!// assume a global instance gAssertCallback of a class that derives from IAssertCallback
//!if (SetAssertCallback)
//!  SetAssertCallback(&gAssertCallback);
//!
//!\endcode
#else   // LINUX
//!\code 
//!#include <dlfcn.h>    // For dlopen, dlsym, dlclose
//!#include <libmdf4/MDF4LibAPI.h>
//!
//!void* hMdf4Lib = dlopen(MDF4LIB_DLL_NAME, RTLD_LAZY); // possibly path to shared library must be specified, too!
//!
//!SetAssertCallbackPtr SetAssertCallback = hMdf4Lib ? (SetAssertCallbackPtr)dlsym(hMdf4Lib, "SetAssertCallback") : nullptr;
//!
//!// assume a global instance gAssertCallback of a class that derives from IAssertCallback
//!if (SetAssertCallback)
//!  SetAssertCallback(&gAssertCallback);
//!
//!\endcode
#endif  // LINUX

interface IAssertCallback
{
public:
  ///////////////////////////////////////////////////
  //! will be called when an assertion occurs
  //!
  //! \param[in] exp string with expression that caused the assertion, e.g. for display
  //! \param[in] file name of the source file where the assertion occurred
  //! \param[in] line line number where the assertion occurred
  virtual void MDFLIBCALL Assert(const _TCHAR* exp, const _TCHAR* file, unsigned int line) = 0;
};



