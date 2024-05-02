#include "mmpch.hpp"
#include "SceneHierarchy.hpp"

#include "Core/MM/World/World.hpp"

#include "EditorEvent.hpp"
#include "Core/App/EventBus.hpp"

#include "Properties.hpp"

namespace mm
{
	void SceneHierarchy::CameraNode()
	{
		ImGui::TreeNodeEx("Camera", leafFlag);
		if (ImGui::IsItemClicked()) {
		}
	}

	void SceneHierarchy::PhysicsWorldNode()
	{
		ImGui::TreeNodeEx("Physics World", leafFlag);
		if (ImGui::IsItemClicked()) {
			EditorEvent::ItemSelected e = {};
			e.item = &m_world->GetPhysicsWorld();
			e.type = Properties::TYPE_PHYSICS_WORLD;
			EventBus::Instance()->postpone(e);
		}
	}

	void SceneHierarchy::LightsNode()
	{
		if (ImGui::TreeNodeEx("Lights", nonLeafFlag)) {
			auto& light = m_world->GetLight();
			ImGui::TreeNodeEx(light.GetName().c_str(), leafFlag);
			if (ImGui::IsItemClicked()) {
				EditorEvent::ItemSelected e = {};
				e.item = &light;
				e.type = Properties::TYPE_LIGHT;
				EventBus::Instance()->postpone(e);
			}
			//for (auto& light : m_world->GetLights()) {
			//	if (ImGui::TreeNodeEx(light->GetName().c_str(), leafFlag)) {
			//		EditorEvent::ItemSelected e = {};
			//		e.item = light.get();
			//		e.type = Properties::TYPE_LIGHT;
			//		EventBus::Instance()->postpone(e);
			//	}
			//}
			ImGui::TreePop();
		}
	}

	void SceneHierarchy::SkinNode(Model& model)
	{
		if (ImGui::TreeNodeEx("Skin", nonLeafFlag)) {
			if (ImGui::IsItemClicked()) {
			}
			auto& meshes = model.GetSkin().GetMeshes();
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
	}

	void SceneHierarchy::ArmatureNode(Model& model)
	{
		if (ImGui::TreeNodeEx("Armature", nonLeafFlag)) {
			auto& pmx = model.GetPMXFile();
			auto& armature = model.GetArmature();
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
	}

	void SceneHierarchy::MorphNode(Model& model)
	{
		ImGui::TreeNodeEx("Morph", leafFlag);
		if (ImGui::IsItemClicked()) {
			EditorEvent::ItemSelected e = {};
			/* Need pass model because extra information needed */
			/* from PMX file */
			e.item = &model;
			e.type = Properties::TYPE_MORPH;
			EventBus::Instance()->postpone(e);
		}
	}

	void SceneHierarchy::AnimationNode(Model& model)
	{
		ImGui::TreeNodeEx("Animation", leafFlag);
		if (ImGui::IsItemClicked()) {
			EditorEvent::ItemSelected e = {};
			e.item = &model;
			e.type = Properties::TYPE_ANIMATION;
			EventBus::Instance()->postpone(e);
		}
	}

	void SceneHierarchy::ModelsNode()
	{
		if (ImGui::TreeNodeEx("Models", nonLeafFlag)) {
			for (auto& model : m_world->GetModels()) {
				auto& pmx = model->GetPMXFile();
				const std::string& name = pmx.GetName();
				if (ImGui::TreeNodeEx(name.c_str(), nonLeafFlag)) {
					SkinNode(*model);
					ArmatureNode(*model);
					MorphNode(*model);
					AnimationNode(*model);
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}

	void SceneHierarchy::OnUIRender()
	{
		ImGui::Begin("Scene heirarchy");

		if (ImGui::TreeNodeEx("World", nonLeafFlag)) {
			if (ImGui::IsItemClicked()) {
				EditorEvent::ItemSelected e = {};
				e.item = m_world;
				e.type = Properties::TYPE_WORLD;
				EventBus::Instance()->postpone(e);
			}
			CameraNode();
			PhysicsWorldNode();
			LightsNode();
			ModelsNode();
			ImGui::TreePop();
		}

		ImGui::End();
	}
}
