#pragma once

#include "Panel.hpp"

#include "Core/Viewport.hpp"

namespace mm
{

class ViewportPanel : public Panel
{
public:
	ViewportPanel(const String& name) : 
		Panel{ name } {}

	void OnUpdate(f32 deltaTime) override; 
	void OnRender() override;

	void SetViewport(Viewport* vp) { m_viewport = vp; }
	Viewport* GetViewport() const { return m_viewport; }

	Vec3 ToScreenPos(Vec3 worldPos);

private:
	Viewport* m_viewport{};
};

}

