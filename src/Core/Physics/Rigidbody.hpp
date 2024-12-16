#pragma once

#include "Collider.hpp"
#include "../SceneObject.hpp"
#include "PhysicsObject.hpp"

namespace mm
{

class Bone;
class Rigidbody : public SceneObject, 
	              public PhysicsObject
{
public:
	enum class Type {
		KINEMATIC,
		DYNAMIC,
		DYNAMIC_FOLLOW
	};

	struct ConstructInfo {
		StringView name{};
		Bone* bone{};
		Collider collider{};
		Transform bindWorld{};
		f32 mass{};
		f32 linearDamping{};
		f32 angularDamping{};
		f32 friction{};
		f32 restitution{};
		u16 group{};
		u16 noCollisionGroupMask{};
		Type type{};
	};

public:
	virtual ~Rigidbody() = default;
	Rigidbody(StringView name) :
		SceneObject{ name } {}

	Transform GetBindWorld() const { return m_bindWorld; }
	void SetBindWorld(const Transform& transform) { m_bindWorld = transform; }
	Type GetType() const { return m_type; }
	void SetType(Type type) { m_type = type; }
	Collider GetCollider() const { return m_collider; }
	void SetCollider(Collider c) { m_collider = c; }
	void SetBone(Bone* bone) { m_bone = bone; }
	void PullBoneTransform(Transform::Type type = Transform::ALL);
	void PushBoneTransform(Transform::Type type = Transform::ALL);

private:
	Bone* m_bone{};
	Collider m_collider;
	Transform m_bindWorld{};
	Type m_type{};
};

}

