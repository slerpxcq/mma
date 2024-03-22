#pragma once

namespace mm
{
	class GLTexture
	{
	public:
		GLTexture(const std::filesystem::path path, uint32_t target);
		void Bind(uint32_t active);

	private:
		uint32_t m_id;
		uint32_t m_target;
		glm::uvec2 size;
	};
}

