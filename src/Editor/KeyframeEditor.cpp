#include "mmpch.hpp"
#include "KeyframeEditor.hpp"

#include "Core/MM/Model/Model.hpp"

namespace mm
{
	KeyframeEditor::KeyframeEditor(EditorLayer& editor) :
		m_editor(editor)
	{
	}

	void KeyframeEditor::OnUpdate(float deltaTime)
	{
		static float accum;

		if (m_playing) {
			accum += deltaTime;
			while (accum >= SUBFRAME_STEP) {
				if (++m_subframe == SUBFRAME_COUNT) {
					m_subframe = 0;
					++m_frame;
				}
				UpdateAnim();
				accum -= SUBFRAME_STEP;
			}
		}
	}

	void KeyframeEditor::UpdateAnim()
	{
		if (m_model != nullptr && m_model->GetAnim() != nullptr) {
			m_model->GetAnim()->Update(m_frame, m_subframe);
		}
	}

	void KeyframeEditor::OnUIRender()
	{
		ImGui::Begin("Keyframe editor");
		std::string animName = m_model != nullptr && m_model->GetAnim() != nullptr ?
			m_model->GetAnim()->GetName() : "--";
		ImGui::Text("Animation: %s", animName.c_str());

		if (ImGui::Button("Play")) {
			m_playing = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop")) {
			m_playing = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset")) {
			m_playing = false;
			m_frame = 0;
			m_subframe = 0;
			UpdateAnim();
		}
		if (ImGui::InputInt("Frame", &m_frame)) {
			if (m_frame < 0)
				m_frame = 0;
			m_subframe = 0;
			UpdateAnim();
		}
		ImGui::End();
	}

	void KeyframeEditor::SetModel(Model* model)
	{
		m_model = model;
	}
}
