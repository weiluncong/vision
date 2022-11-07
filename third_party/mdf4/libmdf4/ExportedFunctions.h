/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces:
-------------------------------------------------------------------------------
MDF4Lib public unmanaged C++ interface: forward declarations for exported functions
See API documentation for details
-------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
-----------------------------------------------------------------------------*/
#pragma once

///////////////////////////////////////////////////
//! \file
//! \brief Exported functions

///////////////////////////////////////////////////
//! Get the FileManager Interface from MDF4Lib and register the license for the library (see \ref licensing)
//! \see IFileManager
//! \param[in]  customerName string with customer name as given with registration information when purchasing the library
//! \param[in]  serialNumber string with serial number as given with registration information when purchasing the library
//! \param[in]  registrationKey string with registration key as given with registration information when purchasing the library
//! \note a IFileManager object will be delivered in any case, even for invalid license, however then later some functions will fail
//! \return pointer to IFileManager object
extern "C" IFileManager* GetFileManager(LPCTSTR customerName, LPCTSTR serialNumber, LPCTSTR registrationKey);

///////////////////////////////////////////////////
//! function pointer to get the IFileManager object
//! \see ::GetFileManager
typedef IFileManager* (*GetFileManagerPtr)(LPCTSTR customerName, LPCTSTR serialNumber, LPCTSTR registrationKey);

///////////////////////////////////////////////////
//! Check for compatibility with MDF4Lib interface
//! \note The interface (API) of MDF4Lib has a 3 part version number which will be increased in case
//! the interface is changed.
//! \param[in]  majorVersion major interface version number as defined in MDF4LibAPI.h (MDF4LIB_INTERFACE_MAJOR_VERSION_NO)
//! \param[in]  minorVersion major interface version number as defined in MDF4LibAPI.h (MDF4LIB_INTERFACE_MINOR_VERSION_NO)
//! \param[in]  revision interface revision number as defined in MDF4LibAPI.h (MDF4LIB_INTERFACE_REVISON__NO)
//! \param[in]  isUnicode true, if Unicode strings should be used, false if multi-byte characters strings (MBCS) should be used
//! \return     true if interface is compatible, otherwise false
extern "C" bool CheckInterfaceCompatibility(UINT8 majorVersion, UINT8 minorVersion, UINT8 revision, bool isUnicode);

///////////////////////////////////////////////////
//! function pointer to check compatibility with MDF4Lib interface
//! \see ::CheckInterfaceCompatibility
typedef bool (*CheckInterfaceCompatibilityPtr)(UINT8 majorVersion, UINT8 minorVersion, UINT8 revision, bool isUnicode);

///////////////////////////////////////////////////
//! Set a callback routine for ASSERTs
//! \see IAssertCallback
//! \param[in]  pAssertCallback the callback interface to set (there only can be one!)
extern "C" void SetAssertCallback(IAssertCallback* pAssertCallback);

///////////////////////////////////////////////////
//! function pointer to set the assert callback
//! \see ::SetAssertCallback
typedef void (*SetAssertCallbackPtr)(IAssertCallback* pAssertCallback);

#ifndef LINUX
///////////////////////////////////////////////////
//! Set COM initialization parameters (for XML parser)
//! Call this method in case the COM apartment model does not match the default model used by Mdf4Lib.
//! \note The method must be called before the frist call to ::GetFileManager!
//!
//! COM only is used for the XML parser component.
//! Without XML parser reading the Mdf4Lib config file and reading XML properties from MF4 files is not possible.
//! \param init true if COM must be initialized by MDF4Lib, false if this has already been done by callee.
//!        <br>Default: true
//! \param multithreaded true if ::CoInitializeEx() will be called with COINIT_MULTITHREADED, otherwise with COINIT_APARTMENTTHREADED
//!        <br>only relevant if init is true
//!        <br>see Microsoft documentation on ::CoInitializeEx().
//!        <br>Default: true (= COINIT_MULTITHREADED)
//! \return true if setting was OK, false if setting was called too late or with different parameters than before
extern "C" bool SetCoInitializeParameters(bool init, bool multithreaded);

///////////////////////////////////////////////////
//! function pointer to set COM initialization parameters
//! \see ::SetCoInitializeParameters
typedef bool (*SetCoInitializeParametersPtr)(bool init, bool multithreaded);
#endif  // LINUX

///////////////////////////////////////////////////
//! Register a callback interface to be called upon unload of Mdf4Lib DLL
//! \see IUnloadDllCallback
//! \param pUnloadDllCallback pointer to IUnloadDllCallback interface to be registered.
//! \return 0 on success
extern "C" int RegisterUnloadDllCallback(IUnloadDllCallback* pUnloadDllCallback);

///////////////////////////////////////////////////
//! function pointer to register a callback interface to be called upon unload of Mdf4Lib DLL
//! \see ::RegisterUnloadDllCallback
typedef int(*RegisterUnloadDllCallbackPtr)(IUnloadDllCallback* pUnloadDllCallback);

///////////////////////////////////////////////////
//! Unregister a callback interface to be called upon unload of Mdf4Lib DLL
//! \see IUnloadDllCallback
//! \param pUnloadDllCallback pointer to IUnloadDllCallback interface to be unregistered.
//! \return 0 on success
extern "C" int UnregisterUnloadDllCallback(IUnloadDllCallback* pUnloadDllCallback);

///////////////////////////////////////////////////
//! function pointer to unregister a callback interface to be called upon unload of Mdf4Lib DLL
//! \see ::UnregisterUnloadDllCallback
typedef int(*UnregisterUnloadDllCallbackPtr)(IUnloadDllCallback* pUnloadDllCallback);

#ifndef LINUX
//////////////////////////////////////////////////
//! Rename Dll relevant for MDF4 Lib (e.g. MdfSort.dll).
//! This can be helpful when using different versions of MDF4 Lib in parallel.
//! Each time Mdf4 Lib tries to load the original DLL, the new name will be used instead
//! Note: this must be called before calling ::GetFileManager and ::SetCoInitializeParameters. Afterwards the rename will fail.
//! \param originalName original name of DLL
//! \param newName new name of DLL
//! \return true if renaming is activated, false otherwise (e.g. wrong input parameters or wrong ordering of calls)
extern "C" bool RenameDll(LPCTSTR originalName, LPCTSTR newName);

///////////////////////////////////////////////////
//! function pointer to rename Dll relevant for MDF4 Lib
//! \see ::RenameDll
typedef bool(*RenameDllPtr)(LPCTSTR originalName, LPCTSTR newName);
#endif  // LINUX