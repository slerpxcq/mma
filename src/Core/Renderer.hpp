#pragma once

#include "Graphics/Program.hpp"
#include "Graphics/Texture.hpp"

namespace mm
{

class SubMesh;
class Grid;

class Renderer
{
public:
	void LoadInternalTextures();
	void LoadDefaultProgram();
	void BeginFrame();
	void EndFrame();
	const auto& GetInternalTextures() const { return m_internalTextures;  }
	Ref<Program> GetDefaultProgram() const { return m_defaultProgram; }

	void Render(SubMesh&);
	void Render(Grid&);

private:
	Ref<Program> m_defaultProgram{};
	DynArray<Ref<Texture>> m_internalTextures{};
};

}

