#pragma once

#include "Layer.hpp"

namespace mm
{

class LayerStack
{
public:
	void OnUpdate(float deltaTime);
	void OnRender();

	template <typename T, typename... Args>
	void EmplaceBack(Args&&... args) {
		m_layers.push_back(std::make_unique<T>(std::forward<Args>(args)...));
	}

private:
	std::vector<std::unique_ptr<Layer>> m_layers;
};

}

