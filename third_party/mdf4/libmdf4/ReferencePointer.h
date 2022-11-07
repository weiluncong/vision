/*----------------------------------------------------------------------------
| File: ReferencePointer.h
| Project: CANape/CANgraph
|
| Description:
|    Smart Pointer class for classes that implement reference counting (e.g. Recorder)
|
|-----------------------------------------------------------------------------
| Copyright (c) by Vector Informatik GmbH.  All rights reserved.
 ----------------------------------------------------------------------------*/

#ifndef __REFERENCE_POINTER_H__
  #define __REFERENCE_POINTER_H__

//----------------------------------------------------------------------------
// template class VReferencePointer
//
// this class acts like a SmartPointer
// construct with an interface/class T that implements public methods GetSelf() and Release() and use like a T*
// the class ensures that the instance is not deleted during lifetime of the class
// the respective class should implement reference counting according to the following pattern:
// 1. implementation of GetSelf() to get a pointer of itself and icrementing the reference counter
// 2. Release() to decrement the reference counter
// 3. replace each pointer to the interface with the smart pointer class
// (i.e. ISomeThing* pSomeThing  =>   VReferencePointer<ISomeThing> pSomeThing)
//
// Caution: for MDF4Lib Interfaces, the Get/Create methods always return an object that has an increment reference counter.
// As a result, for correct reference counting, always assign the result of such a method to a VReferencePointer smart pointer.
// What will not work is the following:
/* \code

IInterfacePointer* pRawPointer = GetPointer();

VReferencePointer<IInterfacePointer> refPtr1 = pRawPointer;
VReferencePointer<IInterfacePointer> refPtr2 = pRawPointer;

// => will crash when leaving the scope!

\endcode

However, the following will work:

\code

VReferencePointer<IInterfacePointer> pRawPointer = GetPointer();

VReferencePointer<IInterfacePointer> refPtr1 = pRawPointer;
VReferencePointer<IInterfacePointer> refPtr2 = pRawPointer;

\endcode


*/

#ifndef ASSERT
#  ifdef DEBUG
#    include <cassert>
#    define ASSERT(_x_) assert(_x_)
#  else
#    define ASSERT(_x_)
#  endif
#endif

template <class T> class VReferencePointer
{
  public:
    // constructor
    VReferencePointer(): mInstance(NULL) {}
    VReferencePointer(T* instance) : mInstance(instance) {}

    // copy constructor
    VReferencePointer(const VReferencePointer<T>& src) : mInstance(src.GetInstance() ? src.GetInstance()->GetSelf() : NULL) {}

    // destructor
    ~VReferencePointer() { if (mInstance) mInstance->Release(); mInstance = NULL; }

    // type cast operator
    inline operator T*() { return mInstance; }

    // type cast operator
    inline operator bool() { return (mInstance != NULL); }

    // type cast operator
    inline operator const T*() const { return mInstance; }

    // member access operator: allow usage like T* pointer (prior please check IsValid())
    inline T* operator->() const { ASSERT(IsValid()); return mInstance; }
    inline T* operator->() { ASSERT(IsValid()); return mInstance; }

    bool operator!() const { return !IsValid(); }

    bool IsValid() const { return (mInstance != NULL); }

    // assignment operator for T* (attention: must not be const T*)
    // we assume that rhs already has incremented reference counter!
    VReferencePointer<T>& operator=(T* rhs) {
      T* pOld = mInstance;
      mInstance = rhs;
      if (pOld) pOld->Release();
      return *this;
    };

    // assignment operator
    VReferencePointer<T>& operator=(const VReferencePointer<T>& rhs) {
      return this->operator=(rhs.GetInstance() ? rhs.GetInstance()->GetSelf() : NULL);
    };

    // comparison operator: compare pointers
    bool operator==(const T* rhs) const { return rhs == mInstance; }

    // negative comparison operator: compare pointers
    bool operator!=(const T* rhs) const { return rhs != mInstance; }

    // comparison operator: compare pointers
    bool operator==(T* rhs) const { return rhs == mInstance; }

    // negative comparison operator: compare pointers
    bool operator!=(T* rhs) const { return rhs != mInstance; }

    // comparison operator: compare pointers
    bool operator==(const VReferencePointer<T>& rhs) const { return rhs.GetInstance() == mInstance; }

    // negative comparison operator: compare pointers
    bool operator!=(const VReferencePointer<T>& rhs) const { return rhs.GetInstance() != mInstance; }

  protected:
    T* mInstance;
    T* GetInstance() const { return mInstance; }

}; // class VReferencePointer

#endif // __REFERENCE_POINTER_H__