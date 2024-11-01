#include "CorePch.hpp"
#include "Program.hpp"

namespace mm
{

Program::Program(std::initializer_list<Ref<Shader>> shaders)
{
	const auto gfx = GetGraphics();
	gfx->CreateProgram(*this);
	for (const auto& shader : shaders) {
		gfx->AttachShader(*this, *shader);
	}
	gfx->LinkProgram(*this);
}

void Program::LoadLocationCache()
{
	const auto gfx = GetGraphics();
	u32 count = gfx->GetUniformCount(*this);
	for (u32 i = 0; i < count; ++i) {
		String name = gfx->GetUniformName(*this, i);
		i32 loc = gfx->GetUniformLocation(*this, name);
		if (loc >= 0) {
			m_locationCache.insert({ name, loc });
		}
	}
}

}
