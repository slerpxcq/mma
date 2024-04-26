#pragma once

#include "Core/MM/Transform.hpp" 

#include "Commands.hpp"

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

		struct CommandIssued {
			ICommand* command;
		};

		struct ItemSelected {
			uint8_t type;
			uint32_t index;
			void* item;
		};
	}
}
