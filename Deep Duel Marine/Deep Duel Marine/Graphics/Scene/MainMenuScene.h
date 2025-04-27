#include "IScene.h"
#include "../Model/Model.h"



class MainMenuScene : public IScene {
public:
	MainMenuScene() noexcept;
	~MainMenuScene() noexcept override;

	void Render() noexcept override;

private:
	Model* m_pModel;
};