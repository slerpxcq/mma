#include "CorePch.hpp"
#include "Shader.hpp"

namespace mm
{

Shader::Shader(const String& source, Graphics::ShaderType type)
{
	const auto gfx = GetGraphics();
	gfx->CreateShader(*this, type);
	gfx->ShaderSource(*this, source);
	if (!gfx->CompileShader(*this)) {
		String msg = gfx->GetCompileError(*this);
		throw CompileError()
	}
}

}
