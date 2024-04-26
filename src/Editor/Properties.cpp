#include "mmpch.hpp"
#include "Properties.hpp"

#include "Core/MM/Model/Skin.hpp"
#include "Core/MM/Model/Armature.hpp"

#include "EditorLayer.hpp"
#include "Sequencer.hpp"

namespace mm
{
	Properties::Properties(EditorLayer& editor) : 
		m_editor(editor),
		m_listener(EventBus::Instance())

	{
		m_listener.listen<EditorEvent::ItemSelected>(MM_EVENT_FN(Properties::OnItemSelected));
	}

	void Properties::MorphSliders(Model& model, uint32_t panel)
	{
		auto& morph = model.GetMorph();
		auto& pmxMorphs = model.GetPMXFile().GetMorphs();
		uint32_t morphCount = morph.GetWeights().size();

		/* if last frame not active and this frame active, store undo value */
		/* if last frame active and this frame not active, store redo value and issue command */

		for (uint32_t morphIndex = 0; morphIndex < morphCount; ++morphIndex) {
			float* valuePtr = &morph.GetWeights()[morphIndex];
			if (pmxMorphs[morphIndex].panel == panel) {
				const std::string& name = model.GetPMXFile().GetMorphName(morphIndex);
				float valueBeforeEdit = *valuePtr;
				ImGui::SliderFloat(name.c_str(), valuePtr, 0.0f, 1.0f);

				static float undoValue;
				if (ImGui::IsItemActivated()) {
					undoValue = valueBeforeEdit;
					MM_INFO("Start edit; undo={0}", undoValue);
				}
				if (ImGui::IsItemDeactivatedAfterEdit()) {
					MM_INFO("End edit: undo={0}, redo={1}", undoValue, *valuePtr);
					/* If there is no keyframe at current frame, create one */
					EventBus::Instance()->postpone<EditorEvent::CommandIssued>({
						new Command::MorphEdited(valuePtr, *valuePtr, undoValue) });

					/* If there is no keyframe at current frame, create one */
					/* Else directly change the value of that keyframe */
					Animation& anim = model.GetAnim();
					auto& morphKeyframes = anim.GetMorphKeyframeMatrix()[morphIndex];
					uint32_t currFrame = m_editor.m_sequencer->GetFrameCounter().GetFrame();
					auto it = std::find_if(morphKeyframes.begin(), morphKeyframes.end(),
						[currFrame](const Animation::Keyframe& keyframe) {
							return keyframe.frame == currFrame;
						});

					if (it == morphKeyframes.end()) {
						anim.InsertMorphKeyframe(
							morphIndex,
							Animation::MorphKeyframe(currFrame, *valuePtr));
						EventBus::Instance()->postpone<EditorEvent::CommandIssued>({
							new Command::KeyframeInserted(
								anim, Command::KeyframeInserted::TYPE_MORPH, morphIndex, currFrame) });
					}
					else {
						it->weight = *valuePtr;
					}
				}
			}
		}
	}

	void Properties::OnUIRender()
	{
		ImGui::Begin("Properties");
		if (m_item != nullptr) {
			switch (m_type) {
			case TYPE_MESH:
				{
					Skin::Mesh& mesh = *(Skin::Mesh*)m_item;
					ImGui::InputFloat3("Diffuse", glm::value_ptr(mesh.material.diffuse));
					ImGui::InputFloat4("Specular", glm::value_ptr(mesh.material.specular));
					ImGui::InputFloat3("Ambient", glm::value_ptr(mesh.material.ambient));
					ImGui::InputFloat3("Edge", glm::value_ptr(mesh.material.edge));
					ImGui::InputFloat("Edge size", &mesh.material.edgeSize);
				}
				break;
			case TYPE_BONE:
				{
					//Armature::Bone& bone = *(Armature::Bone*)m_item;
					Transform& transform = *(Transform*)m_item;
					static glm::vec3 euler;
					euler = glm::eulerAngles(transform.rotation) * glm::degrees(1.f);
					ImGui::InputFloat3("Rotation", glm::value_ptr(euler));
					ImGui::InputFloat3("Translation", glm::value_ptr(transform.translation));
				}
				break;
			case TYPE_MORPH:
				{
					Model& model = *(Model*)m_item;
					if (ImGui::TreeNode("Eye")) {
						MorphSliders(model, PMXFile::PANEL_EYE);
						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Eyebrow")) {
						MorphSliders(model, PMXFile::PANEL_EYEBROW);
						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Lip")) {
						MorphSliders(model, PMXFile::PANEL_MOUTH);
						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Other")) {
						MorphSliders(model, PMXFile::PANEL_OTHER);
						ImGui::TreePop();
					}
				}
				break;
			case TYPE_ANIMATION:
				{
					Animation& anim = *(Animation*)m_item;
					ImGui::Text("Animation name: %s", anim.GetName().c_str());
					if (ImGui::Button("Load")) {

					}
				}
				break;
			default:
				MM_ASSERT(0 && "Unknown type");
			}
		}
		ImGui::End();
	}

	void Properties::OnItemSelected(const EditorEvent::ItemSelected& e)
	{
		MM_INFO("{0}", __FUNCTION__);
		m_item = e.item;
		m_type = e.type;
	}
}
