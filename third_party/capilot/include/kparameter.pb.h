// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: kparameter.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_kparameter_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_kparameter_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3020000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3020001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_kparameter_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_kparameter_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_kparameter_2eproto;
class KParamReply;
struct KParamReplyDefaultTypeInternal;
extern KParamReplyDefaultTypeInternal _KParamReply_default_instance_;
class KParamRequest;
struct KParamRequestDefaultTypeInternal;
extern KParamRequestDefaultTypeInternal _KParamRequest_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::KParamReply* Arena::CreateMaybeMessage<::KParamReply>(Arena*);
template<> ::KParamRequest* Arena::CreateMaybeMessage<::KParamRequest>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

// ===================================================================

class KParamRequest final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:KParamRequest) */ {
 public:
  inline KParamRequest() : KParamRequest(nullptr) {}
  ~KParamRequest() override;
  explicit PROTOBUF_CONSTEXPR KParamRequest(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  KParamRequest(const KParamRequest& from);
  KParamRequest(KParamRequest&& from) noexcept
    : KParamRequest() {
    *this = ::std::move(from);
  }

  inline KParamRequest& operator=(const KParamRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline KParamRequest& operator=(KParamRequest&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const KParamRequest& default_instance() {
    return *internal_default_instance();
  }
  static inline const KParamRequest* internal_default_instance() {
    return reinterpret_cast<const KParamRequest*>(
               &_KParamRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(KParamRequest& a, KParamRequest& b) {
    a.Swap(&b);
  }
  inline void Swap(KParamRequest* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(KParamRequest* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  KParamRequest* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<KParamRequest>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const KParamRequest& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const KParamRequest& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(KParamRequest* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "KParamRequest";
  }
  protected:
  explicit KParamRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNodeNameFieldNumber = 2,
    kTaskNameFieldNumber = 3,
    kParamNameFieldNumber = 4,
    kParamValStrFieldNumber = 5,
    kModeFieldNumber = 1,
  };
  // string node_name = 2;
  void clear_node_name();
  const std::string& node_name() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_node_name(ArgT0&& arg0, ArgT... args);
  std::string* mutable_node_name();
  PROTOBUF_NODISCARD std::string* release_node_name();
  void set_allocated_node_name(std::string* node_name);
  private:
  const std::string& _internal_node_name() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_node_name(const std::string& value);
  std::string* _internal_mutable_node_name();
  public:

  // string task_name = 3;
  void clear_task_name();
  const std::string& task_name() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_task_name(ArgT0&& arg0, ArgT... args);
  std::string* mutable_task_name();
  PROTOBUF_NODISCARD std::string* release_task_name();
  void set_allocated_task_name(std::string* task_name);
  private:
  const std::string& _internal_task_name() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_task_name(const std::string& value);
  std::string* _internal_mutable_task_name();
  public:

  // string param_name = 4;
  void clear_param_name();
  const std::string& param_name() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_param_name(ArgT0&& arg0, ArgT... args);
  std::string* mutable_param_name();
  PROTOBUF_NODISCARD std::string* release_param_name();
  void set_allocated_param_name(std::string* param_name);
  private:
  const std::string& _internal_param_name() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_param_name(const std::string& value);
  std::string* _internal_mutable_param_name();
  public:

  // string param_val_str = 5;
  void clear_param_val_str();
  const std::string& param_val_str() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_param_val_str(ArgT0&& arg0, ArgT... args);
  std::string* mutable_param_val_str();
  PROTOBUF_NODISCARD std::string* release_param_val_str();
  void set_allocated_param_val_str(std::string* param_val_str);
  private:
  const std::string& _internal_param_val_str() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_param_val_str(const std::string& value);
  std::string* _internal_mutable_param_val_str();
  public:

  // int32 mode = 1;
  void clear_mode();
  int32_t mode() const;
  void set_mode(int32_t value);
  private:
  int32_t _internal_mode() const;
  void _internal_set_mode(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:KParamRequest)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr node_name_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr task_name_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr param_name_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr param_val_str_;
  int32_t mode_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_kparameter_2eproto;
};
// -------------------------------------------------------------------

class KParamReply final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:KParamReply) */ {
 public:
  inline KParamReply() : KParamReply(nullptr) {}
  ~KParamReply() override;
  explicit PROTOBUF_CONSTEXPR KParamReply(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  KParamReply(const KParamReply& from);
  KParamReply(KParamReply&& from) noexcept
    : KParamReply() {
    *this = ::std::move(from);
  }

  inline KParamReply& operator=(const KParamReply& from) {
    CopyFrom(from);
    return *this;
  }
  inline KParamReply& operator=(KParamReply&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const KParamReply& default_instance() {
    return *internal_default_instance();
  }
  static inline const KParamReply* internal_default_instance() {
    return reinterpret_cast<const KParamReply*>(
               &_KParamReply_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(KParamReply& a, KParamReply& b) {
    a.Swap(&b);
  }
  inline void Swap(KParamReply* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(KParamReply* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  KParamReply* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<KParamReply>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const KParamReply& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const KParamReply& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(KParamReply* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "KParamReply";
  }
  protected:
  explicit KParamReply(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kErrorInfoFieldNumber = 2,
    kParamStrFieldNumber = 3,
    kResFieldNumber = 1,
  };
  // string error_info = 2;
  void clear_error_info();
  const std::string& error_info() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_error_info(ArgT0&& arg0, ArgT... args);
  std::string* mutable_error_info();
  PROTOBUF_NODISCARD std::string* release_error_info();
  void set_allocated_error_info(std::string* error_info);
  private:
  const std::string& _internal_error_info() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_error_info(const std::string& value);
  std::string* _internal_mutable_error_info();
  public:

  // string param_str = 3;
  void clear_param_str();
  const std::string& param_str() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_param_str(ArgT0&& arg0, ArgT... args);
  std::string* mutable_param_str();
  PROTOBUF_NODISCARD std::string* release_param_str();
  void set_allocated_param_str(std::string* param_str);
  private:
  const std::string& _internal_param_str() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_param_str(const std::string& value);
  std::string* _internal_mutable_param_str();
  public:

  // bool res = 1;
  void clear_res();
  bool res() const;
  void set_res(bool value);
  private:
  bool _internal_res() const;
  void _internal_set_res(bool value);
  public:

  // @@protoc_insertion_point(class_scope:KParamReply)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr error_info_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr param_str_;
  bool res_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_kparameter_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// KParamRequest

// int32 mode = 1;
inline void KParamRequest::clear_mode() {
  mode_ = 0;
}
inline int32_t KParamRequest::_internal_mode() const {
  return mode_;
}
inline int32_t KParamRequest::mode() const {
  // @@protoc_insertion_point(field_get:KParamRequest.mode)
  return _internal_mode();
}
inline void KParamRequest::_internal_set_mode(int32_t value) {
  
  mode_ = value;
}
inline void KParamRequest::set_mode(int32_t value) {
  _internal_set_mode(value);
  // @@protoc_insertion_point(field_set:KParamRequest.mode)
}

// string node_name = 2;
inline void KParamRequest::clear_node_name() {
  node_name_.ClearToEmpty();
}
inline const std::string& KParamRequest::node_name() const {
  // @@protoc_insertion_point(field_get:KParamRequest.node_name)
  return _internal_node_name();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void KParamRequest::set_node_name(ArgT0&& arg0, ArgT... args) {
 
 node_name_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:KParamRequest.node_name)
}
inline std::string* KParamRequest::mutable_node_name() {
  std::string* _s = _internal_mutable_node_name();
  // @@protoc_insertion_point(field_mutable:KParamRequest.node_name)
  return _s;
}
inline const std::string& KParamRequest::_internal_node_name() const {
  return node_name_.Get();
}
inline void KParamRequest::_internal_set_node_name(const std::string& value) {
  
  node_name_.Set(value, GetArenaForAllocation());
}
inline std::string* KParamRequest::_internal_mutable_node_name() {
  
  return node_name_.Mutable(GetArenaForAllocation());
}
inline std::string* KParamRequest::release_node_name() {
  // @@protoc_insertion_point(field_release:KParamRequest.node_name)
  return node_name_.Release();
}
inline void KParamRequest::set_allocated_node_name(std::string* node_name) {
  if (node_name != nullptr) {
    
  } else {
    
  }
  node_name_.SetAllocated(node_name, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (node_name_.IsDefault()) {
    node_name_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:KParamRequest.node_name)
}

// string task_name = 3;
inline void KParamRequest::clear_task_name() {
  task_name_.ClearToEmpty();
}
inline const std::string& KParamRequest::task_name() const {
  // @@protoc_insertion_point(field_get:KParamRequest.task_name)
  return _internal_task_name();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void KParamRequest::set_task_name(ArgT0&& arg0, ArgT... args) {
 
 task_name_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:KParamRequest.task_name)
}
inline std::string* KParamRequest::mutable_task_name() {
  std::string* _s = _internal_mutable_task_name();
  // @@protoc_insertion_point(field_mutable:KParamRequest.task_name)
  return _s;
}
inline const std::string& KParamRequest::_internal_task_name() const {
  return task_name_.Get();
}
inline void KParamRequest::_internal_set_task_name(const std::string& value) {
  
  task_name_.Set(value, GetArenaForAllocation());
}
inline std::string* KParamRequest::_internal_mutable_task_name() {
  
  return task_name_.Mutable(GetArenaForAllocation());
}
inline std::string* KParamRequest::release_task_name() {
  // @@protoc_insertion_point(field_release:KParamRequest.task_name)
  return task_name_.Release();
}
inline void KParamRequest::set_allocated_task_name(std::string* task_name) {
  if (task_name != nullptr) {
    
  } else {
    
  }
  task_name_.SetAllocated(task_name, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (task_name_.IsDefault()) {
    task_name_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:KParamRequest.task_name)
}

// string param_name = 4;
inline void KParamRequest::clear_param_name() {
  param_name_.ClearToEmpty();
}
inline const std::string& KParamRequest::param_name() const {
  // @@protoc_insertion_point(field_get:KParamRequest.param_name)
  return _internal_param_name();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void KParamRequest::set_param_name(ArgT0&& arg0, ArgT... args) {
 
 param_name_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:KParamRequest.param_name)
}
inline std::string* KParamRequest::mutable_param_name() {
  std::string* _s = _internal_mutable_param_name();
  // @@protoc_insertion_point(field_mutable:KParamRequest.param_name)
  return _s;
}
inline const std::string& KParamRequest::_internal_param_name() const {
  return param_name_.Get();
}
inline void KParamRequest::_internal_set_param_name(const std::string& value) {
  
  param_name_.Set(value, GetArenaForAllocation());
}
inline std::string* KParamRequest::_internal_mutable_param_name() {
  
  return param_name_.Mutable(GetArenaForAllocation());
}
inline std::string* KParamRequest::release_param_name() {
  // @@protoc_insertion_point(field_release:KParamRequest.param_name)
  return param_name_.Release();
}
inline void KParamRequest::set_allocated_param_name(std::string* param_name) {
  if (param_name != nullptr) {
    
  } else {
    
  }
  param_name_.SetAllocated(param_name, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (param_name_.IsDefault()) {
    param_name_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:KParamRequest.param_name)
}

// string param_val_str = 5;
inline void KParamRequest::clear_param_val_str() {
  param_val_str_.ClearToEmpty();
}
inline const std::string& KParamRequest::param_val_str() const {
  // @@protoc_insertion_point(field_get:KParamRequest.param_val_str)
  return _internal_param_val_str();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void KParamRequest::set_param_val_str(ArgT0&& arg0, ArgT... args) {
 
 param_val_str_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:KParamRequest.param_val_str)
}
inline std::string* KParamRequest::mutable_param_val_str() {
  std::string* _s = _internal_mutable_param_val_str();
  // @@protoc_insertion_point(field_mutable:KParamRequest.param_val_str)
  return _s;
}
inline const std::string& KParamRequest::_internal_param_val_str() const {
  return param_val_str_.Get();
}
inline void KParamRequest::_internal_set_param_val_str(const std::string& value) {
  
  param_val_str_.Set(value, GetArenaForAllocation());
}
inline std::string* KParamRequest::_internal_mutable_param_val_str() {
  
  return param_val_str_.Mutable(GetArenaForAllocation());
}
inline std::string* KParamRequest::release_param_val_str() {
  // @@protoc_insertion_point(field_release:KParamRequest.param_val_str)
  return param_val_str_.Release();
}
inline void KParamRequest::set_allocated_param_val_str(std::string* param_val_str) {
  if (param_val_str != nullptr) {
    
  } else {
    
  }
  param_val_str_.SetAllocated(param_val_str, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (param_val_str_.IsDefault()) {
    param_val_str_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:KParamRequest.param_val_str)
}

// -------------------------------------------------------------------

// KParamReply

// bool res = 1;
inline void KParamReply::clear_res() {
  res_ = false;
}
inline bool KParamReply::_internal_res() const {
  return res_;
}
inline bool KParamReply::res() const {
  // @@protoc_insertion_point(field_get:KParamReply.res)
  return _internal_res();
}
inline void KParamReply::_internal_set_res(bool value) {
  
  res_ = value;
}
inline void KParamReply::set_res(bool value) {
  _internal_set_res(value);
  // @@protoc_insertion_point(field_set:KParamReply.res)
}

// string error_info = 2;
inline void KParamReply::clear_error_info() {
  error_info_.ClearToEmpty();
}
inline const std::string& KParamReply::error_info() const {
  // @@protoc_insertion_point(field_get:KParamReply.error_info)
  return _internal_error_info();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void KParamReply::set_error_info(ArgT0&& arg0, ArgT... args) {
 
 error_info_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:KParamReply.error_info)
}
inline std::string* KParamReply::mutable_error_info() {
  std::string* _s = _internal_mutable_error_info();
  // @@protoc_insertion_point(field_mutable:KParamReply.error_info)
  return _s;
}
inline const std::string& KParamReply::_internal_error_info() const {
  return error_info_.Get();
}
inline void KParamReply::_internal_set_error_info(const std::string& value) {
  
  error_info_.Set(value, GetArenaForAllocation());
}
inline std::string* KParamReply::_internal_mutable_error_info() {
  
  return error_info_.Mutable(GetArenaForAllocation());
}
inline std::string* KParamReply::release_error_info() {
  // @@protoc_insertion_point(field_release:KParamReply.error_info)
  return error_info_.Release();
}
inline void KParamReply::set_allocated_error_info(std::string* error_info) {
  if (error_info != nullptr) {
    
  } else {
    
  }
  error_info_.SetAllocated(error_info, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (error_info_.IsDefault()) {
    error_info_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:KParamReply.error_info)
}

// string param_str = 3;
inline void KParamReply::clear_param_str() {
  param_str_.ClearToEmpty();
}
inline const std::string& KParamReply::param_str() const {
  // @@protoc_insertion_point(field_get:KParamReply.param_str)
  return _internal_param_str();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void KParamReply::set_param_str(ArgT0&& arg0, ArgT... args) {
 
 param_str_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:KParamReply.param_str)
}
inline std::string* KParamReply::mutable_param_str() {
  std::string* _s = _internal_mutable_param_str();
  // @@protoc_insertion_point(field_mutable:KParamReply.param_str)
  return _s;
}
inline const std::string& KParamReply::_internal_param_str() const {
  return param_str_.Get();
}
inline void KParamReply::_internal_set_param_str(const std::string& value) {
  
  param_str_.Set(value, GetArenaForAllocation());
}
inline std::string* KParamReply::_internal_mutable_param_str() {
  
  return param_str_.Mutable(GetArenaForAllocation());
}
inline std::string* KParamReply::release_param_str() {
  // @@protoc_insertion_point(field_release:KParamReply.param_str)
  return param_str_.Release();
}
inline void KParamReply::set_allocated_param_str(std::string* param_str) {
  if (param_str != nullptr) {
    
  } else {
    
  }
  param_str_.SetAllocated(param_str, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (param_str_.IsDefault()) {
    param_str_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:KParamReply.param_str)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_kparameter_2eproto
