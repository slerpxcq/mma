#pragma once

#include <Core/Application.hpp>

namespace mm
{

class EditorApplication : public Application
{
public:
	EditorApplication(i32 argc, char** argv) :
		Application(argc, argv) {}

	virtual void NewFrame(float deltaTime) override;
	virtual void Startup() override;
	virtual void Shutdown() override;

private:
};

}

