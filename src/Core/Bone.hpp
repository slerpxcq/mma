#pragma once

#include "InverseKinematics/InverseKinematicsInfo.hpp"
#include "SceneObject.hpp"

namespace mm
{

class Rigidbody;
class Bone : public SceneObject
{
	friend class Builder;
public:
	struct AssignmentInfo {
		Bone* target{};
		f32 ratio{};
		Transform::Type type{};
	};

	struct ConstructInfo {
		StringView name;
		i32 index;
		u32 transformLayer;
		u32 flags;
		Transform bindWorld;
	};

	enum Flags : u32 {
		CONNECTED_BIT = (1U << 0),
		ROTATABLE_BIT = (1U << 1),
		MOVEABLE_BIT = (1U << 2),
		VISIBLE_BIT = (1U << 3),
		OPERABLE_BIT = (1U << 4),
		AFTER_PHYSICS_BIT = (1U << 12),
		EXTERNAL_PARENT_BIT = (1U << 13),
	};

	union TipInfo {
		Bone* bone;
		Vec3 offset;
	};

	class Builder;
public:
	Bone(const ConstructInfo& info) : 
		SceneObject{ info.name },
		m_bindWorld{ info.bindWorld },
		m_bindLocal{ info.bindWorld },
		m_index{ info.index },
		m_flags{ info.flags },
		m_transformLayer{ info.transformLayer } {}

	i32 GetIndex() const { return m_index; }
	Bone* GetParent() const { return m_parent; }
	u32 GetTransformLayer() const { return m_transformLayer; }
	u32 GetFlags() const { return m_flags; }

	bool IsConnected() const { return m_flags & Flags::CONNECTED_BIT; }
	bool IsRotatable() const { return m_flags & Flags::ROTATABLE_BIT; }
	bool IsTranslatable() const { return m_flags & Flags::MOVEABLE_BIT; }
	bool IsVisible() const { return m_flags & Flags::VISIBLE_BIT; }
	bool IsOperable() const { return m_flags & Flags::OPERABLE_BIT; }

	Transform GetParentWorld();
	Transform GetBindLocal() const { return m_bindLocal; }
	Transform GetBindWorld() const { return m_bindWorld; }
	Transform GetBindWorldInverse() const { return m_bindWorld.Inverse(); }
	Transform GetAnimLocal();
	Transform GetPoseLocal() const { return m_poseLocal; }

	void SetAnimLocal(const Transform& transform);
	void SetPoseLocal(const Transform& transform) { m_poseLocal = transform; }

	const auto& GetAssignmentInfo() const { return m_assignmentInfo; }
	const auto& GetInverseKinematicsInfo() const { return m_inverseKinematicsInfo; }
	Vec3 GetTipWorldPos() const;

	Opt<Mat3> GetLocalAxes() const { return m_localAxes; }
	Opt<Vec3> GetFixedAxis() const { return m_fixedAxis; }

	Rigidbody* GetRigidbody() const { return m_rigidbody; }
	void PullRigidbodyTransform(Transform::Type type = Transform::Type::ALL);
	void PushRigidbodyTransform(Transform::Type type = Transform::Type::ALL);

private:
	Bone* m_parent{};
	i32 m_index{};
	u32 m_transformLayer{};
	u32 m_flags{};
	TipInfo m_tipInfo{};

	Transform m_bindLocal{};
	Transform m_bindWorld{};
	Transform m_animLocal{};
	Transform m_poseLocal{};

	Rigidbody* m_rigidbody{};
	Opt<Mat3> m_localAxes{};
	Opt<Vec3> m_fixedAxis{};
	Opt<AssignmentInfo> m_assignmentInfo;
	Opt<InverseKinematicsInfo> m_inverseKinematicsInfo;
};

class Bone::Builder {
public:
	Builder(Bone* bone) : m_bone{ bone } {}
	void SetAssignmentInfo(const AssignmentInfo& info) { m_bone->m_assignmentInfo = info; }
	void SetInverseKinematicsInfo(const InverseKinematicsInfo& info) { m_bone->m_inverseKinematicsInfo = info; }
	void SetTipInfoBone(Bone* bone) { m_bone->m_tipInfo.bone = bone; }
	void SetTipInfoOffset(const Vec3& offset) { m_bone->m_tipInfo.offset = offset; }
	void SetLocalAxes(Vec3 x, Vec3 z);
	void SetFixedAxis(Vec3 axis) { m_bone->m_fixedAxis = axis; }
	void SetRigidbody(Rigidbody* rigidbody) { m_bone->m_rigidbody = rigidbody; }
	void SetParent(Bone* parent); 
	Bone* Get() { return m_bone; }

private:
	Bone* m_bone;
};

}

