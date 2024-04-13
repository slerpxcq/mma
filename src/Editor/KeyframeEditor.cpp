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
		m_listener(Application::Instance().GetEventBus()),
		m_sequencer(*this)
	{
		m_listener.listen<EditorEvent::ModelLoaded>(MM_EVENT_FN(KeyframeEditor::OnModelLoaded));
		m_listener.listen<EditorEvent::MotionLoaded>(MM_EVENT_FN(KeyframeEditor::OnMotionLoaded));
	}

	void KeyframeEditor::OnModelLoaded(const EditorEvent::ModelLoaded& e)
	{
		m_model = e.model;
		m_sequencer.SetModel(e.model);

		if (m_model != nullptr) {
			const auto& pmx = m_model->GetPMXFile();
			const auto& pmxClusters = m_model->GetPMXFile().GetClusters();
			for (uint32_t i = 0; i < pmxClusters.size(); ++i) {
				const auto& pc = pmxClusters[i];
				Sequencer::Group group = {};
				group.name = pc.nameJP;
				for (uint32_t j = 0; j < pc.elements.size(); ++j) {
					const auto& elem = pc.elements[j];
					Sequencer::Item item = {};
					item.type = elem.type;
					item.index = elem.index;
					switch (elem.type) {
					case PMXFile::CLUSTER_BONE:
						item.name = pmx.GetBoneName(elem.index);
						break;
					case PMXFile::CLUSTER_MORPH:
						item.name = pmx.GetMorphName(elem.index);
						break;
					}
					group.items.push_back(item);
				}
				m_sequencer.AddGroup(std::move(group));
			}
		}
	}

	void KeyframeEditor::OnMotionLoaded(const EditorEvent::MotionLoaded& e)
	{
		if (m_model != nullptr) {
			m_playing = false;
			m_frameCounter.Set(0);
			UpdateAnim();
		}
	}

	void KeyframeEditor::OnUpdate(float deltaTime)
	{
		if (m_playing)
			m_frameCounter.Step(deltaTime);
		UpdateAnim();
	}

	void KeyframeEditor::UpdateAnim()
	{
		if (m_model != nullptr && m_model->GetAnim() != nullptr) {
			m_model->GetAnim()->Update(m_frameCounter.GetFrame(), m_frameCounter.GetSubFrame());
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
		static int32_t frame;
		if (ImGui::InputInt("Frame", &frame)) {
			if (frame < 0)
				frame = 0;
			m_frameCounter.Set(frame);
			UpdateAnim();
		}
		ImGui::End();

		m_sequencer.OnUIRender();
	}
}
