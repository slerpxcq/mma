#pragma once

#include "Layer.hpp"

namespace mm
{

class CoreLayer : public Layer
{
public:
	virtual void OnUpdate(f32 deltaTime) override;
	virtual void OnRender() override;

private:
};

}

