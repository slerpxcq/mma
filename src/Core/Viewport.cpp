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

Mat4 Viewport::GetMatrix() const
{
	Mat4 view = glm::inverse(m_camera->GetNode()->GetWorldMatrix());
	Mat4 proj = glm::perspective(m_camera->GetFOV(),
								 m_frameBuffer->GetAspect(),
								 m_camera->GetNear(), m_camera->GetFar());
	return proj * view;
}

}
