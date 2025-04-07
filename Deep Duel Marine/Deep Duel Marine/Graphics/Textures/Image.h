#pragma once
#include <vector>
#include <string>

#include "../../Dependencies/lodepng/lodepng.h"



class Image {
public:
	Image(const std::string& name) noexcept;
	~Image();

private:
	std::vector<unsigned char> m_buffer{};
	unsigned int m_width = 0;
	unsigned int m_height = 0;
};
