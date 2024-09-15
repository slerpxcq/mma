#pragma once

#include <dexode/EventBus.hpp>

#include <core/src/Application.hpp>

namespace mm
{

class EditorApplication : public Application
{
public:
	//virtual void NewFrame(float deltaTime) override;
	virtual void Startup() override;
	virtual void Shutdown() override;

private:
};

}

