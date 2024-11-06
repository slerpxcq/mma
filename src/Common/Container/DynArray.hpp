#pragma once

#include <vector>

namespace mm
{

template <typename T, typename Allocator = std::allocator<T>>
using DynArray = std::vector<T, Allocator>;

}

