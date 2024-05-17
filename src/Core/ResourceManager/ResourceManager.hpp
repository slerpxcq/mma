#pragma once

#include "Library.hpp"

#include "Core/GL/GLTexture.hpp"
#include "Core/GL/GLShader.hpp"
#include "Core/MM/Renderer/Effect.hpp"
#include "Core/MM/Files/File.hpp"

namespace mm
{
	class ResourceManager
	{
	public:
		/* Reture nullptr if the resource does not exist */
		GLTexture* GetTexture(const std::string& name) { return m_textures[name]; }
		GLShader* GetShader(const std::string& name) { return m_shaders[name]; }
		Effect* GetEffect(const std::string& name) { return m_effects[name]; }

		template <typename T, typename... Args>
		T* LoadTexture(const std::string& name, Args&&... args) { 
			auto* ptr = m_textures.Get(name);
			if (ptr)
				return dynamic_cast<T*>(ptr);

			auto tex = std::make_unique<T>(std::forward<Args>(args)...);
			return dynamic_cast<T*>(m_textures.Insert(name, std::move(tex))); 
		}

		GLShader* LoadShader(const std::string& name) { 
			auto shader = std::make_unique<GLShader>();
			return m_shaders.Insert(name, std::move(shader)); 
		}

		Effect* LoadEffect(const std::filesystem::path& path) { 
			auto effect = std::make_unique<Effect>(path);
			std::string name = effect->GetName();
			return m_effects.Insert(name, std::move(effect)); 
		}

		template <typename T>
		T* LoadFile(const std::filesystem::path& path) {
			auto* ptr = m_files.Get(path.string());
			if (ptr)
				return dynamic_cast<T*>(ptr);

			auto file = std::make_unique<T>(path);
			return dynamic_cast<T*>(m_files.Insert(path.string(), std::move(file)));
		}

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


