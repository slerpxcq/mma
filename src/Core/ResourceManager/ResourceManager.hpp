#pragma once

#include "Library.hpp"

#include "Core/GL/GLTexture.hpp"
#include "Core/GL/GLShader.hpp"
#include "Core/MM/Files/File.hpp"
#include "Core/MM/Renderer/Effect.hpp"

namespace mm
{
	class ResourceManager
	{
	public:
		/* Reture nullptr if the resource does not exist */
		GLTexture* GetTexture(const std::string& name) { return m_textures[name]; }
		GLShader* GetShader(const std::string& name) { return m_shaders[name]; }
		Effect* GetEffect(const std::string& name) { return m_effects[name]; }
		void LoadTexture(const std::string& name, std::unique_ptr<GLTexture> tex) { m_textures.Insert(name, std::move(tex)); }
		void LoadShader(const std::string& name, std::unique_ptr<GLShader> shader) { m_shaders.Insert(name, std::move(shader)); }
		void LoadFile(std::unique_ptr<File> file) { 
			const std::string& name = file->GetName();
			m_files.Insert(name, std::move(file)); 
		}
		void LoadEffect(std::unique_ptr<Effect> effect) { 
			const std::string& name = effect->GetName();
			m_effects.Insert(name, std::move(effect)); 
		}

		//.template <typename T, typename... Args>
		//.T* Load(const std::string& name, Args&&... args) {
		//.	std::forward<Args>(args...);
		//.}

	public:
		static ResourceManager& Instance() { return s_instance; }

	private:
		static ResourceManager s_instance;

	private:
		//Library<std::string, Resource> m_resources;

		Library<std::string, GLTexture> m_textures;
		Library<std::string, GLShader> m_shaders;
		Library<std::string, File> m_files;
		Library<std::string, Effect> m_effects;
	};
}


