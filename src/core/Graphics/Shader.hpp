#pragma once

#include "GPUResource.hpp"

namespace mm
{

class Shader : public GPUResource
{
public:
	struct CompileError : public RuntimeError {
		CompileError(const char* what) : RuntimeError{ what } {}
	};

public:
	Shader(StringView source, Graphics::ShaderType type);
	~Shader() { GetGraphics()->DeleteShader(*this); }

private:
};

}

