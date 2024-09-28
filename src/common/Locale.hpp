#pragma once

#include "Container/String.hpp"

namespace mm::Locale
{

String UTF16LEToUTF8(const char* s, size_t len);
String ShiftJISToUTF8(const char* s, size_t len);
String UTF8ToShiftJIS(const char* s, size_t len);

}



