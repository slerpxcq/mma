#include "mmpch.hpp"
#include "Locale.hpp"

#include <codecvt>
#include <locale>
#include <cstring>

#include <unicode/unistr.h>

namespace mm
{
	std::string Locale::UTF16LEToUTF8(const char* s, size_t len)
	{
		static std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffffu, std::codecvt_mode::little_endian>, char16_t> conv;
		std::string str(conv.to_bytes(std::u16string((char16_t*)s, (char16_t*)&s[len])));
		return str;
	}

	/* Include null terminator */
	/* https://gist.github.com/kilfu0701/e279e35372066ae1832850c438d5611e */
	std::string Locale::ShiftJISToUTF8(const char* s, size_t len)
	{
		std::string sjfstr(s, s + len);
		icu::UnicodeString src(sjfstr.c_str(), "shift_jis");
		int length = src.extract(0, src.length(), NULL, "utf8");

		std::vector<char> result(length + 1);
		src.extract(0, src.length(), &result[0], "utf8");

		return std::string(result.begin(), result.end() - 1).append("");
	}

	std::string Locale::UTF8ToShiftJIS(const std::string& u8string)
	{
		icu::UnicodeString src(u8string.c_str(), "utf8");
		int length = src.extract(0, src.length(), NULL, "shift_jis");

		std::vector<char> result(length + 1);
		src.extract(0, src.length(), &result[0], "shift_jis");

		return std::string(result.begin(), result.end() - 1).append("");
	}
}

