#pragma once

#include "Panel.hpp"

#include <renderer/src/GL/FrameBuffer.hpp>

namespace mm
{

class Viewport : public Panel
{
public:
	Viewport();
	void OnUIRender() override;
	~Viewport() { s_count--; }

private:
	static inline uint32_t s_count = 0;
	std::shared_ptr<FrameBuffer> m_framebuffer;
};

}

