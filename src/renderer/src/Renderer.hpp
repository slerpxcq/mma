#pragma once

#include <core/src/SceneNode.hpp>

#include "GL/Buffer.hpp"
#include "GL/Shader.hpp"

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
	void RenderScene(std::shared_ptr<SceneNode> node);
	void Visit(std::shared_ptr<Node> node);

private:
	void CreateGlobalUniformBuffer();
	void LoadDefaultShader();

private:
	std::shared_ptr<Buffer> m_globalUniformBuffer;
	std::shared_ptr<Shader> m_defaultShader;
};

}



