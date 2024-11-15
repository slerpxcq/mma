#pragma once

#include "../Base.hpp"
#include "../Container/String.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

#include <iostream>

namespace mm
{

struct Transform
{
public:
	Vec3 translation;
	Quat rotation;

public:
	Transform(const Vec3& translation = Vec3{ 0 },
			  const Quat& rotation = glm::identity<Quat>()) :
		translation(translation),
		rotation(rotation) {}

	Transform(const Quat& rotation) :
		translation(Vec3{0}),
		rotation(rotation) {}

	Transform(const Mat4& mat) :
		translation(mat[3]),
		rotation(glm::toQuat(Mat3(mat))) {}

	Transform Inverse() const {
		glm::quat rotInv = glm::inverse(rotation);
		return { -glm::rotate(rotInv, translation), rotInv };
	}

	String ToString() const {
		return glm::to_string(translation) + "\n" + glm::to_string(rotation);
	}

	static Transform Identity() {
		return { glm::vec3(0), glm::identity<glm::quat>() };
	}

	glm::mat4 ToMat4() const {
		return glm::translate(glm::mat4(1.f), translation) * glm::toMat4(rotation);
	}

};

inline Transform operator*(const Transform& lhs, float s) 
{
	return { s * lhs.translation,
		     glm::slerp(glm::identity<Quat>(), lhs.rotation, s) };
}

inline Transform operator*(float s, const Transform& rhs) 
{
	return rhs * s;
}

inline Transform operator/(const Transform& lhs, float s) 
{
	return lhs * (1.f/s);
}

inline Transform operator*(const Transform& lhs, const Transform& rhs)  
{
	return { glm::rotate(lhs.rotation, rhs.translation) + lhs.translation,
		     lhs.rotation * rhs.rotation };
}

inline bool operator==(const Transform& lhs, const Transform& rhs) 
{
	return (lhs.translation == rhs.translation) && (lhs.rotation == rhs.rotation);
}

inline bool operator!=(const Transform& lhs, const Transform& rhs) 
{
	return !(lhs == rhs);
}

inline Transform operator+(const Transform& t, const Vec3& v) 
{
	return { t.translation + v, t.rotation };
}

inline Transform operator+(const Vec3& v, const Transform& t) 
{
	return { t.translation + v, t.rotation };
}

inline Transform operator*(const Transform& t, const Quat& q) 
{
	return t * Transform{ Vec3{0}, q };
}

inline Transform operator*(const Quat& q, const Transform& t) 
{
	return Transform{ Vec3{0}, q } * t;
}

}


