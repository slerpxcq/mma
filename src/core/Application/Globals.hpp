#pragma once

#include "Common/Config.hpp"
#include "Common/Log.hpp"

#include "../Graphics/Program.hpp"
#include "../Graphics/Texture.hpp"

namespace mm
{

class SceneManager;
class InputManager;
class Graphics;
class Node;
class FrameBuffer;

MM_GLOBAL_OBJECT(Logger*, CoreLogger)
MM_GLOBAL_OBJECT(Logger*, AppLogger)
MM_GLOBAL_OBJECT(InputManager*, InputManager)
MM_GLOBAL_OBJECT(Graphics*, Graphics)
// MM_GLOBAL_OBJECT(Node*, RootNode)
MM_GLOBAL_OBJECT(FrameBuffer*, MainFrameBuffer)
/* BEGIN TEST CODE */
MM_GLOBAL_OBJECT(SceneManager*, SceneManager)
MM_GLOBAL_OBJECT(Ref<Program>, DefaultProgram)
MM_GLOBAL_OBJECT(DynArray<Ref<Texture>>, DefaultTextures)
/* END TEST CODE */

};

