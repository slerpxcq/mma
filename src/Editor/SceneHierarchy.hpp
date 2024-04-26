#pragma once

namespace mm
{
	class EditorLayer;
	class World;

	class SceneHierarchy
	{
	public:
		SceneHierarchy(EditorLayer& editor) : m_editor(editor) {}
		void SetWorld(World* world) { m_world = world; }
		void OnUIRender();

	private:
		EditorLayer& m_editor;
		World* m_world = nullptr;
	};
}

