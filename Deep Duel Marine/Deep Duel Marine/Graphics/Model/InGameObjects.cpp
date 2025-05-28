#include "InGameObjects.h"



FieldHintObject::FieldHintObject() noexcept {
	m_pModel = new Model("FielHint.dae", "FieldHintVertShader.cso", "FieldHintPixelShader.cso");

	//m_constBuff1.model = MakeTransformMatrix(Vector3D(-0.5, 0.2f, 13), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(1.0f));

	m_constBuff1.model = MakeTransformMatrix(Vector3D(-0.5, 0.2f, 0), Vector3D(0.0f,0.0f,0.0f), Vector3D(1.0f));
	m_pConstBuff1 = Renderer::CreateConstBuffer((char*)&m_constBuff1, sizeof(ConstBuf1));

}
FieldHintObject::~FieldHintObject() noexcept {
	if (m_pConstBuff1) {
		m_pConstBuff1->Release();
		m_pConstBuff1 = nullptr;
	}

	delete m_pModel;
}

void FieldHintObject::Render() noexcept {
	Renderer::GetDeviceContext()->VSSetConstantBuffers(1u, 1u, &m_pConstBuff1);

	m_pModel->Render();
}