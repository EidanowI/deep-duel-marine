#include "IScene.h"
#include "../Model/Model.h"
#include "../Camera/Camera.h"
#include "../Model/WaterSurfaceObject.h"
#include "../Model/BeaconLogoObject.h"



class MainMenuScene : public IScene {
public:
	MainMenuScene() noexcept;
	~MainMenuScene() noexcept override;

	void Render() noexcept override;

private:
	Camera* m_pCamera;
	WaterSurfaceObject* m_pWaterSurface_object;
	BeaconLogoObject* m_pBeaconLogo_object;
};