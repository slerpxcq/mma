#pragma once

#include "Core/Application/Application.hpp"

namespace mm
{

class EditorApplication : public Application
{
public:
	EditorApplication(i32 argc, char** argv) :
		Application(argc, argv) {}

	virtual void OnNewFrame(f32 deltaTime) override;
	virtual void OnUpdate(f32 deltaTime) override;
	virtual void Startup() override;
	virtual void Shutdown() override;

private:
};

}

