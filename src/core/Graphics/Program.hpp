#pragma once

#include "GPUResource.hpp"

namespace mm
{

class Shader;

class Program : public GPUResource
{
public:
	struct LinkError : public RuntimeError {
		LinkError(const char* what) : RuntimeError{ what } {}
	};

public:
	explicit Program(std::initializer_list<Ref<Shader>> shaders);
	~Program() { GetGraphics()->DeleteProgram(*this); }

private:
	void LoadLocationCache();

private:
	HashMap<String, i32> m_locationCache{};
};

}

