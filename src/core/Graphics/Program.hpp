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
	explicit Program(InitList<Ref<Shader>> shaders);
	~Program() { GetGraphics()->DeleteProgram(*this); }

	template <typename T>
	void SetUniform(StringView name, const T& val) {
		SetUniform(name, 1, &val);
	}

	template <typename T>
	void SetUniform(StringView name, u32 count, const T* val) {
		i32 loc = GetLocation(name);
		if (loc < 0) {
			return;
		}
		SetUniform(loc, count, val);
	}

	template <typename T>
	void SetUniform(i32 loc, u32 count, const T* val) {
		GetGraphics()->SetUniform(*this, loc, count, val);
	}

private:
	void LoadLocations();
	i32 GetLocation(StringView name);

private:
	HashMap<String, i32> m_locations{};
};

}

