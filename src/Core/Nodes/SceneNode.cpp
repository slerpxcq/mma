#include "mmpch.hpp"
#include "SceneNode.hpp"

#include "Core/App/FileDialog.hpp"
#include "Core/Manager/FileManager.hpp"

#include "ModelNode.hpp"

namespace mm
{

void SceneNode::MakeModelNode()
{

}

void SceneNode::ShowProperties()
{
	if (ImGui::Button("Load model")) {
		auto path = FileDialog::Open("pmx");
		ModelNode::Make(*FileManager::Get().Load<PMXFile>(path));
	}
}

}
