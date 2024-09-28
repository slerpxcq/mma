#pragma once

#include <cstdint>
#include <cstddef>

#define DECL_SINGLETON(_class_) \
public: \
static void CreateInstance() { s_instance = new _class_{}; } \
static void DestroyInstance() { delete s_instance; } \
_class_& GetInstance() { return *s_instance; } \
private: \
static inline _class_* s_instance{}; \
_class_() = default;  

#define GLOBAL_ATTRIBUTE(_type_, _name_)  \
class _name_ { \
friend _type_ Get##_name_(); \
friend void Set##_name_(_type_); \
static inline _type_ data; \
}; \
static inline _type_ Get##_name_() { return _name_::data; } \
static inline void Set##_name_(_type_ x) { _name_::data = x; } \

namespace mm
{
using u64 = std::uint64_t;
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8 = std::byte;
using i64 = std::int64_t;
using i32 = std::int32_t;
using i16 = std::int16_t;
using i8 = std::int8_t;
using f32 = float;
using f64 = double;
}

