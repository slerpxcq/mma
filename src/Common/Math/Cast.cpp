#include "Cast.hpp"

namespace mm
{

template <> Vec2 Cast<Vec2, ImVec2>(const ImVec2& v)
{
	return Vec2{ v.x, v.y };
}

template <> ImVec2 Cast<ImVec2, Vec2>(const Vec2& v)
{
	return ImVec2{ v.x, v.y };
}


}