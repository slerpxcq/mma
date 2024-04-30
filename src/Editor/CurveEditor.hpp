#pragma once

namespace mm
{
	class EditorLayer;

	class CurveEditor
	{
	public:
		CurveEditor(EditorLayer& editor) :
			m_editor(editor) {}

		void OnUIRender();

	private:
		EditorLayer& m_editor;
	};
}

