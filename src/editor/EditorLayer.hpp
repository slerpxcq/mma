#pragma once

#include "Core/Layer/ImGuiLayer.hpp"
#include "Panel/Panel.hpp"

namespace mm
{

class EditorLayer : public ImGuiLayer
{
public:
	EditorLayer(const Window& window) : ImGuiLayer{ window } {}
	void OnUpdate(float deltaTime) override;
	void OnRender() override;

private:
	DynArray<Scoped<Panel>> m_panels;
};

}

