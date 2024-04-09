#include "mmpch.hpp"
#include "PoseEditor.hpp"

#include "EditorLayer.hpp"
#include "EditorEvent.hpp"

#include "Core/MM/Model/Model.hpp"
#include "Core/App/Input.hpp"
#include "Core/Locale/Locale.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "Core/App/Application.hpp"
#include "Core/Utility/Type.hpp"

namespace mm
{
	PoseEditor::PoseEditor(EditorLayer& editor) :
		m_editor(editor),
		m_listener(Application::Instance().GetEventBus())
	{
		m_listener.listen<Event::MouseButtonPressed>(MM_EVENT_FN(PoseEditor::OnMouseButtonPressed));
		m_listener.listen<Event::KeyPressed>(MM_EVENT_FN(PoseEditor::OnKeyPressed));
		m_listener.listen<EditorEvent::ModelLoaded>(MM_EVENT_FN(PoseEditor::OnModelLoaded));
	}

	void PoseEditor::OnModelLoaded(const EditorEvent::ModelLoaded& e)
	{
		m_model = e.model;

		if (m_model != nullptr) {
			m_context.screenPos.clear();
			m_context.screenPos.resize(m_model->GetArmature().GetBones().size());
		}
	}

	void PoseEditor::OnUpdate(float deltaTime)
	{
	}

	void PoseEditor::Edit()
	{
		ImGuizmo::Enable(true);

		ImGuizmo::Manipulate(
			glm::value_ptr(m_editor.GetWorld().GetCamera().GetView()),
			glm::value_ptr(m_editor.GetWorld().GetCamera().GetProj()),
			(ImGuizmo::OPERATION)m_context.operation,
			(ImGuizmo::MODE)m_context.mode,
			glm::value_ptr(m_context.world));

		glm::mat4 local = m_context.worldToLocal * m_context.world;
		m_model->GetArmature().GetPose()[m_context.selected] = Transform(local);
	}

	void PoseEditor::Pick()
	{
		const auto& pmxBones = m_model->GetPMXFile().GetBones();
		glm::vec2 viewportPos = m_editor.GetViewport().GetPos();
		glm::vec2 mousePos = Input::MousePos();
		int32_t selected = -1;
		float currZ = std::numeric_limits<float>::infinity();

		// Check against each bone in screen space
		for (uint32_t i = 0; i < m_context.screenPos.size(); ++i) {
			glm::vec3 screenPos = m_context.screenPos[i];
			if (pmxBones[i].flags & PMXFile::BONE_VISIBLE_BIT && 
				pmxBones[i].flags & PMXFile::BONE_OPERABLE_BIT &&
				glm::distance(glm::vec2(screenPos), mousePos) < CIRCLE_RADIUS &&
				screenPos.z < currZ) {
				selected = i;
				currZ = screenPos.z;
			}
		}

		m_context.selected = selected;
	}

	glm::vec3 PoseEditor::WorldToScreen(const glm::vec3& world)
	{
		const auto& camera = m_editor.GetWorld().GetCamera();
		glm::mat4 worldToNDC = camera.GetProj() * camera.GetView();
		glm::vec4 ndcPos = worldToNDC * glm::vec4(world, 1.0);
		ndcPos /= ndcPos.w;
		glm::vec2 viewportSize = m_editor.GetViewport().GetSize();
		glm::vec2 viewportPos = m_editor.GetViewport().GetPos();
		return glm::vec3(
			(ndcPos.x + 1.0f) * 0.5f * viewportSize.x + viewportPos.x,
			(-ndcPos.y + 1.0f) * 0.5f * viewportSize.y + viewportPos.y,
			ndcPos.z);
	}

	void PoseEditor::Draw()
	{
		const auto& bones = m_model->GetArmature().GetBones();
		const auto& pmxBones = m_model->GetPMXFile().GetBones();

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		for (uint32_t i = 0; i < bones.size(); ++i) {
			const auto& bone = bones[i];
			const auto& pmxBone = pmxBones[i];
			auto& screenPos = m_context.screenPos[i];

			screenPos = WorldToScreen(bone.animWorld.trans);

			if (pmxBone.flags & PMXFile::BONE_VISIBLE_BIT) {
				uint32_t color = (i == m_context.selected) ?
					SELECTED_COLOR : 
					UNSELECTED_COLOR;

				if (pmxBone.flags & PMXFile::BONE_MOVEABLE_BIT) {
					drawList->AddRectFilled(
						ImVec2(screenPos.x - CIRCLE_RADIUS, screenPos.y - CIRCLE_RADIUS),
						ImVec2(screenPos.x + CIRCLE_RADIUS, screenPos.y + CIRCLE_RADIUS), color);
					drawList->AddRect(
						ImVec2(screenPos.x - CIRCLE_RADIUS, screenPos.y - CIRCLE_RADIUS),
						ImVec2(screenPos.x + CIRCLE_RADIUS, screenPos.y + CIRCLE_RADIUS), OUTLINE_COLOR, OUTLINE_SIZE);
				}
				else if (pmxBone.flags & PMXFile::BONE_FIXED_AXIS_BIT) {
					static constexpr float HALF_SQRT3 = 0.866f;
					drawList->AddTriangleFilled(
						ImVec2(screenPos.x - HALF_SQRT3 * CIRCLE_RADIUS, screenPos.y + 0.5f * CIRCLE_RADIUS),
						ImVec2(screenPos.x, screenPos.y - CIRCLE_RADIUS),
						ImVec2(screenPos.x + HALF_SQRT3 * CIRCLE_RADIUS, screenPos.y + 0.5f * CIRCLE_RADIUS),
						color);
					drawList->AddTriangle(
						ImVec2(screenPos.x - HALF_SQRT3 * CIRCLE_RADIUS, screenPos.y + 0.5f * CIRCLE_RADIUS),
						ImVec2(screenPos.x, screenPos.y - CIRCLE_RADIUS),
						ImVec2(screenPos.x + HALF_SQRT3 * CIRCLE_RADIUS, screenPos.y + 0.5f * CIRCLE_RADIUS),
						color, OUTLINE_SIZE);
				}
				else {
					drawList->AddCircleFilled(
						ImVec2(screenPos.x, screenPos.y),
						CIRCLE_RADIUS, color);
					drawList->AddCircle(
						ImVec2(screenPos.x, screenPos.y),
						CIRCLE_RADIUS, OUTLINE_COLOR, 0, OUTLINE_SIZE);
				}

				glm::vec2 endPos(0.0f);
				if (pmxBone.flags & PMXFile::BONE_CONNECTED_BIT) {
					int32_t end = pmxBone.connetcionEnd.boneIndex;
					endPos = (end >= 0) ? 
						glm::vec2(m_context.screenPos[end]) :
						endPos = screenPos;
				}
				else {
					glm::vec3 endWorld =
						glm::make_vec3(pmxBone.connetcionEnd.position);
					endWorld = glm::rotate(bones[i].animWorld.rot, endWorld);
					endWorld += bones[i].animWorld.trans;
					endPos = WorldToScreen(endWorld);
				}
				glm::vec2 screenPos2D(screenPos);
				glm::vec2 d = glm::normalize(endPos - screenPos2D);
				glm::vec2 n(-d.y, d.x);
				glm::vec2 p1 = screenPos2D + CIRCLE_RADIUS * n;
				glm::vec2 p2 = screenPos2D - CIRCLE_RADIUS * n;

				drawList->AddLine(ImVec2(p1.x, p1.y), ImVec2(p2.x, p2.y), color, OUTLINE_SIZE);
				drawList->AddLine(ImVec2(p2.x, p2.y), ImVec2(endPos.x, endPos.y), color, OUTLINE_SIZE);
				drawList->AddLine(ImVec2(endPos.x, endPos.y), ImVec2(p1.x, p1.y), color, OUTLINE_SIZE);
			}
		}
	}

	void PoseEditor::OnMouseButtonPressed(const Event::MouseButtonPressed& e)
	{
		if (!m_enabled || m_model == nullptr)
			return;

		switch (m_context.state) {
		case Context::EDITING:
			if (e.button == GLFW_MOUSE_BUTTON_LEFT && !ImGuizmo::IsOver())
				m_context.state = Context::PICKING;
			break;
		case Context::PICKING:
			if (e.button == GLFW_MOUSE_BUTTON_LEFT)
				Pick();
			break;
		}
	}

	void PoseEditor::OnKeyPressed(const Event::KeyPressed& e)
	{
		if (!m_enabled || m_model == nullptr)
			return;

		const auto& bones = m_model->GetArmature().GetBones();

		switch (m_context.state) {
		case Context::PICKING:
			if (m_context.selected >= 0) {
				if (e.code == GLFW_KEY_R || e.code == GLFW_KEY_T) {  
					int32_t parent = bones[m_context.selected].parent;

					glm::mat4 parentAnimWorld = parent >= 0 ?
						Transform::toMat4(bones[parent].animWorld) :
						glm::identity<glm::mat4>();

					glm::mat4 bindParent = Transform::toMat4(bones[m_context.selected].bindParent);
					m_context.world = Transform::toMat4(bones[m_context.selected].animWorld);
					m_context.worldToLocal = glm::inverse(parentAnimWorld * bindParent);
					
					const auto& pmxBone = m_model->GetPMXFile().GetBones()[m_context.selected];
					if (e.code == GLFW_KEY_T && (pmxBone.flags & PMXFile::BONE_MOVEABLE_BIT)) {
						m_context.operation = ImGuizmo::TRANSLATE;
						m_context.state = Context::EDITING;
					}
					if (e.code == GLFW_KEY_R && (pmxBone.flags & PMXFile::BONE_ROTATABLE_BIT)) {
						m_context.operation = ImGuizmo::ROTATE;
						m_context.state = Context::EDITING;
					}
				}
			}
			break;
		case Context::EDITING:
			{
				if (e.code == GLFW_KEY_ESCAPE) {
					m_context.state = Context::PICKING;
					break;
				}

				switch (m_context.operation) {
				case ImGuizmo::TRANSLATE:
				case ImGuizmo::TRANSLATE_X:
				case ImGuizmo::TRANSLATE_Y:
				case ImGuizmo::TRANSLATE_Z:
					if (e.code == (GLFW_KEY_X)) 
						m_context.operation = ImGuizmo::TRANSLATE_X;
					if (e.code == (GLFW_KEY_Y)) 
						m_context.operation = ImGuizmo::TRANSLATE_Y;
					if (e.code == (GLFW_KEY_Z)) 
						m_context.operation = ImGuizmo::TRANSLATE_Z;
					if (e.code == (GLFW_KEY_R))
						m_context.operation = ImGuizmo::ROTATE;
					break;
				case ImGuizmo::ROTATE:
				case ImGuizmo::ROTATE_X:
				case ImGuizmo::ROTATE_Y:
				case ImGuizmo::ROTATE_Z:
					if (e.code == (GLFW_KEY_X)) 
						m_context.operation = ImGuizmo::ROTATE_X;
					if (e.code == (GLFW_KEY_Y)) 
						m_context.operation = ImGuizmo::ROTATE_Y;
					if (e.code == (GLFW_KEY_Z)) 
						m_context.operation = ImGuizmo::ROTATE_Z;
					if (e.code == (GLFW_KEY_T))
						m_context.operation = ImGuizmo::TRANSLATE;
					break;
				}
				
				// Toggle WORLD/LOCAL
				if (e.code == (GLFW_KEY_W)) {
					m_context.mode ^= 1;
				}
			}
			break;
		}
	}

	void PoseEditor::MorphSliders(uint32_t panel)
	{
		auto& morph = m_model->GetMorph();
		auto& pmxMorphs = m_model->GetPMXFile().GetMorphs();
		uint32_t morphCount = morph.GetWeights().size();
		for (uint32_t i = 0; i < morphCount; ++i) {
			if (pmxMorphs[i].panel == panel) {
				ImGui::SliderFloat(
					m_model->GetPMXFile().GetMorphName(i).c_str(),
					&morph.GetWeights()[i],
					0.0f, 1.0f);
			}
		}
	}

	void PoseEditor::OnUIRender()
	{
		ImGui::Begin("Pose editor");
		ImGui::Checkbox("Enable", &m_enabled);
		ImGui::Text("Model: %s", m_model != nullptr ?
			m_model->GetPMXFile().GetInfo().nameJP.c_str() : "--");
		ImGui::Text("Bone: %s", m_model != nullptr && m_context.selected >= 0 ?
			m_model->GetPMXFile().GetBoneName(m_context.selected).c_str() : "--");
		ImGui::End();

		ImGui::Begin("Morph edit");
		if (m_model != nullptr) {
			if (ImGui::TreeNode("Eye")) {
				MorphSliders(PMXFile::PANEL_EYE);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Eyebrow")) {
				MorphSliders(PMXFile::PANEL_EYEBROW);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Lip")) {
				MorphSliders(PMXFile::PANEL_MOUTH);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Other")) {
				MorphSliders(PMXFile::PANEL_OTHER);
				ImGui::TreePop();
			}
		}
		ImGui::End();

		ImGui::Begin("Viewport");
        ImVec2 min = ImGui::GetWindowContentRegionMin();
        ImVec2 max = ImGui::GetWindowContentRegionMax();
        ImVec2 size = max - min;
        ImVec2 pos = ImGui::GetWindowPos() + min;
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);
		if (m_enabled && m_model != nullptr) {
			switch (m_context.state) {
			case Context::PICKING:
				Draw();
				break;
			case Context::EDITING:
				Edit();
				break;
			}
		}
		ImGui::End();
	}
}
