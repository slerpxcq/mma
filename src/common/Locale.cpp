#include "Locale.hpp"

#include <codecvt>
#include <locale>
#include <cstring>
#include <vector>

#include <unicode/unistr.h>

namespace mm::Locale
{

String UTF16LEToUTF8(const char* s, size_t len)
{
	static std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffffu, std::codecvt_mode::little_endian>, char16_t> conv;
	std::string str(conv.to_bytes(std::u16string((char16_t*)s, (char16_t*)&s[len])));
	return str;
}

/* Include null terminator */
/* https://gist.github.com/kilfu0701/e279e35372066ae1832850c438d5611e */
String ShiftJISToUTF8(const char* s, size_t len)
{
	std::string sjfstr(s, s + len);
	icu::UnicodeString src(sjfstr.c_str(), "shift_jis");
	int length = src.extract(0, src.length(), NULL, "utf8");

	std::vector<char> result(length + 1);
	src.extract(0, src.length(), &result[0], "utf8");

	return std::string(result.begin(), result.end() - 1).append("");
}

String UTF8ToShiftJIS(const char* s, size_t len)
{
	icu::UnicodeString src(s, "utf8");
	int length = src.extract(0, src.length(), NULL, "shift_jis");

	std::vector<char> result(length + 1);
	src.extract(0, src.length(), &result[0], "shift_jis");

	return std::string(result.begin(), result.end() - 1).append("");
}

}


