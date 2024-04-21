#pragma once

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
		Transform() = default;
		Transform(const glm::vec3& translation, const glm::quat& rotation) :
			translation(translation),
			rotation(rotation) {}

		Transform(const glm::mat4& mat) :
			translation(mat[3]),
			rotation(glm::toQuat(glm::mat3(mat))) {}

		inline Transform inverse() const {
			glm::quat rotInv = glm::inverse(rotation);
			return { -glm::rotate(rotInv, translation),
				 rotInv };
		}

		inline Transform operator*(float s) {
			return { s * translation,
				 glm::slerp(glm::identity<glm::quat>(), rotation, s) };
		}

		inline Transform operator*(const Transform& rhs) const {
			return { glm::rotate(this->rotation, rhs.translation) + this->translation,
				 this->rotation * rhs.rotation };
		}

		static inline Transform identity() {
			return { glm::vec3(0), glm::identity<glm::quat>() };
		}

		static inline glm::mat4 toMat4(const Transform& x) {
			return glm::translate(glm::mat4(1.f), x.translation) * glm::toMat4(x.rotation);
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Transform& x)
	{
		return os << glm::to_string(x.translation) << "\n" <<
			glm::to_string(x.rotation);
	}
}


