#pragma once

#include "PropertiesPanel.hpp"
#include "SceneHierarchyPanel.hpp"

#include "Core/Utility/Singleton.hpp"

namespace mm
{

class Editor : public Singleton<Editor> 
{
	friend class Singleton<Editor>;
public:
	virtual void Startup() override;
	virtual void Shutdown() override;

	void OnUpdate(float deltaTime);
	void OnUIRender();

private:
	PropertiesPanel m_propertiesPanel;
	SceneHierarchyPanel m_sceneHierarchyPanel;
};

}

