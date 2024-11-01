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
	auto msg = gfx->LinkProgram(*this); 
	if (!msg) {
		throw LinkError{ msg.value().c_str() };
	}
	LoadLocationCache();
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

i32 Program::GetLocation(StringView name)
{
	auto it = m_locationCache.find(String{ name });
	if (it != m_locationCache.end()) {
		return it->second;
	} else {
		return -1;
	}
}

}
