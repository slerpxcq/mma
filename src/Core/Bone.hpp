#pragma once

#include "Common/Math/Transform.hpp"
#include "SceneObject.hpp"
#include "InverseKinematicData.hpp"

namespace mm
{

class Bone : public SceneObject
{
public:
	struct BoneConstructInfo {
		StringView name;
		i32 parentIndex;
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
		IK_BIT = (1U << 5),
		ASSIGN_LOCAL_BIT = (1U << 7),
		ASSIGN_ROTATION_BIT = (1U << 8),
		ASSIGN_MOVE_BIT = (1U << 9),
		FIXED_AXIS_BIT = (1U << 10),
		LOCAL_AXIS_BIT = (1U << 11),
		AFTER_PHYSICS_BIT = (1U << 12),
		EXTERNAL_PARENT_BIT = (1U << 13),
	};
public:
	Bone(BoneConstructInfo info) : 
		SceneObject{ info.name },
		m_parentIndex{ info.parentIndex },
		m_bindLocal{ info.bindLocal },
		m_bindWorldInv{ info.bindWorld.Inverse() },
		m_flags{ info.flags },
		m_transformLayer{ info.transformLayer } {}

	i32 GetParentIndex() const { return m_parentIndex; }
	u32 GetFlags() const { return m_flags; }
	Transform GetBindLocal() const { return m_bindLocal; }
	Transform GetBindWorld() const { return m_bindWorldInv.Inverse(); }
	Transform GetBindWorldInv() const { return m_bindWorldInv; }

private:
	i32 m_parentIndex{};
	u32 m_transformLayer{};
	u32 m_flags{};
	Transform m_bindLocal{};
	Transform m_bindWorldInv{};
};

}

