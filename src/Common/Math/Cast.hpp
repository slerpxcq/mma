#pragma once

#include "../Base.hpp"

#include <btBulletDynamicsCommon.h>
#include <imgui.h>

namespace mm
{

template <typename Dst, typename Src> Dst Cast(const Src& src);
template <> Vec2 Cast<Vec2, ImVec2>(const ImVec2& v);

}
