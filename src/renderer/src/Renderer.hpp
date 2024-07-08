#pragma once

#include <core/src/SceneNode.hpp>

#include "GL/Buffer.hpp"

namespace mm
{

/* std430 */
struct GlobalUniformBlock
{
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 viewProjection;
};

class Renderer : public Singleton<Renderer>
{
public:
	virtual void Startup() override;
	virtual void Shutdown() override;
	void RenderScene(SceneNode& node);

private:
	void CreateGlobalUniformBuffer();

private:
	std::shared_ptr<Buffer> m_globalUniformBuffer;
};

}



