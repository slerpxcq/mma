#pragma once

#include <string>

namespace mm
{
	class Model;
	class Animation;

	namespace EditorEvent
	{
		struct ModelLoaded {
			Model* model;
		};

		struct MotionLoaded {
			Animation* animation;
		};
	}
}
