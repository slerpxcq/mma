#pragma once

#include <unordered_map>
#include <map>

namespace mm
{

template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

template <typename K, typename V>
using Map = std::map<K, V>;

}

