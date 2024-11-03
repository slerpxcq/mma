#pragma once

#include "Common/Config.hpp"
#include "Common/Log.hpp"

// #include "../Graphics/Program.hpp"
#include "../Graphics/Graphics.hpp"
#include "InputManager.hpp"

#include "../Node.hpp"

namespace mm
{

MM_GLOBAL_OBJECT(Logger*, CoreLogger)
MM_GLOBAL_OBJECT(Logger*, AppLogger)
MM_GLOBAL_OBJECT(InputManager*, InputManager)
MM_GLOBAL_OBJECT(Graphics*, Graphics)
MM_GLOBAL_OBJECT(Node*, RootNode)
MM_GLOBAL_OBJECT(FrameBuffer*, MainFrameBuffer)
/* BEGIN TEST CODE */
MM_GLOBAL_OBJECT(Ref<Program>, DefaultProgram);
MM_GLOBAL_OBJECT(DynArray<Ref<Texture>>, DefaultTextures);
/* END TEST CODE */

};

