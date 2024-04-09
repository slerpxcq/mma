#include "mmpch.hpp"
#include "KeyframeEditor.hpp"

#include "EditorLayer.hpp"
#include "EditorEvent.hpp"

#include "Core/MM/Model/Model.hpp"
#include "Core/App/Application.hpp"

#include "Core/Utility/Type.hpp"

namespace mm
{
	KeyframeEditor::KeyframeEditor(EditorLayer& editor) :
		m_editor(editor),
		m_listener(Application::Instance().GetEventBus())
	{
		m_listener.listen<EditorEvent::ModelLoaded>(MM_EVENT_FN(KeyframeEditor::OnModelLoaded));
		m_listener.listen<EditorEvent::MotionLoaded>(MM_EVENT_FN(KeyframeEditor::OnMotionLoaded));
	}

	void KeyframeEditor::OnModelLoaded(const EditorEvent::ModelLoaded& e)
	{
		m_model = e.model;

		if (m_model != nullptr) {
			const auto& pmxBones = m_model->GetPMXFile().GetBones();
			for (const auto& pb : pmxBones) {
				if (pb.flags & PMXFile::BONE_OPERABLE_BIT) {
					m_sequencer.AddItem(KeyframeSequence::Item({ pb.nameJP, 0, 100, false }));
				}
			}
		}
	}

	void KeyframeEditor::OnMotionLoaded(const EditorEvent::MotionLoaded& e)
	{
		if (m_model != nullptr) {
			m_playing = false;
			m_frame = 0;
			m_subframe = 0;
			UpdateAnim();
		}
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
		ImGui::Begin("Playback");
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

		m_sequencer.OnUIRender();
	}
}
