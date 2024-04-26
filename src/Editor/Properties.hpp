#pragma once

namespace mm
{
	class EditorLayer;

	class Properties
	{
	public:
		Properties(EditorLayer& editor) : m_editor(editor) {}
		void OnUIRender();

	private:
		void OnItemSelected();

	private:
		EditorLayer& m_editor;
	};
}

