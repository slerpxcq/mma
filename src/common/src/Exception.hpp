#pragma once

#include <stdexcept>

namespace mm
{

struct MMException : public std::runtime_error
{
	MMException(const char* what) : std::runtime_error(what) {}
};

}

