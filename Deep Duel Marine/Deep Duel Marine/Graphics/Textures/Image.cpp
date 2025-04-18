#include "Image.h"



Image::Image(const std::string& name) noexcept {
	lodepng::decode(m_data, m_width, m_height, "Textures/" + name);
}
Image::~Image() {
	m_data.clear();
}

int Image::GetWidth() {
	return m_width;
}
int Image::GetHeight() {
	return m_height;
}

std::vector<unsigned char>* Image::GetData() {
	return &m_data;
}