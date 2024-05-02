#pragma once

namespace mm
{
	class Light 
	{
	public:
		virtual ~Light() {}
		const std::string& GetName() { return m_name; }

	protected:
		std::string m_name;
	};
}
