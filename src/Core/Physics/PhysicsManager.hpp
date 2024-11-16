#pragma once

#include "Rigidbody.hpp"
#include "Collider.hpp"
#include "Constraint.hpp"

namespace mm
{

class PhysicsManager
{
public:

public:
	MM_NONMOVABLE(PhysicsManager)
	PhysicsManager() = default;
	virtual ~PhysicsManager() = default;

	virtual Rigidbody* CreateRigidbody(const Rigidbody::ConstructInfo& info) = 0;
	virtual Collider CreateCollider(const Collider::ConstructInfo& info) = 0;
	virtual Constraint CreateConstraint(const Constraint::ConstructInfo& info) = 0;
};

}

