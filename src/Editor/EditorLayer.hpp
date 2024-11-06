#pragma once

#include "Core/Layer/ImGuiLayer.hpp"
#include "Panel/Panel.hpp"

/* BEGIN TEST INCLUDES */
/* END TEST INCLUDES */

namespace mm
{

class EditorLayer : public ImGuiLayer
{
public:
	EditorLayer();
	~EditorLayer();

	EditorLayer(const Window& window);
	void OnUpdate(f32 deltaTime) override;
	void OnRender() override;

private:
	DynArray<Scoped<Panel>> m_panels;
	/* BEGIN TEST VARIABLES */
	/* END TEST VARIABLES */
};

}

