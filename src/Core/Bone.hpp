#pragma once

#include "Common/Math/Transform.hpp"
#include "SceneObject.hpp"

namespace mm
{

class Bone : public SceneObject
{
public:
	Bone(StringView name,
		 i32 parentIndex,
		 Transform bindLocal,
		 Transform bindWorld) : 
		SceneObject{ name },
		m_parentIndex{ parentIndex },
		m_bindLocal{ bindLocal },
		m_bindWorldInv{ bindWorld.Inverse() } {}

	i32 GetParentIndex() const { return m_parentIndex; }
	Transform GetBindLocal() const { return m_bindLocal; }
	Transform GetBindWorld() const { return m_bindWorldInv.Inverse(); }
	Transform GetBindWorldInv() const { return m_bindWorldInv; }

private:
	i32 m_parentIndex{};
	Transform m_bindLocal{};
	Transform m_bindWorldInv{};
};

}

