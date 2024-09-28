#include "EditorPch.hpp"
#include "EditorApplication.hpp"

int main()
{
	return std::make_unique<mm::EditorApplication>()->Run();
}