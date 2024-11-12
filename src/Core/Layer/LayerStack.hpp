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
	void Push(Args&&... args) {
		m_layers.push_back(MakeScoped<T>(std::forward<Args>(args)...));
	}

private:
	DynArray<Scoped<Layer>> m_layers;
};

}

