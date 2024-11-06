#pragma once

#include "Camera.hpp"
#include "Graphics/FrameBuffer.hpp"

namespace mm
{

class Viewport
{
public:
	Viewport(Ref<Camera> camera, Ref<FrameBuffer> fb);
	Mat4 GetMatrix() const;
	FrameBuffer& GetFrameBuffer() { return *m_frameBuffer; }
	Camera& GetCamera() { return *m_camera; }

private:
	u32 m_width{};
	u32 m_height{};
	Ref<Camera> m_camera{};
	Ref<FrameBuffer> m_frameBuffer{};
};

}

