#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include "../IBindable.h"
#include "../Utils/GraphicsMath.h"
#include "../Renderer/Renderer.h"
#include "../MainWindow/MainWindow.h"



class Camera : public IBindable {
public:
	struct ConstBuf0 {
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
public:
	Camera(Vector3D position, Vector3D rotation) noexcept;
	~Camera() noexcept;

	void SetPosition(Vector3D position) noexcept;
	void SetRotation(Vector3D rotation) noexcept;

	void UpdateDirectionVectors() noexcept;
	void UpdateViewMatrix() noexcept;
	void UpdateConstBuf0() noexcept;

	void Bind() noexcept override;
	void Bind(int bind_index) noexcept override;

private:
	Vector3D m_position;
	Vector3D m_rotation;
	DirectX::XMMATRIX m_rotation_matrix;

	Vector3D m_right;
	Vector3D m_up;
	Vector3D m_forward;

	ConstBuf0 m_constBuf0;
	ID3D11Buffer* m_pConstBuf0;
};