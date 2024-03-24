#pragma once

#include <cstdint>
#include <string>

namespace mm
{
	class Locale
	{
	public:
		Locale() = delete;

		static std::string UTF16LEToUTF8(const char* s, size_t len);
		static std::string ShiftJISToUTF8(const char* s, size_t len);
	};
}

