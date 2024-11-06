#include "CorePch.hpp"
#include "Camera.hpp"

#include "Node.hpp"
#include "Viewport.hpp"

namespace mm
{

Vec3 Camera::GetViewVector() const
{
	return -Vec3{ m_node->GetWorldMatrix()[2] };
}

Vec3 Camera::GetUpVector() const
{
	return Vec3{ m_node->GetWorldMatrix()[1] };
}

Vec3 Camera::GetRightVector() const
{
	return Vec3{ m_node->GetWorldMatrix()[0] };
}

}
