#include "mmpch.hpp"
#include "FileDialog.hpp"

#include <nfd.h>

namespace mm
{

std::filesystem::path FileDialog::Open(const char* filter)
{
	std::filesystem::path ret;
	nfdchar_t* path = nullptr;
	nfdresult_t result = NFD_OpenDialog(filter, nullptr, &path);
	if (result == NFD_OKAY) {
		ret = path;
	}
	return ret;
}

std::filesystem::path FileDialog::Save(const char* filter)
{
	std::filesystem::path ret;
	nfdchar_t* path = nullptr;
	nfdresult_t result = NFD_OpenDialog(filter, nullptr, &path);
	if (result == NFD_OKAY) {
		ret = path;
	}
	return ret;
}

}