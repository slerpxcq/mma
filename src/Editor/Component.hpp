#pragma once

namespace mm
{
	class Component
	{
	public:
		Component(const std::string& name) : 
			m_name(name) {}
		virtual ~Component() {}
		virtual void PropertiesPanel() = 0;

	protected:
		std::string m_name;
	};
}

