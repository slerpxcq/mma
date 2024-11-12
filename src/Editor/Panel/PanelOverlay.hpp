#pragma once

#include "Core/NamedObject.hpp"

namespace mm
{
class Panel;
class PanelOverlay : public NamedObject
{
public:
	PanelOverlay(Panel& parent, StringView name) :
		NamedObject{ name },
		m_parent{ parent } {}
	virtual ~PanelOverlay() = default;
	virtual void OnUpdate(f32) = 0;
	virtual void OnRender() = 0;

protected:
	Panel& m_parent;
};

}

