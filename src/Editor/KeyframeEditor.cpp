#include "mmpch.hpp"
#include "KeyframeEditor.hpp"

#include "EditorLayer.hpp"
#include "EditorEvent.hpp"

#include "Core/MM/Model/Model.hpp"
#include "Core/App/Application.hpp"

namespace mm
{
	KeyframeEditor::KeyframeEditor(EditorLayer& editor) :
		m_editor(editor),
		m_listener(Application::Instance().GetEventBus())
	{
		m_listener.listen<EditorEvent::ModelLoaded>(MM_EVENT_FN(KeyframeEditor::OnModelLoaded));
		m_listener.listen<EditorEvent::MotionLoaded>(MM_EVENT_FN(KeyframeEditor::OnMotionLoaded));

		 //m_sequence.myItems.push_back(MySequence::MySequenceItem{ 0, 10, 30, false });
		 //m_sequence.myItems.push_back(MySequence::MySequenceItem{ 1, 20, 30, true });
		 //m_sequence.myItems.push_back(MySequence::MySequenceItem{ 3, 12, 60, false });
		 //m_sequence.myItems.push_back(MySequence::MySequenceItem{ 2, 61, 90, false });
		 //m_sequence.myItems.push_back(MySequence::MySequenceItem{ 4, 90, 99, false });
	}

	void KeyframeEditor::OnModelLoaded(const EditorEvent::ModelLoaded& e)
	{
		m_model = e.model;
		m_clusters.clear();

		const auto& pmxClusters = m_model->GetPMXFile().GetClusters();
		for (uint32_t i = 0; i < pmxClusters.size(); ++i) {
			m_clusters.emplace_back();
			m_clusters.back().name = pmxClusters[i].nameJP;
		}

		const auto& pmxBones = m_model->GetPMXFile().GetBones();
		const auto& pmxMorphs = m_model->GetPMXFile().GetMorphs();
		for (uint32_t i = 0; i < m_clusters.size(); ++i) {
			auto& cluster = m_clusters[i];
			const auto& pmxCluster = pmxClusters[i];
			for (uint32_t j = 0; j < pmxCluster.elements.size(); ++j) {
				const auto& element = pmxCluster.elements[j];
				std::string name;
				switch (element.type) {
				case PMXFile::CLUSTER_BONE:
					name = pmxBones[element.index].nameJP;
					break;
				case PMXFile::CLUSTER_MORPH:
					name = pmxMorphs[element.index].nameJP;
					break;
				}

				cluster.items.push_back(MySequence::MySequenceItem({ std::move(name), 0, 0, 0, false }));
			}
		}

		for (uint32_t i = 0; i < m_clusters.size(); ++i) {
			MM_INFO("{0}", m_clusters[i].name);
			for (auto& i : m_clusters[i].items)
				MM_INFO("- {0}", i.label);
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

		ImGui::Begin("Keyframe edit");
		static int selectedEntry = -1;
		static int firstFrame = 0;
		static bool expanded = true;
		static int currentFrame = 100;

		//ImGui::PushItemWidth(130);
		//ImGui::InputInt("Frame Min", &m_clusters.minFrame);
		//ImGui::SameLine();
		//ImGui::InputInt("Frame ", &currentFrame);
		//ImGui::SameLine();
		//ImGui::InputInt("Frame Max", &m_clusters.maxFrame);
		//ImGui::PopItemWidth();

		if (m_model != nullptr) {
			for (uint32_t i = 0; i < m_clusters.size(); ++i) {
				if (ImGui::TreeNode("%s", m_clusters[i].name.c_str())) {
					ImSequencer::Sequencer(&m_clusters[i], &currentFrame, &expanded, &selectedEntry, &firstFrame,
					 ImSequencer::SEQUENCER_EDIT_STARTEND | 
					 ImSequencer::SEQUENCER_COPYPASTE | 
					 ImSequencer::SEQUENCER_CHANGE_FRAME);
					ImGui::TreePop();
				}
			}
		}

		ImGui::End();
	}
}
