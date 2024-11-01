#include "CorePch.hpp"
#include "Shader.hpp"

namespace mm
{

Shader::Shader(const String& source, Graphics::ShaderType type)
{
	const auto gfx = GetGraphics();
	auto result = gfx->CreateShader(*this, source, type);
	if (!result) {
		throw CompileError(result.value().c_str());
	}
}

}
