#pragma once

#include "ImGuiContext.hpp"

#include "Panels/MenuBar.hpp"
#include "Panels/ViewportPanel.hpp"
#include "Panels/SceneHierarchyPanel.hpp" 

#include <dexode/EventBus.hpp>

#include <core/src/Application.hpp>

namespace mm
{

class EditorApplication : public Application
{
public:
	virtual void NewFrame(float deltaTime) override;
	virtual void Startup() override;
	virtual void Shutdown() override;

private:
	void RegisterWindowCallbacks();

private:
	std::unique_ptr<ImGuiContext> m_GUIContext;

	std::shared_ptr<dexode::EventBus> m_eventBus;
	dexode::EventBus m_listener;

	std::unique_ptr<MenuBar> m_menuBar;
	std::unique_ptr<ViewportPanel> m_viewport;
	std::unique_ptr<SceneHierarchyPanel> m_sceneHierarchy;

	std::shared_ptr<SceneNode> m_sceneNode;
};

}

