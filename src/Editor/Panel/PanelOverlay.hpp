#pragma once

#include "Core/NamedObject.hpp"

namespace mm
{

class Panel;
class PanelOverlay 
{
public:
	virtual ~PanelOverlay() = default;
	PanelOverlay(Panel& parent) :
		m_parent{ parent } {}

	virtual void OnUpdate(f32) = 0;
	virtual void OnRender() = 0;

	Panel* GetParent() { return &m_parent; }

protected:
	Panel& m_parent;
};

}

