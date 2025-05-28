#include "InGameScene.h"
#include "../Renderer/Renderer.h"
#include "../../Dependencies/imGUI/imgui.h"
#include "../../Dependencies/imGUI/imgui_internal.h"
#include "../Model/ShipObjects.h"


void SuperFunc();
void BeginImGuiTranspWindow(const char* pWindow_name);
void EndImGuiTranspWindow();

extern bool G_is_vertical_ship_position;

enum SELECT_STATE {
	REMOVE,
	SHIP1,
	SHIP2,
	SHIP3,
	SHIP4
};
SELECT_STATE G_select_state = REMOVE;

struct ShipCellPos {
	ShipCellPos() {
		x = -1;
		y = -1;
	}
	int x = -1;
	int y= -1;
};
struct ShpInPlace {
	enum SHIP_TYPE {
		one = 1,
		two = 2,
		three = 3,
		four = 4
	};
	ShpInPlace(SHIP_TYPE type, Vector3D pos, int x, int y){
		if (type == one) {
			m_cells[0].x = x; m_cells[0].y = y;
			m_cells[1].x = -1; m_cells[1].y = -1;
			m_cells[2].x = -1; m_cells[2].y = -1;
			m_cells[3].x = -1; m_cells[3].y = -1;
		}
		else if (type == two) {
			if (G_is_vertical_ship_position) {
				m_cells[0].x = x; m_cells[0].y = y;
				m_cells[1].x = x; m_cells[1].y = y + 1;
				m_cells[2].x = -1; m_cells[2].y = -1;
				m_cells[3].x = -1; m_cells[3].y = -1;
			}
			else {
				m_cells[0].x = x; m_cells[0].y = y;
				m_cells[1].x = x + 1; m_cells[1].y = y;
				m_cells[2].x = -1; m_cells[2].y = -1;
				m_cells[3].x = -1; m_cells[3].y = -1;
			}
		}
		else if (type == three) {
			if (G_is_vertical_ship_position) {
				m_cells[0].x = x; m_cells[0].y = y;
				m_cells[1].x = x; m_cells[1].y = y + 1;
				m_cells[2].x = x; m_cells[2].y = y + 2;
				m_cells[3].x = -1; m_cells[3].y = -1;
			}
			else {
				m_cells[0].x = x; m_cells[0].y = y;
				m_cells[1].x = x + 1; m_cells[1].y = y;
				m_cells[2].x = x + 2; m_cells[2].y = y;
				m_cells[3].x = -1; m_cells[3].y = -1;
			}
		}
		else {
			if (G_is_vertical_ship_position) {
				m_cells[0].x = x; m_cells[0].y = y;
				m_cells[1].x = x; m_cells[1].y =  y+ 1;
				m_cells[2].x = x; m_cells[2].y = y + 2;
				m_cells[3].x = x; m_cells[3].y = y + 3;
			}
			else {
				m_cells[0].x = x; m_cells[0].y = y;
				m_cells[1].x = x+1; m_cells[1].y = y;
				m_cells[2].x = x+2; m_cells[2].y = y;
				m_cells[3].x = x+3; m_cells[3].y = y;
			}
		}
		m_type = type;
		m_shipObject = new ShipObject(type, pos);
	}
	~ShpInPlace() {
		delete m_shipObject;
	}

	bool IsCrossAnotherShip(ShpInPlace* other) {
		for (int j = 0; j < 4; j++) {
			int x = other->m_cells[j].x;
			int y = other->m_cells[j].y;

			for (int i = 0; i < 4; i++) {
				if (m_cells[i].x == -1 || m_cells[i].y == -1) continue;

				/*
					#0 #1 #2
					#3 #4 #5
					#6 #7 #8
				*/
				if (m_cells[i].x - 1 >= 0 && m_cells[i].x - 1 == x && m_cells[i].y - 1 >= 0 && m_cells[i].y - 1 == y) return true;//#0
				if (m_cells[i].x == x && m_cells[i].y - 1 >= 0 && m_cells[i].y - 1 == y) return true;//#1
				if (m_cells[i].x + 1 == x && m_cells[i].y - 1 >= 0 && m_cells[i].y - 1 == y) return true;//#2
				if (m_cells[i].x - 1 == x && m_cells[i].y == y) return true;//#3
				if (m_cells[i].x == x && m_cells[i].y == y) return true;//#4
				if (m_cells[i].x + 1 == x && m_cells[i].y == y) return true;//#5
				if (m_cells[i].x - 1 == x && m_cells[i].y + 1 == y) return true;//#6
				if (m_cells[i].x == x && m_cells[i].y + 1 == y) return true;//#7
				if (m_cells[i].x + 1 == x && m_cells[i].y + 1 == y) return true;//#8
			}
		}

		

		return false;
	}

	SHIP_TYPE m_type;

	ShipCellPos m_cells[4]{ ShipCellPos(), ShipCellPos(), ShipCellPos (), ShipCellPos ()};
	ShipObject* m_shipObject;
};
struct ShipPlacings {
	bool IsShowSelect4Ship() {
		int count = 0;
		for (int i = 0; i < m_places.size(); i++) {
			if (m_places[i]->m_type == ShpInPlace::SHIP_TYPE::four) {
				count++;
			}
		}

		if (count < 1) return true;
		return false;
	}
	bool IsShowSelect3Ship() {
		int count = 0;
		for (int i = 0; i < m_places.size(); i++) {
			if (m_places[i]->m_type == ShpInPlace::SHIP_TYPE::three) {
				count++;
			}
		}

		if (count < 2) return true;
		return false;
	}
	bool IsShowSelect2Ship() {
		int count = 0;
		for (int i = 0; i < m_places.size(); i++) {
			if (m_places[i]->m_type == ShpInPlace::SHIP_TYPE::two) {
				count++;
			}
		}

		if (count < 3) return true;
		return false;
	}
	bool IsShowSelect1Ship() {
		int count = 0;
		for (int i = 0; i < m_places.size(); i++) {
			if (m_places[i]->m_type == ShpInPlace::SHIP_TYPE::one) {
				count++;
			}
		}

		if (count < 4) return true;
		return false;
	}

	bool TryToPlace(int ship_size, Vector3D pos, int x, int y) {
		if (ship_size == 4) {
			if (!G_is_vertical_ship_position && x > 6) return false;
			if (G_is_vertical_ship_position && y > 6) return false;

			ShpInPlace* test = new ShpInPlace(ShpInPlace::four, pos, x, y);

			for (int i = 0; i < m_places.size(); i++) {
				if (m_places[i]->IsCrossAnotherShip(test)) {
					delete test;
					return false;
				}
			}


			m_places.push_back(test);
		}
		else if (ship_size == 3) {
			if (!G_is_vertical_ship_position && x > 7) return false;
			if (G_is_vertical_ship_position && y > 7) return false;


			ShpInPlace* test = new ShpInPlace(ShpInPlace::three, pos, x, y);

			for (int i = 0; i < m_places.size(); i++) {
				if (m_places[i]->IsCrossAnotherShip(test)) {
					delete test;
					return false;
				}
			}

			m_places.push_back(test);
		}
		else if (ship_size == 2) {
			if (!G_is_vertical_ship_position && x > 8) return false;
			if (G_is_vertical_ship_position && y > 8) return false;

			ShpInPlace* test = new ShpInPlace(ShpInPlace::two, pos, x, y);
			for (int i = 0; i < m_places.size(); i++) {
				if (m_places[i]->IsCrossAnotherShip(test)) {
					delete test;
					return false;
				}
			}

			m_places.push_back(test);
		}
		else {
			ShpInPlace* test = new ShpInPlace(ShpInPlace::one, pos, x, y);
			for (int i = 0; i < m_places.size(); i++) {
				if (m_places[i]->IsCrossAnotherShip(test)) {
					delete test;
					return false;
				}
			}

			m_places.push_back(test);
		}

		return true;
	}

	void TryToRemoveShip(int x, int y) {
		for (int i = 0; i < m_places.size(); i++) {
			for (int j = 0; j < 4; j++) {
				if (m_places[i]->m_cells[j].x == x && m_places[i]->m_cells[j].y == y) {
					delete m_places[i];
					m_places.erase(m_places.begin() + i);
					return;
				}
			}
		}
	}
	void Render() {
		for (int i = 0; i < m_places.size(); i++) {
			m_places[i]->m_shipObject->Render();
		}
	}

	std::vector<ShpInPlace*> m_places;
};

ShipPlacings G_placings{};


InGameScene::InGameScene() noexcept {
	//m_pCamera = new Camera(Vector3D(0, 24, 8.0), Vector3D(-1.3, 0, 0));
	m_pCamera = new Camera(Vector3D(0, 24, 0), Vector3D(-1.57, 0, 0));

	m_pWaterSurface_object = new WaterSurfaceObject("WaterSurfacePixelShaderInGame.cso");

	m_pFieldHint_object = new FieldHintObject();
}
InGameScene::~InGameScene() noexcept {
	delete m_pWaterSurface_object;
	delete m_pCamera;
	delete m_pFieldHint_object;
}

void InGameScene::Render() noexcept {
	static struct ConstBuf1 {
		DirectX::XMMATRIX model;
	} transfMat;//8.25, 0.0, -8.73
	float x_test = 8.25 - 9 * 1.94778f;
	float y_test = -8.73 + 9 * 1.94778f;
	transfMat.model = MakeTransformMatrix(Vector3D(x_test, 0.0, y_test), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(1.0f));
	static Model test_ship = Model("OneShip.dae", "ModelVertexShader.cso", "ModelPixelShader.cso");
	static ID3D11Buffer* m_pConstBuff1_trans = Renderer::CreateConstBuffer((char*)&transfMat, sizeof(ConstBuf1));

	
	m_pCamera->Bind();

	m_pWaterSurface_object->Render();

	m_pFieldHint_object->Render();

	//Renderer::GetDeviceContext()->VSSetConstantBuffers(1u, 1u, &m_pConstBuff1_trans);
	//test_ship.Render();

	G_placings.Render();
	SuperFunc();
}

ImVec2  operator+(const ImVec2& lhs, const ImVec2& rhs);
ImVec2  operator-(const ImVec2& lhs, const ImVec2& rhs);

bool HidenFieldButton(int x, int y) {
	static Model one_ship = Model("OneShip.dae", "ModelVertexShader.cso", "SelectShipPixelShader.cso");
	static Model two_ship = Model("TwoShip.fbx", "ModelVertexShader.cso", "SelectShipPixelShader.cso");
	static Model three_ship = Model("TreeShip.fbx", "ModelVertexShader.cso", "SelectShipPixelShader.cso");
	static Model four_ship = Model("SuperMassiveShip.dae", "ModelVertexShader.cso", "SelectShipPixelShader.cso");
	static Texture halogram_tex = Texture("Halogram.png");
	static struct ConstBuf1 {
		DirectX::XMMATRIX model;
	} transfMat;
	//float x_test = 8.25 - x * 1.94778f;
	//float y_test = -8.73 + y * 1.94778f;

	float x_test = 8.38 - x * 1.99f;
	float y_test = -8.9 + y * 1.99f;
	float angle = G_is_vertical_ship_position ? 3.1415f : 1.57f;
	transfMat.model = MakeTransformMatrix(Vector3D(x_test, 0.0, y_test), Vector3D(0.0f, angle, 0.0f), Vector3D(1.0f));
	static ID3D11Buffer* m_pConstBuff1_trans = Renderer::CreateConstBuffer((char*)&transfMat, sizeof(ConstBuf1));
	Renderer::UpdateConstBuffer(m_pConstBuff1_trans, &transfMat, sizeof(ConstBuf1));

	

	using namespace ImGui;
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImVec2 size_arg = ImVec2(MainWindow::GetWindowWidth() / 25, MainWindow::GetWindowWidth() / 25);

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	std::string name = "##bcc" + std::to_string(x) + std::to_string(y);
	const ImGuiID id = window->GetID(name.c_str());
	const ImVec2 label_size = CalcTextSize(name.c_str(), NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((0 & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, 0);

	// Render

	ImColor buton_active_color = ImColor(100, 100, 100, 0);
	ImColor buton_hovered_color = ImColor(255, 255, 255,0);
	ImColor buton_color = ImColor(130, 130, 130, 0);

	if (hovered) {
		halogram_tex.Bind();
		Renderer::GetDeviceContext()->VSSetConstantBuffers(1u, 1u, &m_pConstBuff1_trans);
		if(G_select_state == SHIP1)
			one_ship.Render();
		if (G_select_state == SHIP2)
			two_ship.Render();
		if (G_select_state == SHIP3)
			three_ship.Render();
		if (G_select_state == SHIP4)
			four_ship.Render();
	}

	const ImU32 col = (held && hovered) ? buton_active_color : hovered ? buton_hovered_color : buton_color;
	RenderNavHighlight(bb, id);
	RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

	//if (g.LogEnabled)
		//LogSetNextTextDecoration("[", "]");
	//RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

	// Automatically close popups
	//if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
	//    CloseCurrentPopup();

	return pressed;
}
bool FourShipSelectButton(int ship_size)
{
	static Texture shipstexs[4] = { Texture("Ship1.png"), Texture("Ship2.png"),Texture("Ship3.png"),Texture("Ship4.png") };
	std::string label = "Ship labe" + std::to_string(ship_size);
	ImGuiButtonFlags flags = 0;
	ImVec2 size_arg = ImVec2(MainWindow::GetWindowWidth() / 22, MainWindow::GetWindowWidth() / 22);
	ImVec2 pos_vec = ImGui::GetCursorScreenPos();

	using namespace ImGui;
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label.c_str());
	const ImVec2 label_size = CalcTextSize(label.c_str(), NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);

	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	// Render
	ImColor buton_active_color = ImColor(150, 150, 150, 255);
	ImColor buton_hovered_color = ImColor(255, 255, 255, 150);
	ImColor buton_color = ImColor(255, 255, 255, 255);

	const ImVec4 col = ((held && hovered) ? buton_active_color : hovered ? buton_hovered_color : buton_color);

	Texture* pTex = shipstexs + ship_size - 1;
	ImGui::SetCursorPos(pos_vec);

	ImGui::Image(pTex->GetShaderResView(), size_arg, ImVec2(0, 0), ImVec2(1, 1), col);

	return pressed;
}

void SuperFunc() {
	BeginImGuiTranspWindow("fields buttons");
	ImFont* pFont = ImGui::GetFont();

	float y_pos = (float)MainWindow::GetWindowHeight() / 8.2443f;
	for (int y = 0; y < 10; y++) {
		float x_pos = (float)MainWindow::GetWindowWidth() / 3.35f;
		for (int x = 0; x < 10; x++) {
			ImGui::SetCursorPos(ImVec2(x_pos, y_pos));
			if (HidenFieldButton(x, y)) {
				if (G_select_state == SELECT_STATE::REMOVE) {
					G_placings.TryToRemoveShip(x, y);
				}
				else {
					float x_test = 8.38 - x * 1.99f;
					float y_test = -8.9 + y * 1.99f;
					Vector3D vec = Vector3D(x_test, 0, y_test);
					if (G_placings.TryToPlace(G_select_state, vec, x, y)) {
						G_select_state = SELECT_STATE::REMOVE;
					}
				}
				//MessageBoxA(0, std::to_string(i).c_str(), std::to_string(j).c_str(), 0);
			}
			x_pos += MainWindow::GetWindowWidth() / 25 + (float)MainWindow::GetWindowWidth() / 282.35294f;
		}
		y_pos += MainWindow::GetWindowWidth() / 25 + (float)MainWindow::GetWindowWidth() / 282.35294f;
	}

	int disp_start_count = 0;
	if (G_placings.IsShowSelect4Ship()) {
		ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 2 - MainWindow::GetWindowWidth() / 29*2 - 30, (float)MainWindow::GetWindowHeight() / 1.11));
		if (FourShipSelectButton(4)) {
			G_select_state = SHIP4;
		}
		disp_start_count++;
	}	
	if (G_placings.IsShowSelect3Ship()) {
		ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 2 - MainWindow::GetWindowWidth() / 29 + 10, (float)MainWindow::GetWindowHeight() / 1.11));
		if (FourShipSelectButton(3)) {
			G_select_state = SHIP3;
		}
		disp_start_count++;
	}
	if (G_placings.IsShowSelect2Ship()) {
		ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 2 + MainWindow::GetWindowWidth() / 29 - 10, (float)MainWindow::GetWindowHeight() / 1.11));
		if (FourShipSelectButton(2)) {
			G_select_state = SHIP2;
		}
		disp_start_count++;
	}
	if (G_placings.IsShowSelect1Ship()) {
		ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() / 2 + MainWindow::GetWindowWidth() / 29 * 2 + 30, (float)MainWindow::GetWindowHeight() / 1.11));
		if (FourShipSelectButton(1)) {
			G_select_state = SHIP1;
		}
		disp_start_count++;
	}

	if (!disp_start_count) {
		pFont->Scale = (float)MainWindow::GetWindowWidth() / 3200;
		ImGui::PushFont(pFont);
		ImGui::SetCursorPos(ImVec2(MainWindow::GetWindowWidth() /2 - MainWindow::GetWindowWidth() / 29 * 4 / 2,MainWindow::GetWindowHeight() / 1.11));
		if (ImGui::Button(" READY!", ImVec2(MainWindow::GetWindowWidth() / 29 * 4, MainWindow::GetWindowWidth() / 29))) {
			///TODO START GAME LOGIC
		}
		ImGui::PopFont();
	}

	EndImGuiTranspWindow();
}