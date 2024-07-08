#pragma once

#include "ViewportPanel.hpp"

namespace mm
{

class CameraController
{
public:
	CameraController(ViewportPanel& vp) : m_viewport(vp) {}
	void OnUIRender();

private:
	ViewportPanel& m_viewport;
};

}

