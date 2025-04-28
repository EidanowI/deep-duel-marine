#include "IScene.h"
#include "../Model/Model.h"
#include "../Camera/Camera.h"



class MainMenuScene : public IScene {
public:
	MainMenuScene() noexcept;
	~MainMenuScene() noexcept override;

	void Render() noexcept override;

private:
	Camera* m_pCamera;
	Model* m_pModel;
};