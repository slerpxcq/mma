#pragma once

#include "Common/Math/Transform.hpp"
#include "InverseKinematics/InverseKinematicsInfo.hpp"
#include "SceneObject.hpp"

namespace mm
{

class Bone : public SceneObject
{
public:
	struct AssignmentInfo
	{
		enum Type : u8 {
			TRANSLATION = 1<<0,
			ROTATION = 1<<1,
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
		Transform bindLocal;
		Transform bindWorld;
	};

	enum Flags : u32 {
		CONNECTED_BIT = (1U << 0),
		ROTATABLE_BIT = (1U << 1),
		MOVEABLE_BIT = (1U << 2),
		VISIBLE_BIT = (1U << 3),
		OPERABLE_BIT = (1U << 4),
		FIXED_AXIS_BIT = (1U << 10),
		LOCAL_AXIS_BIT = (1U << 11),
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
		m_bindLocal{ info.bindLocal },
		m_bindWorld{ info.bindWorld },
		m_flags{ info.flags },
		m_transformLayer{ info.transformLayer } {}

	i32 GetIndex() const { return m_index; }
	void SetParent(Bone* parent) { m_parent = parent; }
	Bone* GetParent() const { return m_parent; }
	void SetTipBone(Bone* bone) { m_tipInfo.bone = bone; }
	void SetTipOffset(const Vec3& offset) { m_tipInfo.offset = offset; }
	TipInfo GetTipInfo() const { return m_tipInfo; }
	u32 GetTransformLayer() const { return m_transformLayer; }
	u32 GetFlags() const { return m_flags; }
	Transform GetBindLocal() const { return m_bindLocal; }
	Transform GetBindWorld() const { return m_bindWorld; }
	Transform GetBindWorldInverse() const { return m_bindWorld.Inverse(); }
	void SetAnimLocal(const Transform& transform);
	Transform GetAnimLocal() const { return m_animLocal; }
	const auto& GetAssignmentInfo() const { return m_assignmentInfo; }
	void SetAssignmentInfo(const AssignmentInfo& info) { m_assignmentInfo = info; }
	const auto& GetInverseKinematicsInfo() const { return m_inverseKinematicsInfo; }
	void SetInverseKinematicsInfo(const InverseKinematicsInfo& info) { m_inverseKinematicsInfo = info; }

private:
	i32 m_index{};
	Bone* m_parent{};
	TipInfo m_tipInfo{};
	u32 m_transformLayer{};
	u32 m_flags{};
	Transform m_bindLocal{};
	Transform m_bindWorld{};
	Transform m_animLocal{};
	Opt<AssignmentInfo> m_assignmentInfo;
	Opt<InverseKinematicsInfo> m_inverseKinematicsInfo;
};

}

