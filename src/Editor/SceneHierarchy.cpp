#include "mmpch.hpp"
#include "SceneHierarchy.hpp"

#include "Core/MM/World.hpp"

namespace mm
{
	void SceneHierarchy::OnUIRender()
	{
		ImGui::ShowDemoWindow();
		ImGui::Begin("Scene heirarchy");

		uint32_t leafFlag = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		if (ImGui::TreeNodeEx("World")) {
			if (ImGui::TreeNodeEx("Camera")) {
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("Lights")) {
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("Models")) {
				for (auto& model : m_world->GetModels()) {
					auto& pmx = model->GetPMXFile();
					const std::string& name = pmx.GetName();
					if (ImGui::TreeNodeEx(name.c_str())) {
						if (ImGui::TreeNodeEx("Skin")) {
							for (auto& mesh : model->GetSkin().GetMeshes()) {
								ImGui::TreeNodeEx(mesh.name.c_str(), leafFlag);
								if (ImGui::IsItemClicked()) {
									MM_INFO("Item clicked");
								}
							}
							ImGui::TreePop();
						}
						if (ImGui::TreeNodeEx("Armature")) {
							auto& bones = model->GetArmature().GetBones();
							for (uint32_t i = 0; i < bones.size(); ++i) {
								ImGui::TreeNodeEx(pmx.GetBoneName(i).c_str(), leafFlag);
								if (ImGui::IsItemClicked()) {
									MM_INFO("Item clicked");
								}
							}
							ImGui::TreePop();
						}
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		ImGui::End();
	}
}
