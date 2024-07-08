#include "EditorPch.hpp"
#include "CameraController.hpp"

namespace mm
{

void CameraController::OnUIRender()
{
	if (auto cam = m_viewport.m_camera.lock(); cam) {
		/* Process input and set camera transform */
	}
}

}
