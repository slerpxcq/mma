#pragma once

namespace mm
{

class Panel
{
public:
	virtual ~Panel() = default;
	virtual void OnUpdate(f32 deltaTime) = 0; 
	virtual void OnRender() = 0;

private:
};

}

