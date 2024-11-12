#pragma once

#include "Panel.hpp"
#include "Core/Graphics/FrameBuffer.hpp"

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

private:
	//void UpdateCamera() const;

private:
	Viewport* m_viewport{};
};

}

