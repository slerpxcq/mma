#pragma once

namespace mm
{

class Layer
{
public:
	virtual void OnUpdate(float deltaTime) {}
	virtual void OnRender() {}
	virtual	~Layer() {}

private:
};

}

