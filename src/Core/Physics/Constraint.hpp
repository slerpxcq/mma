#pragma once

#include "PhysicsObject.hpp"
#include "../NamedObject.hpp"

namespace mm
{

class Rigidbody;
class Constraint : public PhysicsObject,
	               public NamedObject
{
public:
	enum class Type {
		GENERIC_6DOP_SPRING
	};

	struct ConstructInfo {
		Type type;
		StringView name{};
		Transform bindWorld{};
		Rigidbody* rigidbodyA{};
		Rigidbody* rigidbodyB{};
		Vec3 linearStiffness{};
		Vec3 angularStiffness{};
		Pair<Vec3, Vec3> linearLimit{};
		Pair<Vec3, Vec3> angularLimit{};
	};

public:
	Constraint(StringView name) :
		NamedObject{ name } {}
};

}

