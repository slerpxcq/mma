#include "mmpch.hpp"
#include "KeyframeEditor.hpp"

#include "Core/MM/Model/Model.hpp"

namespace mm
{
	KeyframeEditor::KeyframeEditor(EditorLayer& editor) :
		m_editor(editor)
	{
		 m_sequence.mFrameMin = -100;
		 m_sequence.mFrameMax = 1000;
		 m_sequence.myItems.push_back(MySequence::MySequenceItem{ 0, 10, 30, false });
		 m_sequence.myItems.push_back(MySequence::MySequenceItem{ 1, 20, 30, true });
		 m_sequence.myItems.push_back(MySequence::MySequenceItem{ 3, 12, 60, false });
		 m_sequence.myItems.push_back(MySequence::MySequenceItem{ 2, 61, 90, false });
		 m_sequence.myItems.push_back(MySequence::MySequenceItem{ 4, 90, 99, false });
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

		ImGui::Begin("Keyframe edit");
		 static int selectedEntry = -1;
         static int firstFrame = 0;
         static bool expanded = true;
         static int currentFrame = 100;


         ImGui::PushItemWidth(130);
         ImGui::InputInt("Frame Min", &m_sequence.mFrameMin);
         ImGui::SameLine();
         ImGui::InputInt("Frame ", &currentFrame);
         ImGui::SameLine();
         ImGui::InputInt("Frame Max", &m_sequence.mFrameMax);
         ImGui::PopItemWidth();
         ImSequencer::Sequencer(&m_sequence, &currentFrame, &expanded, &selectedEntry, &firstFrame, 
			 ImSequencer::SEQUENCER_EDIT_STARTEND | 
			 ImSequencer::SEQUENCER_ADD | 
			 ImSequencer::SEQUENCER_DEL | 
			 ImSequencer::SEQUENCER_COPYPASTE | 
			 ImSequencer::SEQUENCER_CHANGE_FRAME);
         // add a UI to edit that particular item
         if (selectedEntry != -1)
         {
           const MySequence::MySequenceItem &item = m_sequence.myItems[selectedEntry];
           ImGui::Text("I am a %s, please edit me", SequencerItemTypeNames[item.mType]);
           // switch (type) ....
         }
		ImGui::End();
	}

	void KeyframeEditor::SetModel(Model* model)
	{
		m_model = model;
	}
}
