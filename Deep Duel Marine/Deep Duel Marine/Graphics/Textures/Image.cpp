#include "Image.h"



Image::Image(const std::string& name) noexcept {
	lodepng::decode(m_buffer, m_width, m_height, "Textures/" + name);
}
Image::~Image() {
	m_buffer.clear();
}