/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: all
-------------------------------------------------------------------------------
MDF4Lib public unmanaged C++ interface: general data types
-------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
-----------------------------------------------------------------------------*/
#pragma once

//! \file
//! \brief General data types for MDF4Lib

#ifdef LINUX
  #include <cstdint>

  #ifndef TCHAR
    #define TCHAR char
  #endif

  #ifndef _T
    #define _T(x) x
  #endif

  #include "Backward.h"

  #ifndef MDFLIBCALL
  #define MDFLIBCALL
  #endif
#else   // !LINUX
  #include <tchar.h>
  #include <BaseTsd.h>
  #ifndef MDFLIBCALL
  #define MDFLIBCALL __cdecl
  #endif
#endif  //LINUX

#if !defined(CINTERFACE) && !defined(_OBJBASE_H_)
#define interface struct
#define CINTERFACE
#endif


#ifndef REAL_DEFINED
typedef double REAL;
#define REAL_DEFINED
#endif

#ifndef BYTE_DEFINED
typedef unsigned char BYTE;
#define BYTE_DEFINED
#endif

#include "LPCTSTR.h"

