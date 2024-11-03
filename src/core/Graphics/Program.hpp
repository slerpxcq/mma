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
	~Program();
	void Use() const;

	template <typename T>
	bool SetUniform(StringView name, const T& val) {
		return SetUniform(name, 1, &val);
	}

	template <typename T>
	bool SetUniform(StringView name, u32 count, const T* val) {
		i32 loc = GetLocation(name);
		if (loc < 0) {
			return false;
		}
		DoSetUniform(loc, count, val);
		return true;
	}

private:
	template <typename T>
	void DoSetUniform(i32 loc, u32 count, const T* val) {
		GetGraphics()->SetUniform(*this, loc, count, val);
	}

	void LoadLocations();
	i32 GetLocation(StringView name);

private:
	HashMap<String, i32> m_locations{};
};

}

