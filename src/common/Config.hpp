#pragma once

#include <cstdint>

namespace mm::Config
{

static constexpr int GL_VERSION_MAJOR = 4;
static constexpr int GL_VERSION_MINOR = 6;
static constexpr const char* FONT_PATH = "../../resources/fonts/Noto_Sans_JP/static/NotoSansJP-Regular.ttf";

enum class GraphicsAPI { GL4, VK, D3D };

}



