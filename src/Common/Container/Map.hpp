#pragma once

#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>

namespace mm
{

template <typename K, typename V> using HashMap = std::unordered_map<K, V>;
template <typename K, typename V> using Map = std::map<K, V>;
template <typename K> using Set = std::set<K>;
template <typename K> using HashSet = std::unordered_set<K>;

}

