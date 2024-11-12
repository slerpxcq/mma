#pragma once

#include "Common/Config.hpp"
#include "Common/Log.hpp"

#include "../Graphics/Program.hpp"
#include "../Graphics/Texture.hpp"

namespace mm
{

class SceneManager;
class Renderer;
class InputManager;
class Graphics;
class Node;
class Viewport;
class InverseKinematicsSolver;

MM_GLOBAL_OBJECT(Logger*, CoreLogger)
MM_GLOBAL_OBJECT(Logger*, AppLogger)
MM_GLOBAL_OBJECT(InputManager*, InputManager)
MM_GLOBAL_OBJECT(Graphics*, Graphics)
MM_GLOBAL_OBJECT(Viewport*, MainViewport)
/* BEGIN TEST CODE */
MM_GLOBAL_OBJECT(SceneManager*, SceneManager)
MM_GLOBAL_OBJECT(Renderer*, Renderer)
MM_GLOBAL_OBJECT(InverseKinematicsSolver*, InverseKinematicsSolver);
// MM_GLOBAL_OBJECT(Ref<Program>, DefaultProgram)
// MM_GLOBAL_OBJECT(DynArray<Ref<Texture>>, DefaultTextures)
/* END TEST CODE */

};

