#include "mmpch.hpp"
#include "Application.hpp"

int main()
{
	using namespace mm;

	Application& app = Application::Instance();

	app.Init();
	app.Run();
	app.DeInit();

	return 0;
}