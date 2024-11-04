#include "Cast.hpp"

namespace mm
{

template <>
Vec2 Cast<Vec2, ImVec2>(const ImVec2& v)
{
	return Vec2{ v.x, v.y };
}

}