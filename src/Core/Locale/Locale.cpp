#include "mmpch.hpp"
#include "Locale.hpp"
#include "ShiftJIS.hpp"

#include <codecvt>
#include <locale>
#include <cstring>

namespace mm
{
	std::string Locale::UTF16LEToUTF8(const char* s, size_t len)
	{
		static std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffffu, std::codecvt_mode::little_endian>, char16_t> conv;
		std::string str(conv.to_bytes(std::u16string((char16_t*)s, (char16_t*)&s[len])));
		str.push_back(0);
		return str;
	}

	// https://stackoverflow.com/questions/33165171/c-shiftjis-to-utf8-conversion
	std::string Locale::ShiftJISToUTF8(const char* s, size_t len)
	{
		std::string output(3 * len, ' '); //ShiftJis won't give 4byte UTF8, so max. 3 byte per s char are needed
		size_t indexInput = 0, indexOutput = 0;

		while (indexInput < len)
		{
			char arraySection = ((uint8_t)s[indexInput]) >> 4;

			size_t arrayOffset;
			if (arraySection == 0x8) arrayOffset = 0x100; //these are two-byte shiftjis
			else if (arraySection == 0x9) arrayOffset = 0x1100;
			else if (arraySection == 0xE) arrayOffset = 0x2100;
			else arrayOffset = 0; //this is one byte shiftjis

			//determining real array offset
			if (arrayOffset)
			{
				arrayOffset += (((uint8_t)s[indexInput]) & 0xf) << 8;
				indexInput++;
				if (indexInput >= len) break;
			}
			arrayOffset += (uint8_t)s[indexInput++];
			arrayOffset <<= 1;

			//unicode number is...
			uint16_t unicodeValue = (shiftJIS_convTable[arrayOffset] << 8) | shiftJIS_convTable[arrayOffset + 1];

			//converting to UTF8
			if (unicodeValue < 0x80)
			{
				output[indexOutput++] = unicodeValue;
			}
			else if (unicodeValue < 0x800)
			{
				output[indexOutput++] = 0xC0 | (unicodeValue >> 6);
				output[indexOutput++] = 0x80 | (unicodeValue & 0x3f);
			}
			else
			{
				output[indexOutput++] = 0xE0 | (unicodeValue >> 12);
				output[indexOutput++] = 0x80 | ((unicodeValue & 0xfff) >> 6);
				output[indexOutput++] = 0x80 | (unicodeValue & 0x3f);
			}
		}

		output.resize(indexOutput); //remove the unnecessary bytes
		output.resize(strlen(output.c_str()) + 1);
		return output;
	}
}

