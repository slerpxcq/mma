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
		m_bindWorld{ bindWorld } {}

private:
	Transform m_bindLocal{};
	Transform m_bindWorld{};
};

}

