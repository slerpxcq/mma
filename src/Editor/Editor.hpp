#pragma once

#include "Core/Utility/Singleton.hpp"

namespace mm
{

class Editor : public Singleton<Editor> 
{
	friend class Singleton<Editor>;
public:
	void OnUpdate(float deltaTime);
	void OnUIRender();

private:
	Editor() {}
};

}

