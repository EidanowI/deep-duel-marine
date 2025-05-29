#pragma once
#include "IScene.h"
#include "../Model/Model.h"
#include "../Camera/Camera.h"
#include "../Model/WaterSurfaceObject.h"
#include "../Model/InGameObjects.h"
#include "../Model/ShipObjects.h"



void RecieveStartCallback();
void RecieveTurnCallback();
void RecieveShotResponce(int x, int y, bool is_dead);


enum TARGET_CELL_STATE {
	NO_INFO,
	MISS,
	DEAD
};
enum SELECT_STATE {
	REMOVE,
	SHIP1,
	SHIP2,
	SHIP3,
	SHIP4
};
enum GAME_STATE {
	PLACE_SHIP,
	PLAY_GAME
};

struct ShipCellPos {
public:
	ShipCellPos();

public:
	int x = -1;
	int y = -1;
	bool is_dead = true;
};

struct ShpInPlace {
	enum SHIP_TYPE {
		one = 1,
		two = 2,
		three = 3,
		four = 4
	};

public:
	ShpInPlace(SHIP_TYPE type, Vector3D pos, int x, int y);
	~ShpInPlace();

	bool IsCrossAnotherShip(ShpInPlace* other);

public:
	SHIP_TYPE m_type;

	ShipCellPos m_cells[4]{ ShipCellPos(), ShipCellPos(), ShipCellPos(), ShipCellPos() };
	ShipObject* m_shipObject;
};

struct ShipPlacings {
	bool IsShowSelect4Ship();
	bool IsShowSelect3Ship();
	bool IsShowSelect2Ship();
	bool IsShowSelect1Ship();

public:
	bool TryToPlace(int ship_size, Vector3D pos, int x, int y);

	void TryToRemoveShip(int x, int y);

public:
	int AlliweShipsCount();

public:

	void Render();

public:
	std::vector<ShpInPlace*> m_places;
};


class InGameScene : public IScene {
	friend void PlaceShipLogic(InGameScene* pInGameScene);
public:
	InGameScene() noexcept;
	~InGameScene() noexcept override;

	void Render() noexcept override;

private:
	Camera* m_pCamera;
	WaterSurfaceObject* m_pWaterSurface_object;
	FieldHintObject* m_pFieldHint_object;
};