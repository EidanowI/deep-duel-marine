#include "Shader.h"
#include "../Renderer/Renderer.h"



unsigned int Hash(const std::string& a) {
	unsigned int hash = 5381;
	for (int i = 0; i < a.size(); i++) {
		hash = ((hash << 5) + hash) + a[i];
	}

	return hash;
}


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


std::map<unsigned int, VertexShader*> ShaderManager::s_vertexShaders{};
std::map<unsigned int, PixelShader*> ShaderManager::s_pixelShaders{};

void ShaderManager::Initialize() noexcept {
	///TODO precompiling some shaders for faster loading
}
void ShaderManager::Terminate() noexcept {
	for (const auto& [v_id, v_pShader] : s_vertexShaders) {
		delete v_pShader;
	}

	for (const auto& [p_id, p_pShader] : s_pixelShaders) {
		delete p_pShader;
	}
}

VertexShader* ShaderManager::GetVertexShader(const std::string& name) noexcept {
	unsigned int hash = Hash(name);

	if (s_vertexShaders.contains(hash)) {
		return s_vertexShaders[hash];
	}
	else {
		auto pVShader = new VertexShader(name);
		s_vertexShaders[hash] = pVShader;

		return pVShader;
	}
}
PixelShader* ShaderManager::GetPixelShader(const std::string& name) noexcept {
	unsigned int hash = Hash(name);

	if (s_pixelShaders.contains(hash)) {
		return s_pixelShaders[hash];
	}
	else {
		auto pPShader = new PixelShader(name);
		s_pixelShaders[hash] = pPShader;
		return pPShader;
	}
}