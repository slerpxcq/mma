#pragma once

namespace mm
{

class Panel
{
public:
	/* Should be called by derived class */
	virtual void OnUIRender();
	virtual void OnUpdate() {}

protected:
	bool m_opened = false;
	bool m_hovered = false;
	bool m_focused = false;
	bool m_resized = false;

	glm::uvec2 m_windowSize { 0, 0 };
	glm::uvec2 m_contentMax;
	glm::uvec2 m_contentMin;
	glm::uvec2 m_contentSize { 0, 0 };
	glm::uvec2 m_contentPos;

	std::string m_name;
};

}

