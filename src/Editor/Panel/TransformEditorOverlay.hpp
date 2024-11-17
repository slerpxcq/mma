#pragma once

#include "PanelOverlay.hpp"

namespace mm
{

class Bone;
class TransformEditorOverlay : public PanelOverlay
{
public:
	struct GizmoContext {
		enum class Operation {
			ROTATE, ROTATE_X, 
			ROTATE_Y, ROTATE_Z, 
			ROTATE_SCREEN,
			TRANSLATE, TRANSLATE_X, 
			TRANSLATE_Y, TRANSLATE_Z 
		};
		enum class Mode { LOCAL, WORLD };
		enum class Constraint { NONE, LOCAL, FIXED };
		Bone* bone{};
		Mat4 worldToLocal{};
		Mat4 localFrame{};
		Mat4 localFrameInverse{};
		Operation operation{};
		Mode mode{};
		Constraint constraint{};
	};

	struct EditorContext {
		enum class State {
			PICKING, EDITING
		} state{};
		bool enabled{ true };
		Bone* selectedBone{};
		HashSet<Bone*> selectedBones{};
	};

public:
	TransformEditorOverlay(Panel& parent);

	void OnUpdate(f32) override;
	void OnRender() override;

private:
	Vec3 ToScreenPos(Vec3 world);
	void ComputeGizmoContext(Bone* bone);
	void ShowGizmo();
	void ProcessInput();

private:
	static constexpr f32 BUTTON_RADIUS = 7.5f;
	static constexpr u32 OUTLINE_COLOR = 0x7fc0c0c0;
	static constexpr u32 FILL_COLOR = 0x3fc0c0c0;
	static constexpr f32 OUTLINE_SIZE = 1.5f;

	GizmoContext m_gizmoContext{};
	EditorContext m_editorContext{};
};

}

