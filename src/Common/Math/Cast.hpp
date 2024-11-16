#pragma once

#include "../Base.hpp"

#include "Transform.hpp"
#include <imgui.h>
#include <btBulletDynamicsCommon.h>

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

template <> inline Quat Cast<Quat, btQuaternion>(const btQuaternion& q)
{
	return Quat{ q.getW(), q.getX(), q.getY(), q.getZ() };
}

template <> inline btQuaternion Cast<btQuaternion, Quat>(const Quat& q)
{
	return btQuaternion{ q.x, q.y, q.z, q.w };
}

template <> inline Vec3 Cast<Vec3, btVector3>(const btVector3& v)
{
	return Vec3{ v.getX(), v.getY(), v.getZ() };
}

template <> inline btVector3 Cast<btVector3, Vec3>(const Vec3& v)
{
	return btVector3{ v.x, v.y, v.z };
}

template <> inline Transform Cast<Transform, btTransform>(const btTransform& t)
{
	return Transform{ Cast<Vec3>(t.getOrigin()), Cast<Quat>(t.getRotation()) };
}

template <> inline btTransform Cast<btTransform, Transform>(const Transform& t)
{
	return btTransform{ Cast<btQuaternion>(t.rotation), Cast<btVector3>(t.translation) };
}

}
