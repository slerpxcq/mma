#include "mmpch.hpp"
#include "PoseEditor.hpp"

#include "EditorLayer.hpp"
#include "EditorEvent.hpp"

#include "Core/MM/Model/Model.hpp"
#include "Core/App/Input.hpp"
#include "Core/Locale/Locale.hpp"

#include "Core/App/Application.hpp"
#include "Core/App/EventBus.hpp"
#include "Core/Utility/Type.hpp"

#include "Commands.hpp"

namespace mm
{
	PoseEditor::PoseEditor(EditorLayer& editor) :
		m_editor(editor),
		m_listener(EventBus::Instance())
	{
		m_listener.listen<Event::MouseButtonPressed>(MM_EVENT_FN(PoseEditor::OnMouseButtonPressed));
		m_listener.listen<Event::KeyPressed>(MM_EVENT_FN(PoseEditor::OnKeyPressed));
		m_listener.listen<EditorEvent::ModelLoaded>(MM_EVENT_FN(PoseEditor::OnModelLoaded));
	}

	/* This does not need to be an event */
	void PoseEditor::OnModelLoaded(const EditorEvent::ModelLoaded& e)
	{
		m_model = e.model;

		if (m_model != nullptr) {
			m_context.screenPos.clear();
			m_context.screenPos.resize(m_model->GetArmature().GetBones().size());
		}
	}

	void PoseEditor::SetModel(Model* model)
	{
		m_model = model;

		if (m_model != nullptr) {
			m_context.screenPos.clear();
			m_context.screenPos.resize(m_model->GetArmature().GetBones().size());
		}
	}

	void PoseEditor::OnUpdate(float deltaTime)
	{
	}

	void PoseEditor::EditTransform()
	{
		/* Axis limit and local axes */
		const auto& camera = m_editor.GetWorld().GetCamera();
		const float* view = glm::value_ptr(camera.GetView());
		const float* proj = glm::value_ptr(camera.GetProj());

		/* Gizmo */
		ImGuizmo::Enable(true);
		if (m_context.fixedAxis) {
			ImGuizmo::Manipulate(
				view, proj,
				ImGuizmo::ROTATE_X,
				ImGuizmo::LOCAL,
				glm::value_ptr(m_context.localFrame));
		} 
		else if (m_context.useLocalFrame) {
			ImGuizmo::Manipulate(
				view, proj,
				ImGuizmo::ROTATE,
				ImGuizmo::LOCAL,
				glm::value_ptr(m_context.localFrame));
		}
		else {
			ImGuizmo::Manipulate(
				view, proj,
				(ImGuizmo::OPERATION)m_context.operation,
				(ImGuizmo::MODE)m_context.mode,
				glm::value_ptr(m_context.world));
		}

		/* Transform back to local space */
		glm::mat4 world;
		if (m_context.useLocalFrame || m_context.fixedAxis)
			world = m_context.localFrame * m_context.localFrameInv;
		else
			world = m_context.world;
			
		glm::mat4 local = m_context.worldToLocal * world;
		Transform* valuePtr = &m_model->GetArmature().GetPose()[m_context.selected];
		*valuePtr = Transform(local);

		static bool lastFrameUsedGizmo = false;
		static Transform undoValue;
		bool thisFrameUsedGizmo = ImGuizmo::IsUsingAny();
		if (!lastFrameUsedGizmo && thisFrameUsedGizmo) {
			undoValue = *valuePtr;
			MM_INFO("Start edit; undo={0}", glm::to_string(undoValue.rotation));
		}
		if (lastFrameUsedGizmo && !thisFrameUsedGizmo) {
			EventBus::Instance()->postpone<EditorEvent::CommandIssued>({
				new Command::BoneEdited(valuePtr, *valuePtr, undoValue) });
			MM_INFO("End edit; undo={0}, redo={1}", glm::to_string(undoValue.rotation), glm::to_string(valuePtr->rotation));

			/* Add or modify the keyframe, same as for morph */
			Animation& anim = m_model->GetAnim();
			auto& boneKeyframes = anim.GetBoneKeyframeMatrix()[m_context.selected];
			uint32_t currFrame = m_editor.m_sequencer->GetFrameCounter().GetFrame();
			auto it = std::find_if(boneKeyframes.begin(), boneKeyframes.end(),
				[currFrame](const Animation::Keyframe& keyframe) {
					return keyframe.frame == currFrame;
				});

			if (it == boneKeyframes.end()) {
				anim.InsertBoneKeyframe(
					m_context.selected, 
					Animation::BoneKeyframe(currFrame, *valuePtr, Bezier()));
				EventBus::Instance()->postpone<EditorEvent::CommandIssued>({
					new Command::KeyframeInserted(anim, Command::KeyframeInserted::TYPE_BONE,
						m_context.selected, currFrame) });
			}
			else {
				it->transform = *valuePtr;
			}
		}
		lastFrameUsedGizmo = thisFrameUsedGizmo;
	}

	glm::vec3 PoseEditor::WorldToScreen(const glm::vec3& world)
	{
		const auto& camera = m_editor.GetWorld().GetCamera();
		glm::mat4 viewProj = camera.GetProj() * camera.GetView();
		glm::vec4 ndcPos = viewProj * glm::vec4(world, 1.0);
		ndcPos /= ndcPos.w;
        ImVec2 min = ImGui::GetWindowContentRegionMin();
        ImVec2 max = ImGui::GetWindowContentRegionMax();
        glm::vec2 viewportSize = glm::vec2(max.x - min.x, max.y - min.y);
        glm::vec2 viewportPos = glm::vec2(ImGui::GetWindowPos().x + min.x, ImGui::GetWindowPos().y + min.y);
		return glm::vec3(
			(ndcPos.x + 1.0f) * 0.5f * viewportSize.x + viewportPos.x,
			(-ndcPos.y + 1.0f) * 0.5f * viewportSize.y + viewportPos.y,
			ndcPos.z);
	}

	void PoseEditor::DrawBones()
	{
		const auto& bones = m_model->GetArmature().GetBones();
		const auto& pmxBones = m_model->GetPMXFile().GetBones();

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		for (uint32_t i = 0; i < bones.size(); ++i) {
			const auto& bone = bones[i];
			auto& boneScreenPos = m_context.screenPos[i];
			boneScreenPos = WorldToScreen(bone.animWorld.translation);
		}

		for (uint32_t i = 0; i < bones.size(); ++i) {
			const auto& bone = bones[i];
			const auto& pmxBone = pmxBones[i];
			auto& boneScreenPos = m_context.screenPos[i];

			if (pmxBone.flags & PMXFile::BONE_VISIBLE_BIT) {
				uint32_t color = (i == m_context.selected) ?
					SELECTED_COLOR : 
					UNSELECTED_COLOR;

				/* Button to select bone */
				char buf[16];
				std::snprintf(buf, sizeof(buf), "B_%u", i);
				ImGui::SetCursorScreenPos(ImVec2(boneScreenPos.x - CIRCLE_RADIUS, boneScreenPos.y - CIRCLE_RADIUS));
				if (ImGui::InvisibleButton(buf, ImVec2(CIRCLE_RADIUS * 2, CIRCLE_RADIUS * 2))) {
					m_context.selected = i;
				}

				/* Moveable bones: square */
				if (pmxBone.flags & PMXFile::BONE_MOVEABLE_BIT) {
					drawList->AddRectFilled(
						ImVec2(boneScreenPos.x - CIRCLE_RADIUS, boneScreenPos.y - CIRCLE_RADIUS),
						ImVec2(boneScreenPos.x + CIRCLE_RADIUS, boneScreenPos.y + CIRCLE_RADIUS), color);
					drawList->AddRect(
						ImVec2(boneScreenPos.x - CIRCLE_RADIUS, boneScreenPos.y - CIRCLE_RADIUS),
						ImVec2(boneScreenPos.x + CIRCLE_RADIUS, boneScreenPos.y + CIRCLE_RADIUS), OUTLINE_COLOR, OUTLINE_SIZE);
				}
				/* Fixed axis bones: triangle */
				else if (pmxBone.flags & PMXFile::BONE_FIXED_AXIS_BIT) {
					static constexpr float HALF_SQRT3 = 0.866f;
					drawList->AddTriangleFilled(
						ImVec2(boneScreenPos.x - HALF_SQRT3 * CIRCLE_RADIUS, boneScreenPos.y + 0.5f * CIRCLE_RADIUS),
						ImVec2(boneScreenPos.x, boneScreenPos.y - CIRCLE_RADIUS),
						ImVec2(boneScreenPos.x + HALF_SQRT3 * CIRCLE_RADIUS, boneScreenPos.y + 0.5f * CIRCLE_RADIUS),
						color);
					drawList->AddTriangle(
						ImVec2(boneScreenPos.x - HALF_SQRT3 * CIRCLE_RADIUS, boneScreenPos.y + 0.5f * CIRCLE_RADIUS),
						ImVec2(boneScreenPos.x, boneScreenPos.y - CIRCLE_RADIUS),
						ImVec2(boneScreenPos.x + HALF_SQRT3 * CIRCLE_RADIUS, boneScreenPos.y + 0.5f * CIRCLE_RADIUS),
						color, OUTLINE_SIZE);
				}
				/* Rotatable bones: circle */
				else {
					drawList->AddCircleFilled(
						ImVec2(boneScreenPos.x, boneScreenPos.y),
						CIRCLE_RADIUS, color);
					drawList->AddCircle(
						ImVec2(boneScreenPos.x, boneScreenPos.y),
						CIRCLE_RADIUS, OUTLINE_COLOR, 0, OUTLINE_SIZE);
				}

				glm::vec2 endPos(0.0f);
				if (pmxBone.flags & PMXFile::BONE_CONNECTED_BIT) {
					int32_t end = pmxBone.connetcionEnd.boneIndex;
					endPos = (end >= 0) ? 
						glm::vec2(m_context.screenPos[end]) :
						endPos = boneScreenPos;
				}
				else {
					glm::vec3 endWorld =
						glm::make_vec3(pmxBone.connetcionEnd.position);
					endWorld = glm::rotate(bones[i].animWorld.rotation, endWorld);
					endWorld += bones[i].animWorld.translation;
					endPos = WorldToScreen(endWorld);
				}
				glm::vec2 screenPos2D(boneScreenPos);
				glm::vec2 d = glm::normalize(endPos - screenPos2D);
				glm::vec2 n(-d.y, d.x);
				glm::vec2 p1 = screenPos2D + CIRCLE_RADIUS * n;
				glm::vec2 p2 = screenPos2D - CIRCLE_RADIUS * n;

				/* Draw the stick */
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
		}
	}

	void PoseEditor::OnKeyPressed(const Event::KeyPressed& e)
	{
		if (!m_enabled || m_model == nullptr)
			return;

		const auto& bones = m_model->GetArmature().GetBones();
		const auto& pmxBones = m_model->GetPMXFile().GetBones();
		const auto& pmxBone = pmxBones[m_context.selected];

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

					/* Axis limit and local frame */
					m_context.fixedAxis = pmxBone.flags & PMXFile::BONE_FIXED_AXIS_BIT;
					m_context.useLocalFrame = pmxBone.flags & PMXFile::BONE_LOCAL_AXIS_BIT;
					if (m_context.fixedAxis || m_context.useLocalFrame) {
						glm::vec3 x, y, z;
						if (m_context.fixedAxis) {
							x = glm::make_vec3(pmxBone.fixedAxis);
							z = glm::normalize(glm::cross(glm::vec3(0, 1, 0), x));
							y = glm::cross(z, x);
						}
						else if (m_context.useLocalFrame) {
							x = glm::make_vec3(pmxBone.localAxisX);
							z = glm::make_vec3(pmxBone.localAxisZ);
							y = glm::cross(z, x);
						}
						glm::mat4 bindLocalFrame;
						bindLocalFrame[0] = glm::vec4(x, 0);
						bindLocalFrame[1] = glm::vec4(y, 0);
						bindLocalFrame[2] = glm::vec4(z, 0);
						bindLocalFrame[3] = glm::vec4(0, 0, 0, 1);
						m_context.localFrame = m_context.world * bindLocalFrame;
						m_context.localFrameInv = glm::inverse(bindLocalFrame);
					}
					
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
					//m_context.selected = -1;
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
					if (e.code == GLFW_KEY_R && 
						pmxBone.flags & PMXFile::BONE_ROTATABLE_BIT)
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
					if (e.code == GLFW_KEY_T &&
						pmxBone.flags & PMXFile::BONE_MOVEABLE_BIT)
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


	void PoseEditor::OnUIRender()
	{
		ImGui::Begin("Pose editor");
		ImGui::Checkbox("Enable", &m_enabled);
		if (ImGui::Button("Reset pose")) {
			m_model->GetArmature().ResetPose();
		}
		ImGui::Text("Model: %s", m_model != nullptr ?
			m_model->GetPMXFile().GetInfo().nameJP.c_str() : "--");
		ImGui::Text("Bone: %s", m_model != nullptr && m_context.selected >= 0 ?
			m_model->GetPMXFile().GetBoneName(m_context.selected).c_str() : "--");
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
				DrawBones();
				break;
			case Context::EDITING:
				EditTransform();
				break;
			}
		}
		ImGui::End();
	}
}
