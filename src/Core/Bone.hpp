#pragma once

#include "Common/Math/Transform.hpp"
#include "InverseKinematics/InverseKinematicsInfo.hpp"
#include "SceneObject.hpp"

namespace mm
{

class Bone : public SceneObject
{
public:
	struct AssignmentInfo {
		enum Flags : u8 {
			TRANSLATION_BIT = 1<<0,
			ROTATION_BIT = 1<<1,
		};

		Bone* target{};
		f32 ratio{};
		u8 type{};
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

public:
	Bone(ConstructInfo info) : 
		SceneObject{ info.name },
		m_bindWorld{ info.bindWorld },
		m_bindLocal{ info.bindWorld },
		m_flags{ info.flags },
		m_transformLayer{ info.transformLayer } {}

	i32 GetIndex() const { return m_index; }
	void SetParent(Bone* parent); 
	Bone* GetParent() const { return m_parent; }
	u32 GetTransformLayer() const { return m_transformLayer; }
	u32 GetFlags() const { return m_flags; }
	Transform GetBindLocal() const { return m_bindLocal; }
	Transform GetBindWorld() const { return m_bindWorld; }
	Transform GetBindWorldInverse() const { return m_bindWorld.Inverse(); }
	void SetAnimLocal(const Transform& transform);
	Transform GetAnimLocal();
	void SetPoseLocal(const Transform& transform) { m_poseLocal = transform; }
	Transform GetPoseLocal() const { return m_poseLocal; }
	const auto& GetAssignmentInfo() const { return m_assignmentInfo; }
	void SetAssignmentInfo(const AssignmentInfo& info) { m_assignmentInfo = info; }
	const auto& GetInverseKinematicsInfo() const { return m_inverseKinematicsInfo; }
	void SetInverseKinematicsInfo(const InverseKinematicsInfo& info) { m_inverseKinematicsInfo = info; }
	void SetTipInfoBone(Bone* bone) { m_tipInfo.bone = bone; }
	void SetTipInfoOffset(const Vec3& offset) { m_tipInfo.offset = offset; }
	Bone* GetTipInfoBone() const { return m_tipInfo.bone; }
	Vec3 GetTipInfoOffset() const { return m_tipInfo.offset; }
	void SetLocalAxes(Vec3 x, Vec3 z);
	Opt<Mat3> GetLocalAxes() const { return m_localAxes; }
	void SetFixedAxis(Vec3 axis) { m_fixedAxis = axis; }
	Opt<Vec3> GetFixedAxis() const { return m_fixedAxis; }

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

	Opt<Mat3> m_localAxes{};
	Opt<Vec3> m_fixedAxis{};
	Opt<AssignmentInfo> m_assignmentInfo;
	Opt<InverseKinematicsInfo> m_inverseKinematicsInfo;
};

}

