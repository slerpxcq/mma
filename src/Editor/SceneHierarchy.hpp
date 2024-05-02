#pragma once

namespace mm
{
	class EditorLayer;
	class World;
	class Model;

	class SceneHierarchy
	{
		static constexpr uint32_t leafFlag = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		static constexpr uint32_t nonLeafFlag = ImGuiTreeNodeFlags_OpenOnArrow;
	public:
		SceneHierarchy(EditorLayer& editor) : m_editor(editor) {}
		void SetWorld(World* world) { m_world = world; }
		void OnUIRender();

	private:
		void CameraNode();
		void PhysicsWorldNode();
		void LightsNode();
		void SkinNode(Model&);
		void ArmatureNode(Model& model);
		void MorphNode(Model& model);
		void AnimationNode(Model& model);
		void ModelsNode();

	private:
		EditorLayer& m_editor;
		World* m_world = nullptr;
	};
}

