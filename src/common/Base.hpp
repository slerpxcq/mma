#pragma once

#include <cstdint>
#include <cstddef>
#include <functional>
#include <optional>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>

#include <glm/glm.hpp>

#define MM_GLOBAL_OBJECT(_type_, _name_)  \
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

#define MakePair std::make_pair
#define Move std::move
#define Forward std::forward

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

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Vec2i = glm::ivec2;
using Vec3i = glm::ivec3;
using Vec4i = glm::ivec4;
using Vec2u = glm::uvec2;
using Vec3u = glm::uvec3;
using Vec4u = glm::uvec4;
using Mat3 = glm::mat3;
using Mat4 = glm::mat4;
using Quat = glm::quat;
using Color = Vec4;

using FileStream = std::fstream;
using InFileStream = std::ifstream;
using OutFileStream = std::ofstream;

template <typename T>
using Func = std::function<T>;

template <typename T>
using Opt = std::optional<T>;

template <typename T, typename U>
using Pair = std::pair<T, U>; 

template <typename T, size_t Size>
using Array = std::array<T, Size>;

template <typename T>
using InitList = std::initializer_list<T>;

using RuntimeError = std::runtime_error;
using Path = std::filesystem::path;



}

