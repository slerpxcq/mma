#pragma once

namespace mm
{
	class GLTexture
	{
	public:
		GLTexture(uint32_t target);
		GLTexture(const GLTexture&) = delete;
		GLTexture(GLTexture&&) = default;

		uint32_t GetId() const { return m_id; }
		void Bind(uint32_t unit) const;
		void Parameteri(uint32_t name, uint32_t param);

		virtual ~GLTexture();

	protected:
		uint32_t m_id = 0;
		uint32_t m_target = 0;
	};
}

