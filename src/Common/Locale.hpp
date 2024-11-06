#pragma once

#include "Container/String.hpp"
#include "Base.hpp"

namespace mm::Locale
{

String UTF16LEToUTF8(const char* s, i32 len);
String ShiftJISToUTF8(const char* s, i32 len);
String UTF8ToShiftJIS(const char* s, i32 len);

}



