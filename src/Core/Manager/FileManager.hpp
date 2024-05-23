#pragma once

#include "Core/Utility/Singleton.hpp"

#include "File.hpp"
#include "PMXFile.hpp"
#include "VMDFile.hpp"

namespace mm
{

class FileManager : public Singleton<FileManager>
{
	friend class Singleton<FileManager>;
public:
	template <typename T>
	T* Load(const std::filesystem::path& path);

private:
	std::unordered_map<std::string, std::unique_ptr<File>> m_files;
};

template <typename T>
T* FileManager::Load(const std::filesystem::path& path)
{
	auto file = std::make_unique<T>(path);
	T* ret = file.get();
	m_files.insert({ path.string(), std::move(file) });
	MM_INFO("File loaded successfully");
	MM_INFO("- Path: {0}", path.string());
	return ret;
}

}

