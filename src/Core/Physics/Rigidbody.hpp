#pragma once

#include "Collider.hpp"
#include "../SceneObject.hpp"
#include "PhysicsObject.hpp"

namespace mm
{

class Rigidbody : public SceneObject, 
	              public PhysicsObject
{
public:
	enum Flags : u8 {
		KINEMATIC_BIT = 1 << 0,
		DYNAMIC_BIT = 1 << 1
	};
	struct ConstructInfo {
		StringView name{};
		Collider collider{};
		Transform bindWorld{};
		f32 mass{};
		f32 linearDamping{};
		f32 angularDamping{};
		f32 friction{};
		f32 restitution{};
		u16 group{};
		u16 noCollisionGroupMask{};
		u8 flags{};
	};

public:
	virtual ~Rigidbody() = default;
	Rigidbody(StringView name) :
		SceneObject{ name } {}

	Transform GetBindWorld() const { return m_bindWorld; }
	void SetFlags(u8 flags) { m_flags = flags; }
	void SetBindWorld(const Transform& transform) { m_bindWorld = transform; }
	bool IsDynamic() const { return m_flags & DYNAMIC_BIT; }
	bool IsKinematic() const { return m_flags & KINEMATIC_BIT; }

private:
	Transform m_bindWorld{};
	u8 m_flags{};
};

}

