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
	void SetFrameBuffer(FrameBuffer* fb) { m_frameBuffer = fb; }

private:
	FrameBuffer* m_frameBuffer{};
};

}

