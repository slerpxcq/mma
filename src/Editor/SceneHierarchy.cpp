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
		ImGui::Begin("Scene heirarchy");

		uint32_t leafFlag = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		uint32_t nonLeafFlag = ImGuiTreeNodeFlags_OpenOnArrow;

		if (ImGui::TreeNodeEx("World", nonLeafFlag)) {
			if (ImGui::IsItemClicked()) {
				EditorEvent::ItemSelected e = {};
				e.item = m_world;
				e.type = Properties::TYPE_WORLD;
				EventBus::Instance()->postpone(e);
			}
			ImGui::TreeNodeEx("Camera", leafFlag);
			if (ImGui::IsItemClicked()) {
			}
			ImGui::TreeNodeEx("Physics World", leafFlag);
			if (ImGui::IsItemClicked()) {
				EditorEvent::ItemSelected e = {};
				e.item = &m_world->GetPhysicsWorld();
				e.type = Properties::TYPE_PHYSICS_WORLD;
				EventBus::Instance()->postpone(e);
			}
			if (ImGui::TreeNodeEx("Lights", nonLeafFlag)) {
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("Models", nonLeafFlag)) {
				for (auto& model : m_world->GetModels()) {
					auto& pmx = model->GetPMXFile();
					const std::string& name = pmx.GetName();
					if (ImGui::TreeNodeEx(name.c_str(), nonLeafFlag)) {
						if (ImGui::TreeNodeEx("Skin", nonLeafFlag)) {
							if (ImGui::IsItemClicked()) {

							}
							auto& meshes = model->GetSkin().GetMeshes();
							for (uint32_t i=0; i < meshes.size(); ++i) {
								auto& mesh = meshes[i];
								ImGui::TreeNodeEx(mesh.name.c_str(), leafFlag);
								if (ImGui::IsItemClicked()) {
									EditorEvent::ItemSelected e = {};
									e.index = i;
									e.item = &mesh;
									e.type = Properties::TYPE_MESH;
									EventBus::Instance()->postpone(e);
								}
							}
							ImGui::TreePop();
						}

						if (ImGui::TreeNodeEx("Armature", nonLeafFlag)) {
							auto& armature = model->GetArmature();
							auto& bones = armature.GetBones();
							//if (ImGui::IsItemClicked()) {
							//	EditorEvent::ItemSelected e = {};
							//	e.item = &armature;
							//	e.type = Properties::TYPE_ARMATURE;
							//	EventBus::Instance()->postpone(e);
							//}
							/* Cluster */
							auto& clusters = pmx.GetClusters();
							for (auto& cluster : clusters) {
								if (ImGui::TreeNodeEx(cluster.nameJP.c_str(), nonLeafFlag)) {
									for (auto& elem : cluster.elements) {
										if (elem.type == PMXFile::CLUSTER_BONE) {
											const std::string& name = pmx.GetBoneName(elem.index);
											ImGui::TreeNodeEx(name.c_str(), leafFlag);
											if (ImGui::IsItemClicked()) {
												EditorEvent::ItemSelected e = {};
												e.index = elem.index;
												e.item = &armature.GetPose()[elem.index];
												e.type = Properties::TYPE_BONE;
												EventBus::Instance()->postpone(e);
											}
										}
									}
									ImGui::TreePop();
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
							e.item = model.get();
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
