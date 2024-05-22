#include "mmpch.hpp"
#include "Application.hpp"

int main()
{
	using namespace mm;
	Application::Init();
	Application::Get().Run();
	Application::DeInit();
	return 0;
}