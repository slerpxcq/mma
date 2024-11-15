#include "CorePch.hpp"
#include "Viewport.hpp"

#include "Node.hpp"

/* BEGIN TEST INCLUDE */
#include "SceneManager.hpp"
/* END TEST INCLUDE */

namespace mm
{

Viewport::Viewport(Camera* camera, Ref<FrameBuffer> fb) :
	m_camera{ camera }, m_frameBuffer{ fb }
{
	MM_CORE_ASSERT(camera);
	MM_CORE_ASSERT(fb);
}

Mat4 Viewport::GetViewProjectionMatrix() const
{
	return GetProjectionMatrix() * GetViewMatrix();
}

Mat4 Viewport::GetViewMatrix() const
{
	return glm::inverse(m_camera->GetNode()->GetWorldMatrix());
}

Mat4 Viewport::GetProjectionMatrix() const
{
	return glm::perspective(m_camera->GetFOV(),
							m_frameBuffer->GetAspect(),
							m_camera->GetNear(), m_camera->GetFar());
}

}
