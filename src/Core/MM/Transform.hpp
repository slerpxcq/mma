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
		glm::vec3 trans;
		glm::quat rot;

		Transform() = default;
		Transform(const glm::vec3& trans, const glm::quat& rot) :
			trans(trans),
			rot(rot) {}

		Transform(const glm::mat4& mat) :
			trans(mat[3]),
			rot(glm::toQuat(glm::mat3(mat))) {}

		inline Transform inverse() const {
			glm::quat rotInv = glm::inverse(rot);
			return { -glm::rotate(rotInv, trans),
				 rotInv };
		}

		inline Transform operator*(float s) {
			return { s * trans,
				 glm::slerp(glm::identity<glm::quat>(), rot, s) };
		}

		inline Transform operator*(const Transform& rhs) const {
			return { glm::rotate(this->rot, rhs.trans) + this->trans,
				 this->rot * rhs.rot };
		}

		static inline Transform identity() {
			return { glm::vec3(0), glm::identity<glm::quat>() };
		}

		static inline glm::mat4 toMat4(const Transform& x) {
			return glm::translate(glm::mat4(1.f), x.trans) * glm::toMat4(x.rot);
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Transform& x)
	{
		return os << glm::to_string(x.trans) << "\n" <<
			glm::to_string(x.rot);
	}
}


