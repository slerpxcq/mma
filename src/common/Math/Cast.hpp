#pragma once

#include "../Base.hpp"

#include <btBulletDynamicsCommon.h>
#include <imgui.h>

namespace mm
{

template <typename Dst, typename Src>
Dst Cast(const Src& src);

template <>
Vec2 Cast<Vec2, ImVec2>(const ImVec2& v)
{
	return Vec2{ v.x, v.y };
}

// ***

//static inline btVector3 btVec3FromArray(const float* a)
//{
//	return btVector3(a[0], a[1], a[2]);
//}
//
//static inline btVector3 btVec3FromGLM(const glm::vec3& v)
//{
//	return btVector3(v.x, v.y, v.z);
//}
//
//static inline btVector3 btEulerFromArray(const float* a)
//{
//	return btVector3(a[1], a[0], a[2]);
//}
//
//static inline btQuaternion btQuatFromGLM(const glm::quat& q)
//{
//	return btQuaternion(q.x, q.y, q.z, q.w);
//}
//
//static inline glm::vec3 btVec3ToGLM(const btVector3& v)
//{
//	return glm::vec3(v.getX(), v.getY(), v.getZ());
//}
//
//static inline glm::quat btQuatToGLM(const btQuaternion& q)
//{
//	return glm::quat(q.getW(), q.getX(), q.getY(), q.getZ());
//}

}
