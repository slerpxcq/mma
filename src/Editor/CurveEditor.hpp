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
		static constexpr float DOPE_RADIUS = 4.f;
		static constexpr uint32_t DOPE_OUTLINE_COLOR = 0xff0080ff;
		static constexpr float DOPE_OUTLINE_SIZE = 2.5f;
		static constexpr uint32_t DOPE_FILL_COLOR = 0xff000000;
		static constexpr uint32_t BACKGROUND_COLOR = 0xff242424;
		static constexpr uint32_t LEGEND_LENGTH = 250;
		static constexpr uint32_t HEADER_COLOR = 0xff3d3837;
		static constexpr uint32_t RULER_HEIGHT = 20;
		static constexpr uint32_t INDENT_BASE = 15;
		static constexpr uint32_t LEGEND_TEXT_OFFSET = 10;

		static constexpr uint32_t translationColors[3] = {
			0xbf0000ff,
			0xbf00ff00,
			0xbfff0000 };

		static constexpr uint32_t rotationColors[3] = {
			0xbf8080ff,
			0xbf80ff80,
			0xbfff8080 };
	public:
		CurveEditor(EditorLayer& editor) :
			m_editor(editor),
			m_listener(EventBus::Instance()) {
			m_listener.listen<EditorEvent::EntitySelected>(MM_EVENT_FN(CurveEditor::OnItemSelected));
			//m_listener.listen<Event::MouseScrolled>(MM_EVENT_FN(CurveEditor::OnMouseScrolled));
		}


		void OnUIRender();
		void SetContainer(Animation::KeyframeContainer<Animation::BoneKeyframe>& container) {
			m_container = &container;
		}

	private:
		void OnItemSelected(const EditorEvent::EntitySelected& e);

		void DrawDiamond(const ImVec2& center, float radius, float outlineSize, uint32_t outlineColor, uint32_t fillColor);
		void DrawScale();
		void DrawRows();
		void DrawRotationCurve(Animation::BoneKeyframe& keyframe, Animation::BoneKeyframe& next);
		void DrawTranslationCurve(Animation::BoneKeyframe& keyframe, Animation::BoneKeyframe& next);
		void DrawBezierEditor(Animation::BoneKeyframe& keyframe, uint32_t axis);

		const char* GenButtonId() {
			static char buf[16];
			std::snprintf(buf, sizeof(buf), "##CB%u", m_buttonIndex++);
			return buf;
		}

	private:
		struct SelectedDope {
			Animation::BoneKeyframe* keyframe = nullptr;
			int32_t axis = -1;
		};

		enum {
			TRANSLATION_X_BIT = (1<<0),
			TRANSLATION_Y_BIT = (1<<1),
			TRANSLATION_Z_BIT = (1<<2),
			ROTATION_X_BIT = (1<<3),
			ROTATION_Y_BIT = (1<<4),
			ROTATION_Z_BIT = (1<<5)
		};

	private:
		EditorLayer& m_editor;

		Animation::KeyframeContainer<Animation::BoneKeyframe>* m_container = nullptr;

		SelectedDope m_selectedKeyframe;

		uint32_t m_buttonIndex = 0;

		uint8_t m_enabledAxes = 0;

		/* Canvas */
		ImDrawList* m_drawList;
		ImVec2 m_canvasMin;
		ImVec2 m_canvasMax;
		ImVec2 m_canvasOrigin;
		ImVec2 m_canvasSize;

		/* Drawing */
		bool m_anyHandleActivated = false;

		ImVec2 m_rectMin = ImVec2(0, -10);
		ImVec2 m_rectMax = ImVec2(100, 10);

		int32_t m_minFrame = 0;
		int32_t m_maxFrame = 100;

		float m_yGain = 1.0f;

		dexode::EventBus::Listener m_listener;
	};
}

