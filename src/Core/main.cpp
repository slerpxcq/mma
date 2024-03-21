#include "Application.hpp"

int main()
{
	using namespace mm;

	auto &app = Application::Instance();

	app.Init();
	app.Run();
	app.DeInit();

	return 0;
}