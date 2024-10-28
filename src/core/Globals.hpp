#pragma once

#include "Common/Config.hpp"
#include "Common/Log.hpp"
// #include "Graphics/GPUResourceFactory.hpp"
#include "Graphics/Graphics.hpp"
#include "InputManager.hpp"

namespace mm
{

// MM_GLOBAL_ATTRIBUTE(Config::API, GraphicsAPI)
MM_GLOBAL_ATTRIBUTE(Scoped<Logger>, CoreLogger)
MM_GLOBAL_ATTRIBUTE(Scoped<InputManager>, InputManager)
MM_GLOBAL_ATTRIBUTE(Scoped<Graphics>, Graphics)
// MM_GLOBAL_ATTRIBUTE(Scoped<GPUResourceFactory>, GPUResourceFactory)

};

