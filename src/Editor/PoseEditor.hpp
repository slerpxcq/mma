#pragma once

#include <ImGuizmo.h>
#include "Core/App/Event.hpp"
#include "Core/App/EventUser.hpp"

namespace mm
{
	class Model;
	class EditorLayer;
	
	class PoseEditor : public EventUser
	{
		static constexpr uint32_t SELECTED_COLOR = 0x7f0000ff;
		static constexpr uint32_t UNSELECTED_COLOR = 0x7fc0c0c0;
		static constexpr uint32_t OUTLINE_COLOR = 0x7f101010;
		static constexpr float CIRCLE_RADIUS = 7.5f;
		static constexpr float OUTLINE_SIZE = 1.5f;

	public:
		PoseEditor(EditorLayer& editor);
		void SetModel(Model* model);
		Model* GetModel() { return m_model; }
		void OnUpdate(float deltaTime);
		void OnUIRender();

	private:
		struct Context {
			enum State {
				PICKING = 0,
				EDITING
			};

			uint8_t state = PICKING;
			uint8_t operation = 0;
			uint8_t mode = 0;
			int32_t selected = -1;

			glm::mat4 world = glm::mat4(1);
			glm::mat4 worldToLocal = glm::mat4(1);

			std::vector<glm::vec3> screenPos;
		};

	private:
		glm::vec3 WorldToScreen(const glm::vec3& world);
		void Edit();
		void Pick();
		void Draw();

		void OnMouseButtonPressed(const Event::MouseButtonPressed& e);
		void OnKeyPressed(const Event::KeyPressed& e);

	private:
		EditorLayer& m_editor;
		Model* m_model = nullptr;
		Context m_context;
	};
}

