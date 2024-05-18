#pragma once

#include "../Entity.hpp"

namespace mm
{
	class Light : public Entity {
	public:
		virtual ~Light() {}
		const std::string& GetName() { return m_name; }
	protected:
		std::string m_name;
	};
}
