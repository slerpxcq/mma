#include "CorePch.hpp"
#include "Program.hpp"

namespace mm
{

Program::Program(InitList<Ref<Shader>> shaders)
{
	auto gfx = GetGraphics();
	gfx->CreateProgram(*this);
	for (const auto& shader : shaders) {
		gfx->AttachShader(*this, *shader);
	}
	auto msg = gfx->LinkProgram(*this); 
	if (msg) {
		throw LinkError{ msg.value().c_str() };
	}
	LoadLocations();
}

void Program::LoadLocations()
{
	auto gfx = GetGraphics();
	u32 count = gfx->GetUniformCount(*this);
	for (u32 i = 0; i < count; ++i) {
		String name = gfx->GetUniformName(*this, i);
		i32 loc = gfx->GetUniformLocation(*this, name);
		if (loc >= 0) {
			m_locations.insert({ name, loc });
		}
	}
}

i32 Program::GetLocation(StringView name)
{
	auto it = m_locations.find(String{ name });
	if (it != m_locations.end()) {
		return it->second;
	} else {
		return -1;
	}
}

}
