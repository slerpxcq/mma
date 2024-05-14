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
		m_listener.listen<EditorEvent::ItemSelected>(MM_EVENT_FN(PoseEditor::OnItemSelected));
		m_listener.listen<EditorEvent::FrameSet>(MM_EVENT_FN(PoseEditor::OnFrameSet));
	}

	void PoseEditor::OnItemSelected(const EditorEvent::ItemSelected& e)
	{
		//if (dynamic_cast<Item::Model*>(e.item) != nullptr)
		if (e.type == Properties::TYPE_MODEL) {
			SetModel(std::any_cast<Model*>(e.item));
		}
	}

	void PoseEditor::OnFrameSet(const EditorEvent::FrameSet& e)
	{
		m_context.editedBones.clear();
	}

	void PoseEditor::OnUpdate(float deltaTime)
	{
	}

	void PoseEditor::SetModel(Model* model)
	{
		m_model = model;

		if (m_model != nullptr) {
			m_context.screenPos.clear();
			m_context.editedBones.clear();
			m_context.screenPos.resize(m_model->GetArmature().GetBones().size());
		}
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
				(ImGuizmo::OPERATION)m_context.operation,
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
		Transform* valuePtr = &m_model->GetArmature().GetPose()[m_context.currSelectedBone];
		*valuePtr = Transform(local);

		static bool lastFrameUsedGizmo = false;
		static Transform undoValue;
		bool thisFrameUsedGizmo = ImGuizmo::IsUsingAny();

		/* "Rising edge" */
		if (!lastFrameUsedGizmo && thisFrameUsedGizmo) {
			undoValue = *valuePtr;
		}
		/* "Falling edge" */
		if (lastFrameUsedGizmo && !thisFrameUsedGizmo) {
			m_context.editedBones.insert(m_context.currSelectedBone);
			EventBus::Instance()->postpone<EditorEvent::CommandIssued>({
				new PoseEditorEditedCommand(valuePtr, *valuePtr, undoValue) });
		}
		lastFrameUsedGizmo = thisFrameUsedGizmo;
	}

	void PoseEditor::CommitEdited()
	{
		/* For all edited bones */
		Animation& anim = m_model->GetAnim();

		for (auto& bone : m_context.editedBones) {
			auto& boneKeyframes = anim.GetBoneKeyframeMatrix()[bone];

			uint32_t currFrame = m_editor.m_sequencer->GetFrameCounter().GetFrame();
			auto it = std::find_if(boneKeyframes.begin(), boneKeyframes.end(),
				[currFrame](const Animation::Keyframe& keyframe) {
					return keyframe.frame == currFrame;
				});

			Transform& transform = m_model->GetArmature().GetPose()[bone];
			if (it == boneKeyframes.end()) {
				anim.InsertBoneKeyframe(bone, Animation::BoneKeyframe(currFrame, transform, Bezier()));
				//EventBus::Instance()->postpone<EditorEvent::CommandIssued>({
				//	new Command::KeyframeInserted(anim, Command::KeyframeInserted::TYPE_BONE,
				//		m_context.currSelectedBone, currFrame) });
			}
			else {
				it->transform = transform;
			}
		}

		m_context.editedBones.clear();

		/* TODO: Undo/Redo for CommitEdit */
		//if (it == boneKeyframes.end()) {
		//	anim.InsertBoneKeyframe(
		//		m_context.currSelectedBone, 
		//		Animation::BoneKeyframe(currFrame, *valuePtr, Bezier()));
		//	EventBus::Instance()->postpone<EditorEvent::CommandIssued>({
		//		new Command::KeyframeInserted(anim, Command::KeyframeInserted::TYPE_BONE,
		//			m_context.currSelectedBone, currFrame) });
		//}
		//else {
		//	it->transform = *valuePtr;
		//}
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

		for (uint32_t boneIndex = 0; boneIndex < bones.size(); ++boneIndex) {
			const auto& bone = bones[boneIndex];
			auto& boneScreenPos = m_context.screenPos[boneIndex];
			boneScreenPos = WorldToScreen(bone.animWorld.translation);
		}

		for (uint32_t boneIndex = 0; boneIndex < bones.size(); ++boneIndex) {
			const auto& bone = bones[boneIndex];
			const auto& pmxBone = pmxBones[boneIndex];
			auto& boneScreenPos = m_context.screenPos[boneIndex];

			if (pmxBone.flags & PMXFile::BONE_VISIBLE_BIT) {
				//bool isCurrSelected = boneIndex == m_context.currSelectedBone;
				bool isSelected = m_context.selectedBones.find(boneIndex) != m_context.selectedBones.end();
				//isSelected |= isCurrSelected;
				bool isDirty = m_context.editedBones.find(boneIndex) != m_context.editedBones.end();

				uint32_t outlineColor = 
					isSelected ? SELECTED_OUTLINE_COLOR :
					isDirty ? DIRTY_OUTLINE_COLOR : UNSELECTED_OUTLINE_COLOR;

				uint32_t fillColor = 
					isSelected ? SELECTED_FILL_COLOR :
					isDirty ? DIRTY_FILL_COLOR : UNSELECTED_FILL_COLOR;

				float outlineSize = 
					isSelected ? SELECTED_OUTLINE_SIZE : UNSELECTED_OUTLINE_SIZE;

				/* Button to select bone */
				char buf[16];
				std::snprintf(buf, sizeof(buf), "B_%u", boneIndex);
				ImGui::SetCursorScreenPos(ImVec2(boneScreenPos.x - CIRCLE_RADIUS, boneScreenPos.y - CIRCLE_RADIUS));
				if (ImGui::InvisibleButton(buf, ImVec2(CIRCLE_RADIUS * 2, CIRCLE_RADIUS * 2))) {
					m_context.thisFrameClickedOnAnyBone = true;
					m_context.currSelectedBone = boneIndex;
					m_context.selectedBones.insert(boneIndex);
				}

				/* Moveable bones: square */
				if (pmxBone.flags & PMXFile::BONE_MOVEABLE_BIT) {
					drawList->AddRectFilled(
						ImVec2(boneScreenPos.x - CIRCLE_RADIUS, boneScreenPos.y - CIRCLE_RADIUS),
						ImVec2(boneScreenPos.x + CIRCLE_RADIUS, boneScreenPos.y + CIRCLE_RADIUS), 
						fillColor);
					drawList->AddRect(
						ImVec2(boneScreenPos.x - CIRCLE_RADIUS, boneScreenPos.y - CIRCLE_RADIUS),
						ImVec2(boneScreenPos.x + CIRCLE_RADIUS, boneScreenPos.y + CIRCLE_RADIUS), 
						outlineColor, 0, 0, outlineSize);
				}
				/* Fixed axis bones: triangle */
				else if (pmxBone.flags & PMXFile::BONE_FIXED_AXIS_BIT) {
					static constexpr float HALF_SQRT3 = 0.866f;
					drawList->AddTriangleFilled(
						ImVec2(boneScreenPos.x - HALF_SQRT3 * CIRCLE_RADIUS, boneScreenPos.y + 0.5f * CIRCLE_RADIUS),
						ImVec2(boneScreenPos.x, boneScreenPos.y - CIRCLE_RADIUS),
						ImVec2(boneScreenPos.x + HALF_SQRT3 * CIRCLE_RADIUS, boneScreenPos.y + 0.5f * CIRCLE_RADIUS),
						fillColor);
					drawList->AddTriangle(
						ImVec2(boneScreenPos.x - HALF_SQRT3 * CIRCLE_RADIUS, boneScreenPos.y + 0.5f * CIRCLE_RADIUS),
						ImVec2(boneScreenPos.x, boneScreenPos.y - CIRCLE_RADIUS),
						ImVec2(boneScreenPos.x + HALF_SQRT3 * CIRCLE_RADIUS, boneScreenPos.y + 0.5f * CIRCLE_RADIUS),
						outlineColor, outlineSize);
				}
				/* Rotatable bones: circle */
				else {
					drawList->AddCircleFilled(
						ImVec2(boneScreenPos.x, boneScreenPos.y),
						CIRCLE_RADIUS, fillColor);
					drawList->AddCircle(
						ImVec2(boneScreenPos.x, boneScreenPos.y),
						CIRCLE_RADIUS, outlineColor, 0, outlineSize);
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
					endWorld = glm::rotate(bones[boneIndex].animWorld.rotation, endWorld);
					endWorld += bones[boneIndex].animWorld.translation;
					endPos = WorldToScreen(endWorld);
				}
				glm::vec2 screenPos2D(boneScreenPos);
				glm::vec2 d = glm::normalize(endPos - screenPos2D);
				glm::vec2 n(-d.y, d.x);
				glm::vec2 p1 = screenPos2D + CIRCLE_RADIUS * n;
				glm::vec2 p2 = screenPos2D - CIRCLE_RADIUS * n;

				/* Draw the stick */
				//drawList->AddLine(ImVec2(p1.x, p1.y), ImVec2(p2.x, p2.y), outlineColor, outlineSize);
				drawList->AddLine(ImVec2(p2.x, p2.y), ImVec2(endPos.x, endPos.y), outlineColor, outlineSize);
				drawList->AddLine(ImVec2(endPos.x, endPos.y), ImVec2(p1.x, p1.y), outlineColor, outlineSize);
			}
		}
	}

	void PoseEditor::ProcessMouseButton()
	{
		if (!m_enabled || m_model == nullptr)
			return;

		switch (m_context.state) {
		case Context::State::EDITING:
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsOver())
				m_context.state = Context::State::PICKING;
			break;
		}
	}

	void PoseEditor::CalcStartTransform()
	{
		const auto& bones = m_model->GetArmature().GetBones();
		const auto& pmxBones = m_model->GetPMXFile().GetBones();
		const auto& pmxBone = pmxBones[m_context.currSelectedBone];

		int32_t parent = bones[m_context.currSelectedBone].parent;

		glm::mat4 parentAnimWorld = parent >= 0 ?
			Transform::toMat4(bones[parent].animWorld) :
			glm::identity<glm::mat4>();

		glm::mat4 bindParent = Transform::toMat4(bones[m_context.currSelectedBone].bindParent);
		m_context.world = Transform::toMat4(bones[m_context.currSelectedBone].animWorld);
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
	}

	void PoseEditor::ProcessKeys()
	{
		if (!m_enabled || m_model == nullptr)
			return;

		const auto& pmxBones = m_model->GetPMXFile().GetBones();

		switch (m_context.state) {
		case Context::State::PICKING:
			/* Select bone */
			if (m_context.currSelectedBone >= 0) {
				const auto& pmxBone = pmxBones[m_context.currSelectedBone];
				if (ImGui::IsKeyPressed(ImGuiKey_R) || ImGui::IsKeyPressed(ImGuiKey_T)) {
					CalcStartTransform();
					if (ImGui::IsKeyPressed(ImGuiKey_T) && (pmxBone.flags & PMXFile::BONE_MOVEABLE_BIT)) {
						m_context.operation = ImGuizmo::TRANSLATE;
						m_context.state = Context::State::EDITING;
					}
					if (ImGui::IsKeyPressed(ImGuiKey_R) && (pmxBone.flags & PMXFile::BONE_ROTATABLE_BIT)) {
						m_context.operation = ImGuizmo::ROTATE;
						m_context.state = Context::State::EDITING;
					}
				}
			}

			/* Copy */
			if (ImGui::IsKeyPressed(ImGuiKey_C) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
				if (!m_context.selectedBones.empty()) {
					auto content = std::make_unique<PoseEditorClipboardContent>();
					for (auto& bone : m_context.selectedBones) {
						std::string name = m_model->GetPMXFile().GetBoneName(bone);
						Transform transform = m_model->GetArmature().GetPose()[bone];
						MM_INFO("Copied: {0} {1}", name, glm::to_string(transform.rotation));
						content->items.push_back({ name, transform });
					}
					Clipboard::Instance().SetContent(std::move(content));
				}
			}

			/* Paste */
			if (ImGui::IsKeyPressed(ImGuiKey_V) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
				PoseEditorClipboardContent* content = dynamic_cast<PoseEditorClipboardContent*>(Clipboard::Instance().GetContent());
				if (content != nullptr) {
					const auto& dict = m_model->GetArmature().GetDict();
					auto& pose = m_model->GetArmature().GetPose();

					/* Mirror paste */
					if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
						const char* LEFT = u8"左";
						const char* RIGHT = u8"右";
						for (const auto& item : content->items) {
							std::string name = item.boneName;
							bool hasMirror = false;
							if (!std::strncmp(name.c_str(), LEFT, std::strlen(LEFT))) {
								name.replace(0, std::strlen(RIGHT), RIGHT);
								hasMirror = true;
							}
							else if (!std::strncmp(name.c_str(), RIGHT, std::strlen(RIGHT))) {
								name.replace(0, std::strlen(LEFT), LEFT);
								hasMirror = true;
							}

							Transform transform = item.transform;
							if (hasMirror) {
								transform.rotation = glm::conjugate(transform.rotation);
								transform.translation.x = -transform.translation.x;
							}

							try { pose[dict.at(name)] = transform; }
							catch (const std::out_of_range&) {}

							MM_INFO("Pasted: {0} {1}", name, glm::to_string(item.transform.rotation));
						}
					} 
					/* Normal paste */
					else {
						for (const auto& item : content->items) {
							try { pose[dict.at(item.boneName)] = item.transform; }
							catch (const std::out_of_range&) {}
							MM_INFO("Pasted: {0} {1}", item.boneName, glm::to_string(item.transform.rotation));
						}
					}
				}
			}
			break;

		case Context::State::EDITING:
			{
				const auto& pmxBone = pmxBones[m_context.currSelectedBone];
				if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
					m_context.state = Context::State::PICKING;
					break;
				}

				switch (m_context.operation) {
				case ImGuizmo::TRANSLATE:
				case ImGuizmo::TRANSLATE_X:
				case ImGuizmo::TRANSLATE_Y:
				case ImGuizmo::TRANSLATE_Z:
					if (ImGui::IsKeyPressed(ImGuiKey_X))
						m_context.operation = ImGuizmo::TRANSLATE_X;
					if (ImGui::IsKeyPressed(ImGuiKey_Y))
						m_context.operation = ImGuizmo::TRANSLATE_Y;
					if (ImGui::IsKeyPressed(ImGuiKey_Z))
						m_context.operation = ImGuizmo::TRANSLATE_Z;
					if (ImGui::IsKeyPressed(ImGuiKey_R) &&
						pmxBone.flags & PMXFile::BONE_ROTATABLE_BIT)
						m_context.operation = ImGuizmo::ROTATE;
					break;
				case ImGuizmo::ROTATE:
				case ImGuizmo::ROTATE_X:
				case ImGuizmo::ROTATE_Y:
				case ImGuizmo::ROTATE_Z:
					if (ImGui::IsKeyPressed(ImGuiKey_X))
						m_context.operation = ImGuizmo::ROTATE_X;
					if (ImGui::IsKeyPressed(ImGuiKey_Y))
						m_context.operation = ImGuizmo::ROTATE_Y;
					if (ImGui::IsKeyPressed(ImGuiKey_Z))
						m_context.operation = ImGuizmo::ROTATE_Z;
					if (ImGui::IsKeyPressed(ImGuiKey_T) &&
						pmxBone.flags & PMXFile::BONE_MOVEABLE_BIT)
						m_context.operation = ImGuizmo::TRANSLATE;
					break;
				}

				/* Toggle WORLD/LOCAL */
				if (ImGui::IsKeyPressed(ImGuiKey_W)) 
					m_context.mode ^= 1;
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
		ImGui::SameLine();
		if (ImGui::Button("Commit")) {
			CommitEdited();
		}
		ImGui::Text("Model: %s", m_model != nullptr ?
			m_model->GetPMXFile().GetInfo().nameJP.c_str() : "--");
		ImGui::Text("Bone: %s", m_model != nullptr && m_context.currSelectedBone >= 0 ?
			m_model->GetPMXFile().GetBoneName(m_context.currSelectedBone).c_str() : "--");

		ImGui::End();

		/* Draw overlay on viewport */
		ImGui::Begin("Viewport");
        ImVec2 min = ImGui::GetWindowContentRegionMin();
        ImVec2 max = ImGui::GetWindowContentRegionMax();
        ImVec2 size = max - min;
        ImVec2 pos = ImGui::GetWindowPos() + min;
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);

		if (m_enabled && m_model != nullptr) {
			switch (m_context.state) {
			case Context::State::PICKING:
				DrawBones();

				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && 
					!ImGui::IsKeyDown(ImGuiKey_LeftCtrl) &&
					!m_context.thisFrameClickedOnAnyBone) {
					m_context.currSelectedBone = -1;
					m_context.selectedBones.clear();
				}
				m_context.thisFrameClickedOnAnyBone = false;

				break;
			case Context::State::EDITING:
				EditTransform();
				break;
			}
		}

		ProcessKeys();
		ProcessMouseButton();

		ImGui::End();
	}
}
