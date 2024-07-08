#pragma once

#include "Panel.hpp"

#include <renderer/src/GL/FrameBuffer.hpp>

namespace mm
{

class ViewportPanel : public Panel
{
	friend class CameraController;
public:
	ViewportPanel();
	void OnUIRender() override;
	~ViewportPanel() { s_count--; }

	void SetCamera(std::shared_ptr<CameraNode> cam) { m_camera = cam; }

private:
	static inline uint32_t s_count = 0;
	std::shared_ptr<FrameBuffer> m_framebuffer;
	std::weak_ptr<CameraNode> m_camera;
};

}

