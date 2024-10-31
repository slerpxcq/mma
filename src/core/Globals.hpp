#pragma once

#include "Common/Config.hpp"
#include "Common/Log.hpp"
#include "Graphics/Graphics.hpp"
// #include "Graphics/FrameBuffer.hpp"
#include "Node.hpp"
#include "InputManager.hpp"

namespace mm
{

MM_GLOBAL_ATTRIBUTE(Logger*, CoreLogger)
MM_GLOBAL_ATTRIBUTE(InputManager*, InputManager)
MM_GLOBAL_ATTRIBUTE(Graphics*, Graphics)
MM_GLOBAL_ATTRIBUTE(Node*, RootNode)
MM_GLOBAL_ATTRIBUTE(FrameBuffer*, MainFrameBuffer)

};

