#include "Camera.h"



Camera::Camera(Vector3D position, Vector3D rotation) noexcept : m_position(position), m_rotation(rotation) {
	m_rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	m_constBuf0.projection = DirectX::XMMatrixPerspectiveFovLH(1.0f, (float)MainWindow::GetWindowWidth() / (float)MainWindow::GetWindowHeight(), 0.1f, 2000.0f);
	
	UpdateDirectionVectors();
	UpdateViewMatrix();

	m_pConstBuf0 = Renderer::CreateConstBuffer((char*)&m_constBuf0, sizeof(ConstBuf0));
}
Camera::~Camera() noexcept {
	if (m_pConstBuf0) {
		m_pConstBuf0->Release();
		m_pConstBuf0 = nullptr;
	}
}

void Camera::SetPosition(Vector3D position) noexcept {
	m_position = position;

	UpdateConstBuf0();
}
void Camera::SetRotation(Vector3D rotation) noexcept {
	m_rotation = rotation;
	m_rotation_matrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	UpdateConstBuf0();
}

void Camera::UpdateDirectionVectors() noexcept {
	DirectX::XMVECTOR tmp = DirectX::XMVector4Transform(DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f), m_rotation_matrix);
	m_right = Vector3D(DirectX::XMVectorGetX(tmp), DirectX::XMVectorGetY(tmp), DirectX::XMVectorGetZ(tmp));

	tmp = DirectX::XMVector4Transform(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), m_rotation_matrix);
	m_up = Vector3D(DirectX::XMVectorGetX(tmp), DirectX::XMVectorGetY(tmp), DirectX::XMVectorGetZ(tmp));

	tmp = DirectX::XMVector4Transform(DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), m_rotation_matrix);
	m_forward = Vector3D(DirectX::XMVectorGetX(tmp), DirectX::XMVectorGetY(tmp), DirectX::XMVectorGetZ(tmp));
}
void Camera::UpdateViewMatrix() noexcept {
	Vector3D lookDir = m_position + m_forward;
	DirectX::XMVECTOR lookTarget = DirectX::XMVectorSet(
		lookDir.x, lookDir.y, lookDir.z,
		0.0f)
		;
	m_constBuf0.view = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(m_position.x, m_position.y, m_position.z, 1.0f), lookTarget, DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, 1.0f));
}
void Camera::UpdateConstBuf0() noexcept {
	UpdateDirectionVectors();
	UpdateViewMatrix();
	
	Renderer::UpdateConstBuffer(m_pConstBuf0, &m_constBuf0, sizeof(ConstBuf0));
}

void Camera::Bind() noexcept {
	Renderer::GetDeviceContext()->VSSetConstantBuffers(0u, 1u, &m_pConstBuf0);
}
void Camera::Bind(int bind_index) noexcept {
	Bind();
}