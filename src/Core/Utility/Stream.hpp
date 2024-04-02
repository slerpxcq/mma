#pragma once

namespace mm
{
	template <typename T>
	static void Trim(T& value, std::size_t size)
	{
		if (size != sizeof(T)) {
			switch (size) {
			case 1:
				value = static_cast<int8_t>(value);
				break;
			case 2:
				value = static_cast<int16_t>(value);
				break;
			}
		}
	}

	template <typename T>
	static void Read(std::ifstream& stream, T& buffer, std::size_t size = sizeof(T))
	{
		std::memset(&buffer, 0, size);
		stream.read(reinterpret_cast<char*>(&buffer), size);
		Trim(buffer, size);
	}

	template <typename T, std::size_t N>
	static void Read(std::ifstream& stream, T(&buffer)[N], std::size_t size = sizeof(T))
	{
		for (std::size_t i = 0; i < N; ++i)
			Read(stream, buffer[i], size);
	}
}


