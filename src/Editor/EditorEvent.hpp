#pragma once

#include "Core/MM/Transform.hpp" 

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

		//struct CommandIssued {
		//	std::unique_ptr<Command> command;
		//};
	}
}
