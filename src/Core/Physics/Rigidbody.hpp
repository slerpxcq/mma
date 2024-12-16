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
	class Builder;
	friend class Rigidbody::Builder;
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
	Type GetType() const { return m_type; }
	Collider GetCollider() const { return m_collider; }
	void PullBoneTransform(Transform::Type type = Transform::ALL);
	void PushBoneTransform(Transform::Type type = Transform::ALL);

private:
	Bone* m_bone{};
	Collider m_collider;
	Transform m_bindWorld{};
	Type m_type{};
};

class Rigidbody::Builder
{
public:
	Builder(const Rigidbody::ConstructInfo& info);
	void SetHandle(void* handle) { m_rigidbody->SetHandle(handle); }
	void SetBindWorld(const Transform& transform) { m_rigidbody->m_bindWorld = transform; }
	void SetType(Type type) { m_rigidbody->m_type = type; }
	void SetCollider(Collider c) { m_rigidbody->m_collider = c; }
	void SetBone(Bone* bone) { m_rigidbody->m_bone = bone; }
	Rigidbody* Get() { return m_rigidbody; }

private:
	Rigidbody* m_rigidbody;
};

}

