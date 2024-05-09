#pragma once

#include "Core/App/EventBus.hpp"
#include "Core/MM/Animation/Animation.hpp"

#include "Core/App/Event.hpp"
#include "EditorEvent.hpp"

namespace mm
{
	class EditorLayer;

	class CurveEditor
	{
		static constexpr float DOPE_RADIUS = 5.f;
		static constexpr uint32_t DOPE_OUTLINE_COLOR = 0xff0080ff;
		static constexpr float DOPE_OUTLINE_SIZE = 2.5f;
		static constexpr uint32_t DOPE_FILL_COLOR = 0xff000000;
		static constexpr uint32_t BACKGROUND_COLOR = 0xff242424;
		static constexpr uint32_t LEGEND_LENGTH = 200;
		static constexpr uint32_t HEADER_COLOR = 0xff3d3837;
		static constexpr uint32_t RULER_HEIGHT = 20;
	public:
		CurveEditor(EditorLayer& editor) :
			m_editor(editor),
			m_listener(EventBus::Instance()) {
			m_listener.listen<EditorEvent::ItemSelected>(MM_EVENT_FN(CurveEditor::OnItemSelected));
			m_listener.listen<Event::MouseScrolled>(MM_EVENT_FN(CurveEditor::OnMouseScrolled));
		}

		void OnUIRender();
		void SetContainer(Animation::KeyframeContainer<Animation::BoneKeyframe>& container) {
			m_container = &container;
		}

	private:
		void OnItemSelected(const EditorEvent::ItemSelected& e);
		void OnMouseScrolled(const Event::MouseScrolled& e);
		void DrawDiamond(const ImVec2& center, float radius, float outlineSize, uint32_t outlineColor, uint32_t fillColor);

		void DrawScale();
		void DrawRows();

	private:
		EditorLayer& m_editor;

		Animation::KeyframeContainer<Animation::BoneKeyframe>* m_container = nullptr;

		bool m_hovered = false;
		ImDrawList* m_drawList;
		ImVec2 m_canvasMin;
		ImVec2 m_canvasMax;
		ImVec2 m_canvasOrigin;
		ImVec2 m_canvasSize;

		int32_t m_minFrame = 0;
		int32_t m_maxFrame = 10;

		float m_yGain = 1.0f;

		dexode::EventBus::Listener m_listener;
	};
}

