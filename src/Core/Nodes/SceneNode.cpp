#include "mmpch.hpp"
#include "SceneNode.hpp"

#include "Core/App/FileDialog.hpp"
#include "Core/Manager/FileManager.hpp"

#include "ModelNode.hpp"

namespace mm
{

void SceneNode::ShowProperties()
{
	if (ImGui::Button("Load model")) {
		auto path = FileDialog::Open("pmx");
		if (path.string().size() > 0) {
			auto model = ModelNode::Make(*FileManager::Get().Load<PMXFile>(path));
			m_children.push_back(std::move(model));
		}
	}
}

}
