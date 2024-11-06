#pragma once

#undef near
#undef far

#include "SceneObject.hpp"

namespace mm
{

class Viewport;

class Camera : public SceneObject
{
public:
	Camera(StringView name,
		   f32 fov = 45.f, f32 near = .1f, f32 far = 100.f) :
		SceneObject{ name },
		m_fov{ fov }, m_near{ near }, m_far{ far } {}
	
	f32 GetFOV() const { return m_fov; }
	f32 GetNear() const { return m_near; }
	f32 GetFar() const { return m_far; }
	Vec3 GetViewVector() const;
	Vec3 GetUpVector() const;
	Vec3 GetRightVector() const;

private:
	f32 m_fov{};
	f32 m_near{};
	f32 m_far{};
};

}

