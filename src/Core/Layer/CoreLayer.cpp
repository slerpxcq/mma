#include "CorePch.hpp"
#include "CoreLayer.hpp"

#include "../Graphics/Graphics.hpp"

/* BEGIN TEST INCLUDE */
#include "../Node.hpp"
#include "../Model.hpp"
#include "../Graphics/Program.hpp"
#include "../Graphics/FrameBuffer.hpp"
#include "SceneManager.hpp"
#include "Physics/PhysicsManager.hpp"

#include <glm/gtc/matrix_transform.hpp>
/* END TEST INCLUDE */

namespace mm
{

void CoreLayer::OnUpdate(f32 deltaTime)
{
	GetPhysicsManager()->StepSimulation(deltaTime);
	GetSceneManager()->Update(deltaTime);
}

void CoreLayer::OnRender()
{
	GetSceneManager()->Render();
}

}
