#include "Shader.h"
#include "../Renderer/Renderer.h"



VertexShader::VertexShader(const std::string& name) noexcept {
	std::ifstream ifs("Shaders/" + name, std::ios_base::binary | std::ios_base::ate);

	if (ifs.is_open()) {
		int shader_file_biteSize = ifs.tellg();
		ifs.seekg(0);

		shader_sourceCode = std::vector<char>(shader_file_biteSize);
		ifs.read(shader_sourceCode.data(), shader_file_biteSize);

		Renderer::GetDevice()->CreateVertexShader(shader_sourceCode.data(), shader_file_biteSize, 0, &pVertex_shader);
	}

	ifs.close();
}
VertexShader::~VertexShader() noexcept {
	shader_sourceCode.clear();
	if (pVertex_shader) {
		pVertex_shader->Release();
	}	
}

void VertexShader::Bind() noexcept {
	Renderer::GetDeviceContext()->VSSetShader(pVertex_shader, nullptr, NULL);
}


PixelShader::PixelShader(const std::string& name) noexcept {
	std::ifstream ifs("Shaders/" + name, std::ios_base::binary | std::ios_base::ate);

	if (ifs.is_open()) {
		int shader_file_biteSize = ifs.tellg();
		ifs.seekg(0);

		shader_sourceCode = std::vector<char>(shader_file_biteSize);
		ifs.read(shader_sourceCode.data(), shader_file_biteSize);

		Renderer::GetDevice()->CreatePixelShader(shader_sourceCode.data(), shader_file_biteSize, 0, &pPixel_shader);
	}

	ifs.close();
}
PixelShader::~PixelShader() noexcept {
	shader_sourceCode.clear();
	if (pPixel_shader) {
		pPixel_shader->Release();
	}
}

void PixelShader::Bind() noexcept {
	Renderer::GetDeviceContext()->PSSetShader(pPixel_shader, nullptr, NULL);
}