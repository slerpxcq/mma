#pragma once

#include "Collider.hpp"
#include "SceneObject.hpp"
#include "PhysicsObject.hpp"

namespace mm
{

class Rigidbody : public SceneObject, 
	              public PhysicsObject
{
public:
	struct ConstructInfo {
		StringView name{};
		Collider collider{};
		Transform bindWorld{};
		f32 mass{};
		f32 linearDamping{};
		f32 angularDamping{};
		f32 friction{};
		f32 restitution{};
		u16 group;
		u16 noCollisionGroupMask;
		bool isDynamic{};
	};

public:
	virtual ~Rigidbody() = default;
	Rigidbody(StringView name) :
		SceneObject{ name } {}

	Transform GetBindWorld() const { return m_bindWorld; }
	void SetBindWorld(const Transform& transform) { m_bindWorld = transform; }

private:
	Transform m_bindWorld{};
};

}

