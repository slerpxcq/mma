#pragma once

#include "Panel.hpp"

#include <renderer/src/GL/FrameBuffer.hpp>

namespace mm
{

class ViewportPanel : public Panel
{
public:
	ViewportPanel();
	void OnUIRender() override;
	~ViewportPanel() { s_count--; }

private:
	static inline uint32_t s_count = 0;
	std::shared_ptr<FrameBuffer> m_framebuffer;
};

}

