#include "mmpch.hpp"
#include "PropertiesPanel.hpp"

namespace mm
{

void PropertiesPanel::OnUIRender()
{
	ImGui::Begin("Properties");

	if (m_node) {
		ImGui::SeparatorText(m_node->m_name.c_str());
		m_node->ShowProperties();
	}

	ImGui::End();
}

}
