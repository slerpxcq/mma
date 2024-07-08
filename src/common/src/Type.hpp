#pragma once

#include <btBulletDynamicsCommon.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <imgui.h>

namespace mm
{
	static inline btVector3 btVec3FromArray(const float* a)
	{
		return btVector3(a[0], a[1], a[2]);
	}

	static inline btVector3 btVec3FromGLM(const glm::vec3& v)
	{
		return btVector3(v.x, v.y, v.z);
	}

	static inline btVector3 btEulerFromArray(const float* a)
	{
		return btVector3(a[1], a[0], a[2]);
	}

	static inline btQuaternion btQuatFromGLM(const glm::quat& q)
	{
		return btQuaternion(q.x, q.y, q.z, q.w);
	}

	static inline glm::vec3 btVec3ToGLM(const btVector3& v)
	{
		return glm::vec3(v.getX(), v.getY(), v.getZ());
	}

	static inline glm::quat btQuatToGLM(const btQuaternion& q)
	{
		return glm::quat(q.getW(), q.getX(), q.getY(), q.getZ());
	}

	static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
	{
		return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs)
	{
		return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	static inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs)
	{
		return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y);
	}

	static inline ImVec2 operator/(const ImVec2& lhs, const ImVec2& rhs)
	{
		return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y);
	}

	static inline ImVec2 operator*(const ImVec2& lhs, float rhs)
	{
		return ImVec2(lhs.x * rhs, lhs.y * rhs);
	}

	static inline ImVec2 operator/(const ImVec2& lhs, float rhs)
	{
		return ImVec2(lhs.x / rhs, lhs.y / rhs);
	}

	static inline ImVec2 operator*(float lhs, const ImVec2& rhs)
	{
		return ImVec2(lhs * rhs.x, lhs * rhs.y);
	}

	static inline bool operator==(const ImVec2& lhs, const ImVec2& rhs)
	{
		static constexpr float eps = std::numeric_limits<float>::epsilon();
		return std::fabsf(lhs.x - rhs.x) <= eps && std::fabsf(lhs.y - rhs.y) <= eps;
	}

	static inline bool operator!=(const ImVec2& lhs, const ImVec2& rhs)
	{
		return !(lhs == rhs);
	}
}
