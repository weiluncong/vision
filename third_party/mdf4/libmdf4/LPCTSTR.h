/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces: 
-------------------------------------------------------------------------------
MDF4Lib public unmanaged C++ interface
-------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
-----------------------------------------------------------------------------*/

///////////////////////////////////////////////////
//! \file
//! \brief Character pointer definitions


#ifndef __LPCTSTR_H__
#define __LPCTSTR_H__

#if !defined(_INC_TCHAR)&&!defined(LINUX)
#  include <tchar.h>
#endif

#ifndef _WINNT_

#ifndef TCHAR
#define TCHAR _TCHAR
#endif

#ifndef LPCTSTR
#define LPCTSTR const TCHAR*
#endif

#ifndef LPCSTR
#define LPCSTR const CHAR*
#endif

#ifndef LPTSTR
#define LPTSTR TCHAR*
#endif

#endif

#endif
