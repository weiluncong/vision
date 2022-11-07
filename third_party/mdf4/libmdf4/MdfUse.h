#ifndef _MDF_USE_H_
#define _MDF_USE_H_

#include <cstdio>
#include <cstdlib>

#define LINUX

#ifndef LINUX
#include <Windows.h>
#else // LINUX
#include <cstdint>
#ifndef ASSERT
#include <cassert>
#define ASSERT(x) assert(x)
#endif // ASSERT
#endif // !LINUX

#include "MDF4LibAPI.h"
#include "ReferencePointer.h"

#ifdef RENAME_MDF4LIB_DLLS
#include <map>
#include <string>
typedef std::basic_string<TCHAR> tstring;
typedef std::map<tstring, tstring> VRenameDllMap;
#endif

////////////////////////////////////////////////////////////////////////
// type definitions
typedef VReferencePointer<MDF4Lib::IFileManager> IFileManagerPtr;
typedef VReferencePointer<MDF4Lib::IFile> IFilePtr;
typedef VReferencePointer<MDF4Lib::IFileSizeObserver> IFileSizeObserverPtr;
typedef VReferencePointer<MDF4Lib::IFactory> IFactoryPtr;

typedef VReferencePointer<MDF4Lib::IChannelGroup> IChannelGroupPtr;
typedef VReferencePointer<MDF4Lib::IChannelSet> IChannelSetPtr;
typedef VReferencePointer<MDF4Lib::IChannel> IChannelPtr;
typedef VReferencePointer<MDF4Lib::IArrayInfo> IArrayInfoPtr;

typedef VReferencePointer<MDF4Lib::IEventSet> IEventSetPtr;
typedef VReferencePointer<MDF4Lib::IEvent> IEventPtr;

typedef VReferencePointer<MDF4Lib::IDataPointer> IDataPointerPtr;
typedef VReferencePointer<MDF4Lib::IDataWriter> IDataWriterPtr;
typedef VReferencePointer<MDF4Lib::IGroupDataPointer> IGroupDataPointerPtr;

typedef VReferencePointer<MDF4Lib::ICreatorInfo> ICreatorInfoPtr;
typedef VReferencePointer<MDF4Lib::IAttachment> IAttachmentPtr;

typedef VReferencePointer<MDF4Lib::ISourceInfo> ISourceInfoPtr;
typedef VReferencePointer<MDF4Lib::IConversion> IConversionPtr;

typedef VReferencePointer<MDF4Lib::IFormatInfo> IFormatInfoPtr;
typedef VReferencePointer<MDF4Lib::ITimeStamp> ITimeStampPtr;

typedef VReferencePointer<MDF4Lib::ICommentFieldSet> ICommentFieldSetPtr;
typedef VReferencePointer<MDF4Lib::ICommentField> ICommentFieldPtr;

typedef VReferencePointer<MDF4Lib::IComment> ICommentPtr;
typedef VReferencePointer<MDF4Lib::IUnit> IUnitPtr;

#ifndef LINUX
////////////////////////////////////////////////////////////////////////
// helper class that opens MDF4Lib in constructor and closes it in destructor
class VMdf4Lib : protected MDF4Lib::IUnloadDllCallback
{
public:
  ///////////////////////////////////////////////////
  //! constructor for MDF4 Lib wrapper in case we want delayed loading (call to Init required!)
  VMdf4Lib()
      : mpFileManager(NULL), mhLibrary(NULL)
  {
  }

  ///////////////////////////////////////////////////
  //! constructor for MDF4 Lib wrapper
  //! parameters see Init()
  //! Note: calls to this class are not threadsafe except GetFileManager() and IsInitialized()
  VMdf4Lib(LPCTSTR customerName, LPCTSTR serialNo, LPCTSTR regKey, LPCTSTR mdf4LibDllPath = NULL, MDF4Lib::IAssertCallback *pAssertCallback = NULL,
           bool initCOM = true, bool multithreadedCOM = true)
      : mpFileManager(NULL), mhLibrary(NULL)
  {
    Init(customerName, serialNo, regKey, mdf4LibDllPath, pAssertCallback, initCOM, multithreadedCOM);
  }

  ///////////////////////////////////////////////////
  virtual ~VMdf4Lib()
  {
    if (mpFileManager)
    {
      mpFileManager->Release();
    }

    if (mhLibrary)
    {
      UnregisterUnloadDllCallback(this);
      ::FreeLibrary(mhLibrary);
    }
  }

  LPCTSTR GetFullMdf4LibPath(HMODULE hModule = 0)
  {
    TCHAR path[_MAX_PATH + 1] = _T("");
    ::GetModuleFileName(hModule, path, _MAX_PATH);

    TCHAR drive[_MAX_DRIVE] = _T("");
    TCHAR dir[_MAX_DIR] = _T("");
    //#pragma warning(suppress: 6102)
    ::_tsplitpath_s(path, drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, 0, NULL, 0);
    ::_tmakepath_s(mFullMdf4LibPathName, _MAX_PATH, drive, dir, _T(MDF4LIB_BASE_NAME), _T(".dll"));

    return mFullMdf4LibPathName;
  }

  ///////////////////////////////////////////////////
  //! get the IFileManager
  //! \return smart pointer for IFileManager interface
  //! <br><br>Please assign to a smart pointer object (IFileManagerPtr) to have a balanced reference counting.
  IFileManagerPtr GetFileManager() { return mpFileManager ? mpFileManager->GetSelf() : NULL; }

  ///////////////////////////////////////////////////
  bool IsInitialized() { return mhLibrary != NULL; }

  ///////////////////////////////////////////////////
  //! Init for later initialization in case default constructor is used. Only call once!
  //! \param[in]  customerName string with customer name as given with registration information when purchasing the library.
  //! \param[in]  serialNumber string with serial number as given with registration information when purchasing the library.
  //! \param[in]  registrationKey string with registration key as given with registration information when purchasing the library.
  //! \param[in]  mdf4LibDllPath path and name of MDF4 Lib DLL. If not specified, the default DLL name is used in the path of the calling module (EXE).
  //!             <br>Default: NULL (i.e. default DLL path is used)
  //!             <br>Attention: breaking change for V1.7: if mdf4LibDllPath is NULL, then the full path of the calling module (EXE) is used now.
  //!             <br>Previous behavior where Mdf4Lib DLL was searched in different locations or an already loaded instance from some other folder was used,
  //!             <br>please call Init() explicitely with MDF4LIB_DLL_NAME as parameter value (without path).
  //!             <br>However, for security, we recommend to always use an absolute path.
  //!             <br>Note: use VMdf4Lib.GetFullMdf4LibPath(hModule) with the handle of your DLL if Mdf4Lib DLL is located in the same folder as your DLL.
  //!             <br>Example:
  //!
  //!             <br>VMdf4Lib mdf4lib;
  //!             <br>mdf4lib.Init(customerName, serialNo, regKey, mdf4Lib.GetFullMdf4LibPath(hModule));
  //!
  //! \param[in]  pAssertCallback pointer to IAssertCallback that will from now on receive asserts. NULL to unregister callback.
  //!             <br>See also ::SetAssertCallback.
  //!             <br>Default: NULL
  //! \param[in]  initCOM true if COM must be initialized by MDF4Lib, false if this has already been done by callee.
  //!             <br>See also ::SetCoInitializeParameters.
  //!             <br>Default: true
  //! \param[in]  multithreadedCOM true if ::CoInitializeEx will be called with COINIT_MULTITHREADED, otherwise with COINIT_APARTMENTTHREADED.
  //!             <br>only relevant if initCOM is true.
  //!             <br>See also ::SetCoInitializeParameters and Microsoft documentation on ::CoInitializeEx.
  //!             <br>Default: true (= COINIT_MULTITHREADED)
  //! \return     true on success, false otherwise
  bool Init(LPCTSTR customerName, LPCTSTR serialNo, LPCTSTR regKey, LPCTSTR mdf4LibDllPath = NULL, MDF4Lib::IAssertCallback *pAssertCallback = NULL,
            bool initCOM = true, bool multithreadedCOM = true
#ifdef RENAME_MDF4LIB_DLLS
            ,
            VRenameDllMap *pDllMap = NULL
#endif
  )
  {
    if (IsInitialized())
    {
      LogMessage(_T("VMdf4Lib: only initialize once!\n"));
      return false; // do not initialize twice!
    }

    // open library
    mhLibrary = ::LoadLibrary(mdf4LibDllPath ? mdf4LibDllPath : GetFullMdf4LibPath()); // if DLL is not in same dir as EXE, path must be specified!
    if (!mhLibrary)
    {
      if (mdf4LibDllPath)
      {
        LogMessage(_T("VMdf4Lib: unable to load DLL from specified path \""));
        LogMessage(mdf4LibDllPath);
        LogMessage(_T("\"\n"));
      }
      else
      {
        LogMessage(_T("VMdf4Lib: unable to load DLL from default path \""));
        LogMessage(GetFullMdf4LibPath());
        LogMessage(_T("\"\n"));
      }
      return false;
    }

    // check interface compatibility
    MDF4Lib::CheckInterfaceCompatibilityPtr CheckInterfaceCompatibility = (MDF4Lib::CheckInterfaceCompatibilityPtr)::GetProcAddress(mhLibrary, "CheckInterfaceCompatibility");
    if (!CheckInterfaceCompatibility)
    {
      LogMessage(_T("VMdf4Lib: unable to get proc address for CheckInterfaceCompatibility\n"));
      return false;
    }

    if (!CheckInterfaceCompatibility(MDF4LIB_INTERFACE_MAJOR_VERSION_NO, MDF4LIB_INTERFACE_MINOR_VERSION_NO, MDF4LIB_INTERFACE_REVISON_NO, IS_UNICODE))
    {
      LogMessage(_T("VMdf4Lib: compatibility check failed\n"));
      return false;
    }

    // register assert callback
    if (pAssertCallback)
    {
      MDF4Lib::SetAssertCallbackPtr SetAssertCallback = (MDF4Lib::SetAssertCallbackPtr)::GetProcAddress(mhLibrary, "SetAssertCallback");
      if (!SetAssertCallback)
      {
        LogMessage(_T("VMdf4Lib: unable to get proc address for SetAssertCallback\n"));
      }
      else
      {
        SetAssertCallback(pAssertCallback);
      }
    }

    // register assert callback for ourself and (if specified) from user
    RegisterUnloadDllCallback(this);

#ifdef RENAME_MDF4LIB_DLLS
    if (pDllMap)
    {
      MDF4Lib::RenameDllPtr RenameDll = (MDF4Lib::RenameDllPtr)::GetProcAddress(mhLibrary, "RenameDll");
      if (!RenameDll)
      {
        LogMessage(_T("VMdf4Lib: unable to get proc address for RenameDll\n"));
        return false;
      }

      for (VRenameDllMap::iterator it = pDllMap->begin(); it != pDllMap->end(); ++it)
      {
        LPCTSTR orgName = it->first.c_str();
        LPCTSTR newName = it->second.c_str();
        if (!RenameDll(orgName, newName))
        {
          LogMessage(_T("VMdf4Lib: RenameDll failed\n"));
          return false;
        }
      }
    }
#endif

    if (!initCOM || !multithreadedCOM)
    {
      // if we have not the default COM parameters, we explicitly must set them before(!) the first call to ::GetFileManager!
      MDF4Lib::SetCoInitializeParametersPtr SetCoInitializeParameters = (MDF4Lib::SetCoInitializeParametersPtr)::GetProcAddress(mhLibrary, "SetCoInitializeParameters");
      if (!SetCoInitializeParameters)
      {
        LogMessage(_T("VMdf4Lib: unable to get proc address for SetCoInitializeParameters\n"));
      }
      else if (!SetCoInitializeParameters(initCOM, multithreadedCOM))
      {
        LogMessage(_T("VMdf4Lib: SetCoInitializeParameters returned false\n"));
      }
    }

    MDF4Lib::GetFileManagerPtr GetFileManager = (MDF4Lib::GetFileManagerPtr)::GetProcAddress(mhLibrary, "GetFileManager");
    if (!GetFileManager)
    {
      LogMessage(_T("VMdf4Lib: unable to get proc address for GetFileManager\n"));
      return false;
    }
    mpFileManager = GetFileManager(customerName, serialNo, regKey);
    if (!mpFileManager)
    {
      LogMessage(_T("VMdf4Lib: GetFileManager failed\n"));
    }
    return true;
  }

  ///////////////////////////////////////////////////
  //! Register a callback to be called upon DLL unload
  //! \param[in]  pUnloadDllCallback the callback to be registered
  //! \return     true on success, false otherwise
  bool RegisterUnloadDllCallback(MDF4Lib::IUnloadDllCallback *pUnloadDllCallback)
  {
    if (!mhLibrary || !pUnloadDllCallback)
    {
      return false;
    }

    MDF4Lib::RegisterUnloadDllCallbackPtr RegisterUnloadDllCallback =
        (MDF4Lib::RegisterUnloadDllCallbackPtr)::GetProcAddress(mhLibrary, "RegisterUnloadDllCallback");

    if (!RegisterUnloadDllCallback)
    {
      LogMessage(_T("VMdf4Lib: unable to get proc address for RegisterUnloadDllCallback\n"));
      return false;
    }

    return RegisterUnloadDllCallback(pUnloadDllCallback) == 0;
  }

  ///////////////////////////////////////////////////
  //! Unregister a callback to be called upon DLL unload
  //! \param[in]  pUnloadDllCallback the callback to be unregistered
  //! \return     true on success, false otherwise
  bool UnregisterUnloadDllCallback(MDF4Lib::IUnloadDllCallback *pUnloadDllCallback)
  {
    if (!mhLibrary || !pUnloadDllCallback)
    {
      return false;
    }

    MDF4Lib::UnregisterUnloadDllCallbackPtr UnregisterUnloadDllCallback =
        (MDF4Lib::UnregisterUnloadDllCallbackPtr)::GetProcAddress(mhLibrary, "UnregisterUnloadDllCallback");

    if (!UnregisterUnloadDllCallback)
    {
      LogMessage(_T("VMdf4Lib: unable to get proc address for UnregisterUnloadDllCallback\n"));
      return false;
    }
    return UnregisterUnloadDllCallback(pUnloadDllCallback) == 0;
  }

  // implementation of IUnloadDllCallback
protected:
  virtual void MDFLIBCALL OnDestroyFileManager(bool refCountIsZero)
  {
    if (!refCountIsZero)
    {
      mpFileManager = NULL;
      mhLibrary = NULL;
    }
  }

  void LogMessage(LPCTSTR msg)
  {
#ifdef _WINBASE_
    ::OutputDebugString(msg);
#endif
  }

protected:
  HMODULE mhLibrary;
  MDF4Lib::IFileManager *mpFileManager;
  TCHAR mFullMdf4LibPathName[_MAX_PATH + 1];

}; // eof class VMdf4Lib

#else // LINUX

#include <dlfcn.h>  // For dlopen, dlsym, dlerror, dlclose
#include <syslog.h> // For syslog

////////////////////////////////////////////////////////////////////////
// helper class that opens MDF4Lib in constructor and closes it in destructor
class VMdf4Lib
{
protected:
  void *mhLibrary;
  MDF4Lib::IFileManager *mpFileManager;

protected:
  void LogMessage(const char *msg)
  {
    syslog(LOG_DEBUG, "%s", msg);
  }

public:
  ///////////////////////////////////////////////////
  //! constructor for MDF4 Lib wrapper in case we want delayed loading (call to Init required!)
  VMdf4Lib()
      : mhLibrary(nullptr), mpFileManager(nullptr)
  {
  }

  ///////////////////////////////////////////////////
  //! constructor for MDF4 Lib wrapper
  //! parameters see Init
  VMdf4Lib(const char *customerName,
           const char *serialNo,
           const char *regKey,
           const char *mdf4LibDllPath = nullptr,
           MDF4Lib::IAssertCallback *pAssertCallback = nullptr)
      : mhLibrary(nullptr), mpFileManager(nullptr)
  {
    Init(customerName, serialNo, regKey, mdf4LibDllPath, pAssertCallback);
  }

  ///////////////////////////////////////////////////
  virtual ~VMdf4Lib()
  {
    if (mpFileManager)
      mpFileManager->Release();

    if (mhLibrary)
    {
      dlclose(mhLibrary);
      mhLibrary = nullptr;
    }
  }

  ///////////////////////////////////////////////////
  //! get the IFileManager
  //! \return smart pointer for IFileManager interface
  //! <br><br>Please assign to a smart pointer object (IFileManagerPtr) to have a balanced reference counting.
  IFileManagerPtr GetFileManager() { return mpFileManager ? mpFileManager->GetSelf() : nullptr; }

  ///////////////////////////////////////////////////
  bool IsInitialized() { return mhLibrary != nullptr; }

  ///////////////////////////////////////////////////
  //! Init for later initialization in case default constructor is used. Only call once!
  //! \param[in]  customerName string with customer name as given with registration information when purchasing the library.
  //! \param[in]  serialNumber string with serial number as given with registration information when purchasing the library.
  //! \param[in]  registrationKey string with registration key as given with registration information when purchasing the library.
  //! \param[in]  mdf4LibDllPath path and name of MDF4 Lib DLL. If not specified, the default DLL name is used without a path.
  //!             <br>Default: NULL (i.e. default DLL name is used)
  //! \param[in]  pAssertCallback pointer to IAssertCallback that will from now on receive asserts. NULL to unregister callback.
  //!             <br>See also ::SetAssertCallback.
  //!             <br>Default: NULL
  bool Init(const char *customerName,
            const char *serialNo,
            const char *regKey,
            const char *mdf4LibDllPath = nullptr,
            MDF4Lib::IAssertCallback *pAssertCallback = nullptr
#ifdef RENAME_MDF4LIB_DLLS
            ,
            VRenameDllMap *pDllMap = nullptr
#endif
  )
  {
    if (IsInitialized())
    {
      LogMessage("only initialize once!\n");
      return false; // do not initialize twice!
    }

    mhLibrary = dlopen(mdf4LibDllPath ? mdf4LibDllPath : MDF4LIB_DLL_NAME, RTLD_LAZY); // if DLL is not in same dir as EXE, path must be specified!
    if (mhLibrary == nullptr)
    {
      fprintf(stderr, "dlopen failed: %s\n", dlerror());
      if (mdf4LibDllPath)
        LogMessage("unable to load shared library from specified path\n");
      else
        LogMessage("unable to load shared library from default path \"" MDF4LIB_DLL_NAME "\"\n");
      return false;
    }

    dlerror(); // Clear any existing error

    // check interface compatibility
    MDF4Lib::CheckInterfaceCompatibilityPtr CheckInterfaceCompatibility = reinterpret_cast<MDF4Lib::CheckInterfaceCompatibilityPtr>(dlsym(mhLibrary, "CheckInterfaceCompatibility"));
    if (CheckInterfaceCompatibility == nullptr)
    {
      LogMessage("unable to get proc address for CheckInterfaceCompatibility\n");
      return false;
    }

    if (!CheckInterfaceCompatibility(MDF4LIB_INTERFACE_MAJOR_VERSION_NO, MDF4LIB_INTERFACE_MINOR_VERSION_NO, MDF4LIB_INTERFACE_REVISON_NO, IS_UNICODE))
    {
      LogMessage("compatibility check failed\n");
      return false;
    }

    // register assert callback
    if (pAssertCallback != nullptr)
    {
      MDF4Lib::SetAssertCallbackPtr SetAssertCallback = reinterpret_cast<MDF4Lib::SetAssertCallbackPtr>(dlsym(mhLibrary, "SetAssertCallback"));
      if (SetAssertCallback != nullptr)
        SetAssertCallback(pAssertCallback);
    }

#ifdef RENAME_MDF4LIB_DLLS
    if (pDllMap != nullptr)
    {
      MDF4Lib::RenameDllPtr RenameDll = reinterpret_cast<MDF4Lib::RenameDllPtr>(dlsym(mhLibrary, "RenameDll"));
      if (!RenameDll)
      {
        LogMessage(_T("VMdf4Lib: unable to get proc address for RenameDll\n"));
        return false;
      }

      for (VRenameDllMap::iterator it = pDllMap->begin(); it != pDllMap->end(); ++it)
      {
        LPCTSTR orgName = it->first.c_str();
        LPCTSTR newName = it->second.c_str();
        if (!RenameDll(orgName, newName))
        {
          LogMessage(_T("VMdf4Lib: RenameDll failed\n"));
          return false;
        }
      }
    }
#endif

    MDF4Lib::GetFileManagerPtr GetFileManager = reinterpret_cast<MDF4Lib::GetFileManagerPtr>(dlsym(mhLibrary, "GetFileManager"));
    if (GetFileManager == nullptr)
    {
      LogMessage("unable to get proc address for GetFileManager\n");
      return false;
    }
    mpFileManager = GetFileManager(customerName, serialNo, regKey);
    if (mpFileManager == nullptr)
    {
      LogMessage("GetFileManager failed\n");
    }
    return true;
  }

  ///////////////////////////////////////////////////
  //! Register a callback to be called upon DLL unload
  //! \param[in]  pUnloadDllCallback the callback to be registered
  //! \return     true on success, false otherwise
  bool RegisterUnloadDllCallback(MDF4Lib::IUnloadDllCallback * /*pUnloadDllCallback*/)
  {
    // TODO: Implement!
    return true;
  }

  ///////////////////////////////////////////////////
  //! Unregister a callback to be called upon DLL unload
  //! \param[in]  pUnloadDllCallback the callback to be unregistered
  //! \return     true on success, false otherwise
  bool UnregisterUnloadDllCallback(MDF4Lib::IUnloadDllCallback * /*pUnloadDllCallback*/)
  {
    // TODO: Implement!
    return true;
  }
};

#endif // !LINUX

using namespace MDF4Lib;

#endif