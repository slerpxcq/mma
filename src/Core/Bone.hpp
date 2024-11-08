#pragma once

#include "Common/Math/Transform.hpp"
#include "SceneObject.hpp"

namespace mm
{

class Bone : public SceneObject
{
public:
	Bone(StringView name,
		 Transform bindLocal,
		 Transform bindWorld) : 
		SceneObject{ name },
		m_bindLocal{ bindLocal },
		m_bindWorldInv{ bindWorld.Inverse() } {}

	Transform GetBindLocal() const { return m_bindLocal; }
	Transform GetBindWorldInv() const { return m_bindWorldInv; }

private:
	Transform m_bindLocal{};
	Transform m_bindWorldInv{};
};

}

