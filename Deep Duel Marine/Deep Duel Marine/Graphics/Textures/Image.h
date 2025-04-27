#pragma once
#include <vector>
#include <string>

#include "../../Dependencies/lodepng/lodepng.h"



class Image {
public:
	Image(const std::string& name) noexcept;
	~Image() noexcept;

	int GetWidth() noexcept;
	int GetHeight() noexcept;

	std::vector<unsigned char>* GetData() noexcept;

private:
	std::vector<unsigned char> m_data{};
	unsigned int m_width = 0;
	unsigned int m_height = 0;
};
