#include "mmpch.hpp"
#include "Properties.hpp"

#include "Core/MM/Model/Skin.hpp"
#include "Core/MM/Model/Armature.hpp"
#include "Core/MM/Physics/PhysicsWorld.hpp"
#include "Core/MM/Files/VMDFile.hpp"
#include "Core/MM/Lights/DirectionalLight.hpp"

#include "EditorLayer.hpp"
#include "Sequencer.hpp"

#include <nfd.h>

namespace mm
{
	template <typename F, typename T, typename... Args>
	bool WithUndo(T& undoValue, F&& func, const char* label, T& ref, Args&&... args) {
		T valueBeforeEdit = ref;
		bool ret = func(label, (float*)&ref, std::forward<Args>(args)...);
		if (ImGui::IsItemActivated()) {
			undoValue = valueBeforeEdit;
		}
		if (ImGui::IsItemDeactivatedAfterEdit()) {
			T redoValue = ref;
			EventBus::Instance()->postpone<EditorEvent::CommandIssued>({ 
				new ValueEditedCommand<T>(ref, undoValue, redoValue) });
		}
		return ret;
	}

	Properties::Properties(EditorLayer& editor) : 
		m_editor(editor),
		m_listener(EventBus::Instance())
	{
		m_listener.listen<EditorEvent::EntitySelected>(MM_EVENT_FN(Properties::OnEntitySelected));
	}

	template <typename T>
	void Properties::ShowPanel() 
	{
		if (!m_entity)
			return;

		T* e = dynamic_cast<T*>(m_entity);
		if (e)
			ShowPanelCore<T>(*e);
	}

	template <>
	void Properties::ShowPanelCore<World>(World& world) 
	{
		ImGui::SeparatorText("World");
		if (ImGui::Button("Load model")) {
			nfdchar_t* path = nullptr;
			nfdresult_t result = NFD_OpenDialog("pmx", NULL, &path);
			if (result == NFD_OKAY) {
				// PMXFile* pmx = ResourceManager::Instance().Load<PMXFile>(path);
				// Model* model = world.LoadModel(pmx);

				Model* model = world.LoadModel(path);
				EventBus::Instance()->postpone<EditorEvent::EntitySelected>({ model });
				std::free(path);
			}
		}
	}

	template <>
	void Properties::ShowPanelCore<PhysicsWorld>(PhysicsWorld& physicsWorld) 
	{
		ImGui::SeparatorText("Physics world");
		static bool enable;
		if (ImGui::Checkbox("Enable", &enable)) 
			physicsWorld.SetEnable(enable);
		if (ImGui::Button("Reset"))
			physicsWorld.Reset();
	}

	template <>
	void Properties::ShowPanelCore<Model>(Model& model) 
	{
		ImGui::SeparatorText(model.GetPMXFile().GetInfo().nameJP.c_str());
		if (ImGui::Button("Load animation")) {
			nfdchar_t* path = nullptr;
			nfdresult_t result = NFD_OpenDialog("vmd", NULL, &path);
			if (result == NFD_OKAY) {
				// VMDFile* vmd = ResourceManager::Instance().Load<VMDFile>(path);
				// Animation* anim = model.LoadAnimation(vmd)
				Animation* anim = model.LoadAnimation(path);
				EventBus::Instance()->postpone<EditorEvent::EntitySelected>({ anim });
				EventBus::Instance()->postpone<EditorEvent::FrameSet>({ 0 });
				std::free(path);
			}
		}
		if (ImGui::Button("Export animation")) {
			nfdchar_t* path = nullptr;
			nfdresult_t result = NFD_SaveDialog("vmd", NULL, &path);
			if (result == NFD_OKAY) {
				VMDFile vmd(model.GetAnim());
				vmd.Serialize(path);
				std::free(path);
			}
		}
	}

	template <>
	void Properties::ShowPanelCore<Camera>(Camera& camera) 
	{
		ImGui::SeparatorText("Camera");
		static glm::vec3 undo;
		WithUndo(undo, ImGui::InputFloat3, "Translation", camera.m_center, "%.2f", 0);
	}

	template <>
	void Properties::ShowPanelCore<DirectionalLight>(DirectionalLight& directionalLight) 
	{
		ImGui::SeparatorText(directionalLight.m_name.c_str());

		static glm::vec3 undo0;
		WithUndo(undo0, ImGui::DragFloat3, "Color", directionalLight.m_color, 0.01f, 0.0f, 1.0f, "%.2f", 0);

		static glm::vec3 undo1;
		WithUndo(undo1, ImGui::DragFloat3, "Direction", directionalLight.m_direction, 0.01f, -1.0f, 1.0f, "%.2f", 0);
	}

	template <>
	void Properties::ShowPanelCore<Armature>(Armature& armature) 
	{
		ImGui::SeparatorText("Armature");
	}

	static void QuatToEulerInput(glm::vec3& undoEuler, const char* name, glm::quat& q, uint32_t flags = ImGuiInputTextFlags_None)
	{
		glm::vec3 eulerDeg = glm::eulerAngles(q) * glm::degrees(1.0f);
		if (WithUndo(undoEuler, ImGui::DragFloat3, name, eulerDeg, 0.01f, -180.f, 180.f, "%.2f", flags)) 
			q = glm::quat(eulerDeg * glm::radians(1.0f));
	}

	template <>
	void Properties::ShowPanelCore<Armature::Bone>(Armature::Bone& bone) 
	{
		ImGui::SeparatorText(bone.name.c_str());
		ImGui::Text("Pose");

		static glm::vec3 undo0;
		WithUndo(undo0, ImGui::InputFloat3, "Translation##Pose", bone.pose.translation, "%.2f", 0);

		static glm::vec3 undo1;
		QuatToEulerInput(undo1, "Rotation##Pose", bone.pose.rotation);
	
		/* Read only */
		ImGui::Text("Local transform");
		static glm::vec3 _;
		ImGui::InputFloat3("Translation##Local", glm::value_ptr(bone.animLocal.translation), "%.2f", ImGuiInputTextFlags_ReadOnly);
		QuatToEulerInput(_, "Rotation##Local", bone.animLocal.rotation, ImGuiSliderFlags_NoInput);
		ImGui::Text("World transform");
		ImGui::InputFloat3("Translation##World", glm::value_ptr(bone.animWorld.translation), "%.2f", ImGuiInputTextFlags_ReadOnly);
		QuatToEulerInput(_, "Rotation##World", bone.animWorld.rotation, ImGuiInputTextFlags_ReadOnly);
	}

	template <>
	void Properties::ShowPanelCore<Skin::Mesh>(Skin::Mesh& mesh) 
	{
		ImGui::SeparatorText(mesh.name.c_str());
	}

	static void MorphSliders(Model& model, uint32_t panel) 
	{
		auto& morph = model.GetMorph();
		auto& pmxMorphs = model.GetPMXFile().GetMorphs();
		uint32_t morphCount = morph.GetWeights().size();

		for (uint32_t morphIndex = 0; morphIndex < morphCount; ++morphIndex) {
			if (pmxMorphs[morphIndex].panel == panel) {
				const std::string& name = model.GetPMXFile().GetMorphName(morphIndex);
				float& ref = morph.GetWeights()[morphIndex];
				static float undo;
				WithUndo(undo, ImGui::SliderFloat, name.c_str(), ref, 0.0f, 1.0f, "%.2f", 0);
			}
		}
	}

	template <>
	void Properties::ShowPanelCore<Morph>(Morph& morph) 
	{
		ImGui::SeparatorText("Morph");

		auto& model = morph.GetModel();

		if (ImGui::TreeNode("Eye")) {
			MorphSliders(model, PMXFile::PANEL_EYE);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Eyebrow")) {
			MorphSliders(model, PMXFile::PANEL_EYEBROW);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Lip")) {
			MorphSliders(model, PMXFile::PANEL_MOUTH);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Other")) {
			MorphSliders(model, PMXFile::PANEL_OTHER);
			ImGui::TreePop();
		}
	}

	void Properties::OnUIRender()
	{
		ImGui::Begin("Properties");

		ShowPanel<World>();
		ShowPanel<PhysicsWorld>();
		ShowPanel<Camera>();

		/* Lights */
		ShowPanel<DirectionalLight>();
		//ShowPanel<PointLight>();
		//ShowPanel<SpotLight>();
		//ShowPanel<AreaLight>();

		ShowPanel<Model>();
		ShowPanel<Armature>();
		ShowPanel<Armature::Bone>();
		//ShowPanel<Skin>();
		ShowPanel<Skin::Mesh>();
		ShowPanel<Morph>();

		ImGui::End();
	}

	void Properties::OnEntitySelected(const EditorEvent::EntitySelected& e)
	{
		m_entity = e.entity;
	}
}
