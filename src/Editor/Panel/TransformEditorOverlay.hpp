#pragma once

#include "PanelOverlay.hpp"

namespace mm
{

class Bone;
class TransformEditorOverlay : public PanelOverlay
{
public:
	enum class Operation {
		ROTATE, ROTATE_X, ROTATE_Y, ROTATE_Z, ROTATE_SCREEN,
		TRANSLATE, TRANSLATE_X, TRANSLATE_Y, TRANSLATE_Z, TRANSLATE_SCREEN
	};
	enum class Mode {
		LOCAL, WORLD
	};
	struct Context {
		Bone* bone{};
		Mat4 worldToLocal{};
		Mat4 localFrame{};
		Mat4 localFrameInverse{};
		Opt<Vec3> fixedAxis{};
		Opt<Mat4> localAxes{};
		Operation operation{};
		Mode mode{};
	};

public:
	TransformEditorOverlay(Panel& parent, StringView name);

	void OnUpdate(f32) override;
	void OnRender() override;

private:
	Vec3 ToScreenPos(Vec3 worldPos);
	Vec2 GetTipPos(Bone* bone);
	void ShowGizmo();
	void OnBoneSelected(Bone* bone);

private:
	static constexpr f32 BUTTON_RADIUS = 7.5f;
	static constexpr u32 OUTLINE_COLOR = 0x7fc0c0c0;
	static constexpr u32 FILL_COLOR = 0x3fc0c0c0;
	static constexpr f32 OUTLINE_SIZE = 1.5f;

	Context m_context{};
};

}

