#pragma once

namespace mm
{
	class GLTexture
	{
	public:
		GLTexture(const std::filesystem::path path, uint32_t target);
		void Bind(uint32_t active);
		void SetWrap(uint32_t wrapS, uint32_t wrapT);
		void SetFilter(uint32_t min, uint32_t mag);

	private:
		uint32_t m_id;
		uint32_t m_target;
		glm::uvec2 m_size;
	};
}

