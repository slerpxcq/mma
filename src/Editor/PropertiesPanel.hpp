#pragma once

#include "EditorEvent.hpp"

#include "Core/App/EventBus.hpp"
#include "Core/Nodes/Node.hpp"

namespace mm
{

class PropertiesPanel
{
public:
	PropertiesPanel() :
		m_listener(EventBus::GetPtr()) {
		m_listener.listen<EditorEvent::NodeSelected>(MM_EVENT_FN(PropertiesPanel::OnNodeSelected));
	}

	void OnUIRender();

private:
	void OnNodeSelected(const EditorEvent::NodeSelected& e) { m_node = e.node; }

private:
	Node* m_node = nullptr;

	dexode::EventBus::Listener m_listener;
};

}

