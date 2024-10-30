#pragma once

#include <cstdint>
#include <cstddef>
#include <functional>
#include <stdexcept>

#define MM_GLOBAL_ATTRIBUTE(_type_, _name_)  \
class _name_##_g { \
friend _type_& Get##_name_(); \
template <typename T> \
friend void Set##_name_(T&&); \
static inline _type_ data{}; \
}; \
static inline _type_& Get##_name_() { return _name_##_g::data; } \
template <typename T> \
static inline void Set##_name_(T&& x) { _name_##_g::data = std::forward<T>(x); } 

#define MM_APP_ENTRY(_appname_) \
int main(int argc, char** argv) { return std::make_unique<mm::_appname_>(argc, argv)->Run(); }

namespace mm
{

using u64 = std::uint64_t;
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8 = std::uint8_t;
using i64 = std::int64_t;
using i32 = std::int32_t;
using i16 = std::int16_t;
using i8 = std::int8_t;
using f32 = float;
using f64 = double;
using Byte = std::byte;

template <typename T>
using Function = std::function<T>;

using RuntimeError = std::runtime_error;

}

