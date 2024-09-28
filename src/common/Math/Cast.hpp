#pragma once

#include <btBulletDynamicsCommon.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace mm
{

template <typename Src, typename Dst>
Dst Cast(const Src& s);

template <>
typename glm::vec3 Cast<btVector3, glm::vec3>(const btVector3& s)
{
	return {};
}

template <> 
glm::quat Cast<btQuaternion, glm::quat>(const btQuaternion& q)
{
	return {};
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
