#pragma once

#include "Common/Config.hpp"
#include "Common/Log.hpp"
#include "InputManager.hpp"
#include "Node.hpp"

namespace mm
{

MM_GLOBAL_ATTRIBUTE(Config::GraphicsAPI, GraphicsAPI)
MM_GLOBAL_ATTRIBUTE(Scoped<Logger>, CoreLogger)
MM_GLOBAL_ATTRIBUTE(Scoped<InputManager>, InputManager)
// MM_GLOBAL_ATTRIBUTE(Scoped<Node>, RootNode)

};

