#include "IScene.h"
#include "../Model/Model.h"



class MainMenuScene : public IScene {
public:
	MainMenuScene();
	~MainMenuScene() override;

	void Render() override;

private:
	Model* m_pModel;
};