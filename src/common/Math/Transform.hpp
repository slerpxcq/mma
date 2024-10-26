#pragma once

#include "../Container/String.hpp"

#include <glm/glm.hpp>
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
	glm::vec3 translation;
	glm::quat rotation;

public:
	// Transform() { *this = Transform::Identity(); }
	Transform(const glm::vec3& translation = glm::vec3(0), 
			  const glm::quat& rotation = glm::identity<glm::quat>()) :
		translation(translation),
		rotation(rotation) {}

	Transform(const glm::mat4& mat) :
		translation(mat[3]),
		rotation(glm::toQuat(glm::mat3(mat))) {}

	Transform Inverse() const {
		glm::quat rotInv = glm::inverse(rotation);
		return { -glm::rotate(rotInv, translation), rotInv };
	}

	Transform operator*(float s) const {
		return { s * translation,
			 glm::slerp(glm::identity<glm::quat>(), rotation, s) };
	}

	Transform operator*(const Transform& rhs) const {
		return { glm::rotate(this->rotation, rhs.translation) + this->translation,
			 this->rotation * rhs.rotation };
	}

	String ToString() const {
		return glm::to_string(translation) + "\n" + glm::to_string(rotation);
	}

	static Transform Identity() {
		return { glm::vec3(0), glm::identity<glm::quat>() };
	}

	glm::mat4 ToMat4(const Transform& x) const {
		return glm::translate(glm::mat4(1.f), x.translation) * glm::toMat4(x.rotation);
	}

};

}


