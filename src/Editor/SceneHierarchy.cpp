#include "mmpch.hpp"
#include "SceneHierarchy.hpp"

#include "Core/MM/World.hpp"

#include "EditorEvent.hpp"
#include "Core/App/EventBus.hpp"

#include "Properties.hpp"

namespace mm
{
	void SceneHierarchy::OnUIRender()
	{
		ImGui::ShowDemoWindow();
		ImGui::Begin("Scene heirarchy");

		uint32_t leafFlag = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		uint32_t nonLeafFlag = ImGuiTreeNodeFlags_OpenOnArrow;

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
							auto& meshes = model->GetSkin().GetMeshes();
							for (uint32_t i=0; i < meshes.size(); ++i) {
								auto& mesh = meshes[i];
								ImGui::TreeNodeEx(mesh.name.c_str(), leafFlag);
								if (ImGui::IsItemClicked()) {
									EditorEvent::ItemSelected e = {};
									e.index = i;
									e.item = (void*)&mesh;
									e.type = Properties::TYPE_MESH;
									EventBus::Instance()->postpone(e);
								}
							}
							ImGui::TreePop();
						}

						if (ImGui::TreeNodeEx("Armature")) {
							auto& armature = model->GetArmature();
							auto& bones = armature.GetBones();
							for (uint32_t i = 0; i < bones.size(); ++i) {
								ImGui::TreeNodeEx(pmx.GetBoneName(i).c_str(), leafFlag);
								if (ImGui::IsItemClicked()) {
									EditorEvent::ItemSelected e = {};
									e.index = i;
									e.item = (void*)&armature.GetPose()[i];
									e.type = Properties::TYPE_BONE;
									EventBus::Instance()->postpone(e);
								}
							}
							ImGui::TreePop();
						}

						ImGui::TreeNodeEx("Morph", leafFlag);
						if (ImGui::IsItemClicked()) {
							EditorEvent::ItemSelected e = {};
							/* Need pass model because extra information needed */
							/* from PMX file */
							e.item = model.get();
							e.type = Properties::TYPE_MORPH;
							EventBus::Instance()->postpone(e);
						}

						ImGui::TreeNodeEx("Animation", leafFlag);
						if (ImGui::IsItemClicked()) {
							EditorEvent::ItemSelected e = {};
							e.item = &model->GetAnim();
							e.type = Properties::TYPE_ANIMATION;
							EventBus::Instance()->postpone(e);
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
