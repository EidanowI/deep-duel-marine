#pragma once
#include <vector>
#include <string>

#include "../../Dependencies/lodepng/lodepng.h"



class Image {
public:
	Image(const std::string& name) noexcept;
	~Image();

	int GetWidth();
	int GetHeight();

	std::vector<unsigned char>* GetData();

private:
	std::vector<unsigned char> m_data{};
	unsigned int m_width = 0;
	unsigned int m_height = 0;
};
