#pragma once

#include "../Base.hpp"

#include <btBulletDynamicsCommon.h>
#include <imgui.h>

namespace mm
{

template <typename Dst, typename Src> Dst Cast(const Src& src);

template <> inline Vec2 Cast<Vec2, ImVec2>(const ImVec2& v)
{
	return Vec2{ v.x, v.y };
}

template <> inline ImVec2 Cast<ImVec2, Vec2>(const Vec2& v)
{
	return ImVec2{ v.x, v.y };
}

}
