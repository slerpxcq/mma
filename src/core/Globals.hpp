#pragma once

#include "Common/Config.hpp"
#include "Common/Log.hpp"
#include "InputManager.hpp"

namespace mm
{

MM_GLOBAL_ATTRIBUTE(Config::GraphicsAPI, GraphicsAPI)
MM_GLOBAL_ATTRIBUTE(Logger*, CoreLogger)
MM_GLOBAL_ATTRIBUTE(InputManager*, CoreInputManager)

};

