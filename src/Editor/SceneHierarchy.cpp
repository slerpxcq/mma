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
		ImGui::TreeNodeEx("Camera", LEAF_FLAG);
		if (ImGui::IsItemClicked()) 
			EventBus::Instance()->postpone<EditorEvent::EntitySelected>({ &m_world->GetCamera() });
	}

	void SceneHierarchy::PhysicsWorldNode()
	{
		ImGui::TreeNodeEx("Physics World", LEAF_FLAG);
		if (ImGui::IsItemClicked()) 
			EventBus::Instance()->postpone<EditorEvent::EntitySelected>({ &m_world->GetPhysicsWorld() });
	}

	void SceneHierarchy::LightsNode()
	{
		if (ImGui::TreeNodeEx("Lights", NONLEAF_FLAG)) {
			auto& light = m_world->GetLight();
			ImGui::TreeNodeEx(light.GetName().c_str(), LEAF_FLAG);
			if (ImGui::IsItemClicked()) 
				EventBus::Instance()->postpone<EditorEvent::EntitySelected>({ &light });
			ImGui::TreePop();
		}
	}

	void SceneHierarchy::SkinNode(Model& model)
	{
		if (ImGui::TreeNodeEx("Skin", NONLEAF_FLAG)) {
			if (ImGui::IsItemClicked()) 
				EventBus::Instance()->postpone<EditorEvent::EntitySelected>({ &model.GetSkin() });

			auto& meshes = model.GetSkin().GetMeshes();
			for (auto& mesh : meshes) {
				ImGui::TreeNodeEx(mesh.name.c_str(), LEAF_FLAG);
				if (ImGui::IsItemClicked()) 
					EventBus::Instance()->postpone<EditorEvent::EntitySelected>({ &mesh });
			}
			ImGui::TreePop();
		}
	}

	void SceneHierarchy::ArmatureNode(Model& model)
	{
		if (ImGui::TreeNodeEx("Armature", NONLEAF_FLAG)) {
			auto& pmx = model.GetPMXFile();
			auto& armature = model.GetArmature();
			auto& bones = armature.GetBones();

			if (ImGui::IsItemClicked()) 
				EventBus::Instance()->postpone<EditorEvent::EntitySelected>({ &armature });

			for (uint32_t i = 0; i < bones.size(); ++i) {
				ImGui::TreeNodeEx(pmx.GetBoneName(i).c_str(), LEAF_FLAG);
				if (ImGui::IsItemClicked()) 
					EventBus::Instance()->postpone<EditorEvent::EntitySelected>({ &bones[i] });
			}

			ImGui::TreePop();
		}
	}

	void SceneHierarchy::MorphNode(Model& model)
	{
		ImGui::TreeNodeEx("Morph", LEAF_FLAG);
		if (ImGui::IsItemClicked()) 
			EventBus::Instance()->postpone<EditorEvent::EntitySelected>({ &model.GetMorph() });
	}

	void SceneHierarchy::AnimationNode(Model& model)
	{
		ImGui::TreeNodeEx("Animation", LEAF_FLAG);
		if (ImGui::IsItemClicked()) 
			EventBus::Instance()->postpone<EditorEvent::EntitySelected>({ &model.GetAnim() });
	}

	void SceneHierarchy::ModelsNode()
	{
		if (ImGui::TreeNodeEx("Models", NONLEAF_FLAG)) {
			for (auto& model : m_world->GetModels()) {
				auto& pmx = model->GetPMXFile();
				const std::string& name = pmx.GetName();
				if (ImGui::TreeNodeEx(name.c_str(), NONLEAF_FLAG)) {
					if (ImGui::IsItemClicked())
						EventBus::Instance()->postpone<EditorEvent::EntitySelected>({ model.get() });
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

		if (ImGui::TreeNodeEx("World", NONLEAF_FLAG)) {
			if (ImGui::IsItemClicked()) 
				EventBus::Instance()->postpone<EditorEvent::EntitySelected>({ m_world });

			CameraNode();
			PhysicsWorldNode();
			LightsNode();
			ModelsNode();
			ImGui::TreePop();
		}

		ImGui::End();
	}
}
