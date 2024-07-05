#pragma once

#include "GPUResource.hpp"

#include <filesystem>
#include <unordered_map>

namespace mm
{

class Shader : public GPUResource
{
public:
	Shader();
	~Shader();
	bool Compile(const std::string& source, uint32_t type);
	bool Link();
	void Use() const;

	template <typename T>
	void Uniform(const std::string& name, const T& v) { Uniform(name, 1, &v); }

	template <typename T>
	void Uniform(const std::string& name, uint32_t count, const T* v) {
		int32_t loc = GetLoc(name);
		if (loc < 0)
			return;
		UniformHelper(loc, count, v);
	}

private:
	void DeleteAllShaders();
	void AttachAllShaders();
	void LoadLocationCache();

	template <typename T>
	void UniformHelper(int32_t loc, uint32_t count, const T* v);

private:
	/* Shaders are deleted when program is linked successfully */
	std::vector<uint32_t> m_shaders;
	std::unordered_map<std::string, int32_t> m_locationCache;
};

}

