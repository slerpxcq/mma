#include "EditorPch.hpp"
#include "EditorApplication.hpp"

int main()
{
	using namespace mm;
	auto app = std::make_unique<EditorApplication>();
	app->Run();

	return 0;
}