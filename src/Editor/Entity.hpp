#pragma once

namespace mm
{
	class Entity
	{
	public:
		Entity(const std::string& name) : 
			m_name(name) {}
		virtual ~Entity() {}
		virtual void PropertiesPanel() = 0;

	protected:
		std::string m_name;
	};
}

