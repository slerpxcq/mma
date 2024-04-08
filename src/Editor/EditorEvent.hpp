#pragma once

#include <string>

namespace mm
{
	class Model;

	namespace EditorEvent
	{
		struct ModelLoaded {
			Model* model;
		};

		struct MotionLoaded {
		};
	}
}
