#pragma once

#include "Panel.hpp"
#include "Core/Graphics/FrameBuffer.hpp"

namespace mm
{

class ViewportPanel : public Panel
{
public:
	ViewportPanel(const String& name) : Panel{ name } {}
	virtual void OnUpdate(f32 deltaTime) override; 
	virtual void OnRender() override;
	void SetViewport(Viewport* vp) { m_viewport = vp; }
	void UpdateCamera() const;

private:
	Viewport* m_viewport{};
};

}

