#include "DDMImage.h"



Image::Image(const std::string& name) noexcept {
	lodepng::decode(m_data, m_width, m_height, "Textures/" + name);
}
Image::~Image() noexcept {
	m_data.clear();
}

int Image::GetWidth() noexcept {
	return m_width;
}
int Image::GetHeight() noexcept {
	return m_height;
}

std::vector<unsigned char>* Image::GetData() noexcept {
	return &m_data;
}