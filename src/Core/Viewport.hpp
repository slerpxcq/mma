#pragma once

#include "Camera.hpp"
#include "Graphics/FrameBuffer.hpp"

namespace mm
{

class Viewport
{
public:
	Viewport(Camera* camera, Ref<FrameBuffer> fb);
	Mat4 GetViewProjectionMatrix() const;
	Mat4 GetViewMatrix() const;
	Mat4 GetProjectionMatrix() const;
	FrameBuffer& GetFrameBuffer() { return *m_frameBuffer; }
	Camera& GetCamera() { return *m_camera; }

private:
	Camera* m_camera{};
	Ref<FrameBuffer> m_frameBuffer{};
};

}

