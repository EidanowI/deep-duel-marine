#include "Texture.h"



Texture::Texture(std::string file_name) noexcept {
	Image* pImage = new Image(file_name);

	D3D11_TEXTURE2D_DESC textureDesc{};
	{
		textureDesc.Width = pImage->GetWidth();
		textureDesc.Height = pImage->GetHeight();
		textureDesc.MipLevels = 1u;
		textureDesc.ArraySize = 1u;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1u;
		textureDesc.SampleDesc.Quality = 0u;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0u;
		textureDesc.MiscFlags = 0u;
	}
	D3D11_SUBRESOURCE_DATA subresData{};
	{
		subresData.pSysMem = pImage->GetData();
		subresData.SysMemPitch = pImage->GetWidth() * 4;
	}
	
	Renderer::GetDevice()->CreateTexture2D(&textureDesc, &subresData, &m_pTexture);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResViewDesc = {};
	{
		shaderResViewDesc.Format = textureDesc.Format;
		shaderResViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResViewDesc.Texture2D.MipLevels = 1;
	}
	Renderer::GetDevice()->CreateShaderResourceView(
		m_pTexture, &shaderResViewDesc, &m_pShader_resource_view
	);
}
Texture::~Texture() noexcept {
	if (m_pShader_resource_view) {
		m_pShader_resource_view->Release();
		m_pShader_resource_view = nullptr;
	}

	if (m_pTexture) {
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

void Texture::Bind() {
	Bind(0);
}
void Texture::Bind(int bind_index) {
	Renderer::GetDeviceContext()->PSSetShaderResources(bind_index, 1u, &m_pShader_resource_view);
}
