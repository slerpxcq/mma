#pragma once

#include "Core/MM/Math/Transform.hpp" 

#include "Core/MM/Animation/Animation.hpp"

namespace mm
{
	class Command {
	public:
		virtual void Undo() = 0;
		virtual void Redo() = 0;
		virtual ~Command() {}
	};


}
