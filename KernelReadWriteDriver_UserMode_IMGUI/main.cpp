#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <thread>
#include <map>

#include "guidev.h"
#include "Theme/Theme.h"
#include "imgui/imgui_impl_win32.h"

#include "global.h"
#include "Memory.h"
#include "Utils.h"


// Static Pointers //

uint32_t LocalPlayerAdress;

// Static Pointers End //

DWORD BaseModuleAdress;
DWORD ProcessId;

char nameBuffer[16]; // Buffer for Player Name
char weaponnameBuffer[16]; // Buffer for Weapon Name

bool reset_size = true;
bool mainWindow = true;
int tabs = 1;
Theme theme;

/* RW/ImGUI Variable Handle Area */

bool showBoxESP = false;
bool showNameESP = false;
bool showHealthESP = false;
bool showDistanceESP = false;

bool enableAimbot = false;
bool aimbotVisibleCheck = false;
bool aimbotSmoothing = false;
float aimbotSmoothingFactor = 0.5f;

/* RW/ImGUI Variable Handle Area End */


void ReadWriteThreadController()
{
	while (true) {
		if (enableAimbot) { // Aimbot
			uintptr_t EntityList = Driver.ReadVirtualMemory<uintptr_t>(ProcessId, BaseModuleAdress + Memory::Adress::EntityList, sizeof(uintptr_t));

			if (EntityList) { // Check for null
				float Dist = 1000;

				uintptr_t TargetEntity = LocalPlayerAdress;

				int CurrentPlayerCount = Driver.ReadVirtualMemory<int>(ProcessId, BaseModuleAdress + Memory::Adress::PlayerCount, sizeof(int));

				for (int i = 1; i < CurrentPlayerCount; i++) {
					uintptr_t Entity = Driver.ReadVirtualMemory<uintptr_t>(ProcessId, BaseModuleAdress + Memory::Adress::EntityList + (i * 0x4), sizeof(uintptr_t));

					/* Assign Vars */
					Vec3 EntityHeadPos = Driver.ReadVirtualMemory<Vec3>(ProcessId, Entity + Memory::EntityOffsets::HeadX, sizeof(Vec3));
					Vec3 LocalPlayerHeadPos = Driver.ReadVirtualMemory<Vec3>(ProcessId, LocalPlayerAdress + Memory::EntityOffsets::HeadX, sizeof(Vec3));
					int EntityHealth = Driver.ReadVirtualMemory<int>(ProcessId, Entity + Memory::EntityOffsets::HP, sizeof(int));
					int EntityTeam = Driver.ReadVirtualMemory<int>(ProcessId, Entity + Memory::EntityOffsets::Team, sizeof(int));
					int LocalPlayerTeam = Driver.ReadVirtualMemory<int>(ProcessId, LocalPlayerAdress + Memory::EntityOffsets::Team, sizeof(int));

					if ((LocalPlayerHeadPos - EntityHeadPos).hypo3() < Dist && EntityHealth > 0 && EntityTeam != LocalPlayerTeam) // Do Checks
					{
						TargetEntity = Entity; // Set Target
						Dist = (LocalPlayerHeadPos - EntityHeadPos).hypo3(); // Set Distance
					}

					Vec3 Delta = EntityHeadPos - LocalPlayerHeadPos; // Get Delta

					float yaw = atan2f(Delta.y, Delta.x) * 180 / 3.141592653589793238463; // Get Yaw
					float hyp = sqrt(Delta.x * Delta.x + Delta.y * Delta.y); // Get Hypo
					float pitch = atan2f(Delta.z, hyp) * 180 / 3.141592653589793238463; // Get Pitch

					if (IsKeyPressed(VK_CONTROL)) {
						// Check if aimbot key is pressed
						Driver.WriteVirtualMemory<float>(ProcessId, LocalPlayerAdress + Memory::EntityOffsets::ViewAngleX, pitch, sizeof(float)); // Write Pitch
						Driver.WriteVirtualMemory<float>(ProcessId, LocalPlayerAdress + Memory::EntityOffsets::ViewAngleY, yaw, sizeof(float)); // Write Yaw
					}
				}
			}
		}
	}

	return;
}

enum class WindowState {
	Main,
	EspMenu,
	Aimbot,
	Misc,
	Others
};

WindowState currentWindowState = WindowState::Main; // Init currentWindowState to the MainWindow

void RenderMainWindow() {

	int Health = Driver.ReadVirtualMemory<int>(ProcessId, LocalPlayerAdress + Memory::EntityOffsets::HP, sizeof(int));
	int Armour = Driver.ReadVirtualMemory<int>(ProcessId, LocalPlayerAdress + Memory::EntityOffsets::Armor, sizeof(int));

	float HeadX = Driver.ReadVirtualMemory<float>(ProcessId, LocalPlayerAdress + Memory::EntityOffsets::HeadX, sizeof(float));
	float HeadY = Driver.ReadVirtualMemory<float>(ProcessId, LocalPlayerAdress + Memory::EntityOffsets::HeadY, sizeof(float));
	float HeadZ = Driver.ReadVirtualMemory<float>(ProcessId, LocalPlayerAdress + Memory::EntityOffsets::HeadZ, sizeof(float));
	float X = Driver.ReadVirtualMemory<float>(ProcessId, LocalPlayerAdress + Memory::EntityOffsets::X, sizeof(float));
	float Y = Driver.ReadVirtualMemory<float>(ProcessId, LocalPlayerAdress + Memory::EntityOffsets::Y, sizeof(float));
	float Z = Driver.ReadVirtualMemory<float>(ProcessId, LocalPlayerAdress + Memory::EntityOffsets::Z, sizeof(float));
	Driver.ReadVirtualMemory<char>(ProcessId, LocalPlayerAdress + Memory::EntityOffsets::Name, nameBuffer, sizeof(nameBuffer));
	int Team = Driver.ReadVirtualMemory<int>(ProcessId, LocalPlayerAdress + Memory::EntityOffsets::Team, sizeof(int));

	// Weapon Information
	Driver.ReadVirtualMemory<char>(ProcessId, LocalPlayerAdress + Memory::EntityOffsets::Name, weaponnameBuffer, sizeof(weaponnameBuffer));
	short weaponRecoil1 = Driver.ReadVirtualMemory<short>(ProcessId, Memory::EntityOffsets::CurrentWeaponStruct + Memory::EntityOffsets::Weapon::Recoil1, sizeof(short));
	short weaponRecoil2 = Driver.ReadVirtualMemory<short>(ProcessId, Memory::EntityOffsets::CurrentWeaponStruct + Memory::EntityOffsets::Weapon::Recoil2, sizeof(short));

    // Display player information
	ImGui::Text("Local Player Address: 0x%X", LocalPlayerAdress);
	ImGui::Text("Head Position: %.2f, %.2f, %.2f", HeadX, HeadY, HeadZ);
	ImGui::Text("Position: %.2f, %.2f, %.2f", X, Y, Z);
	ImGui::Text("Health: %d", Health);
	ImGui::Text("Armor: %d", Armour);
	ImGui::Text("Player Name: %s", nameBuffer);
	ImGui::Text("Team: %d", Team);

	// Display weapon information
	ImGui::Text("Weapon Information");
	ImGui::Text("Weapon Name: %s", weaponnameBuffer);
	ImGui::Text("Weapon Recoil1: %d", weaponRecoil1);
	ImGui::Text("Weapon Recoil2: %d", weaponRecoil2);

	// Set a fixed size for buttons
	const ImVec2 buttonSize(150.0f, 100.0f);

	if (currentWindowState == WindowState::Main) {

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, -5));

		ImGui::Button("ESP Menu", buttonSize);
		if (ImGui::IsItemClicked()) {
			currentWindowState = WindowState::EspMenu;

		}

		ImGui::SameLine();

		ImGui::Button("Aimbot", buttonSize);
		if (ImGui::IsItemClicked()) {
			currentWindowState = WindowState::Aimbot;
		}

		ImGui::NewLine();

		ImGui::Button("Misc", buttonSize);
		if (ImGui::IsItemClicked()) {
			currentWindowState = WindowState::Misc;
		}

		ImGui::SameLine();

		ImGui::Button("Others", buttonSize);
		if (ImGui::IsItemClicked()) {
			currentWindowState = WindowState::Others;
		}

		ImGui::PopStyleVar();
	}
}

void RenderEspMenu(){

	ImGui::Text("ESP Menu");

	// Checkbox for Box ESP
	ImGui::Checkbox2("Box ESP", &showBoxESP);

	// Checkbox for Name ESP
	ImGui::Checkbox2("Name ESP", &showNameESP);

	// Checkbox for Health ESP
	ImGui::Checkbox2("Health ESP", &showHealthESP);

	// Checkbox for Distance ESP
	ImGui::Checkbox2("Distance ESP", &showDistanceESP);


	if (ImGui::Button("Back")) {
		currentWindowState = WindowState::Main;
	}
		
}


void RenderAimbotMenu() {
    ImGui::Text("Aimbot Menu");

    // Checkbox for Enable Aimbot
    ImGui::Checkbox2("Enable Aimbot", &enableAimbot);

    // Checkbox for Aimbot Visibility Check
    ImGui::Checkbox2("Aimbot Visibility Check", &aimbotVisibleCheck);

    // Checkbox for Aimbot Smoothing
    ImGui::Checkbox2("Aimbot Smoothing", &aimbotSmoothing);

    if (aimbotSmoothing) {
        // Slider for Aimbot Smoothing Factor
		
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
		ImGui::SliderFloat("Smoothing", &aimbotSmoothingFactor, 0, 1);
		ImGui::PopItemWidth();  // Reset item width to default

    }

    if (ImGui::Button("Back")) {
        currentWindowState = WindowState::Main;
    }
}


bool bunnyHop = false;
bool noFlash = false;
bool autoStrafe = false;
bool thirdPerson = false;


void RenderMiscMenu() {
	ImGui::Text("Misc Menu");

	// Checkbox for Bunny Hop
	ImGui::Checkbox2("Bunny Hop", &bunnyHop);

	// Checkbox for No Flash
	ImGui::Checkbox2("No Flash", &noFlash);

	// Checkbox for Auto Strafe
	ImGui::Checkbox2("Auto Strafe", &autoStrafe);

	// Checkbox for Third Person
	ImGui::Checkbox2("Third Person", &thirdPerson);

	if (ImGui::Button("Back")) {
		currentWindowState = WindowState::Main;
	}
}


bool showFPS = false;
bool nightMode = false;
bool showCoords = false;


void RenderOthersMenu() {
	ImGui::Text("Others Menu");

	// Checkbox for Show FPS
	ImGui::Checkbox2("Show FPS", &showFPS);

	// Checkbox for Night Mode
	ImGui::Checkbox2("Night Mode", &nightMode);

	// Checkbox for Show Coordinates
	ImGui::Checkbox2("Show Coordinates", &showCoords);

	if (ImGui::Button("Back")) {
		currentWindowState = WindowState::Main;
	}
}


int main()
{
	// Make BaseModuleAdress + ProcessId Available
	BaseModuleAdress = Driver.GetClientAdress();
	ProcessId = Driver.GetProcessId();

	LocalPlayerAdress = Driver.ReadVirtualMemory<uint32_t>(ProcessId, BaseModuleAdress + Memory::Adress::LocalPlayerAdress, sizeof(uint32_t));


	ShowWindow(GetConsoleWindow(), SW_HIDE);

	std::thread RWThread(ReadWriteThreadController); // Start RW Thread to run alongisde ImGui

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, 1);

	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ExternalProB1"), NULL };
	RegisterClassEx(&wc);
	HWND hwnd = CreateWindow(wc.lpszClassName, _T("ExternalProB1"), WS_OVERLAPPEDWINDOW, 0, 0, 50, 50, NULL, NULL, wc.hInstance, NULL);

	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		UnregisterClass(wc.lpszClassName, wc.hInstance);
		return 1;
	}

	UpdateWindow(hwnd);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	theme.Register(io);

	ImGuiStyle& style = ImGui::GetStyle();
	style.FramePadding = ImVec2(20, 20);
	// new line padding style
	//style.ItemSpacing = ImVec2(5, -5);


	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 4.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImVec4 mainColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	ImVec4 accentColor = ImVec4(0.5f, 0.5f, 0.8f, 1.0f);
	ImVec4 textColor = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);

	style.Colors[ImGuiCol_Text] = textColor;
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = mainColor;
	style.Colors[ImGuiCol_Border] = accentColor;
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = accentColor;
	style.Colors[ImGuiCol_FrameBgActive] = accentColor;
	style.Colors[ImGuiCol_TitleBg] = mainColor;
	style.Colors[ImGuiCol_TitleBgActive] = accentColor;
	style.Colors[ImGuiCol_TitleBgCollapsed] = mainColor;
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = accentColor;
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
	style.Colors[ImGuiCol_Button] = accentColor;
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	style.Colors[ImGuiCol_Header] = accentColor;
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	style.Colors[ImGuiCol_Separator] = accentColor;

	ImGuiStyle& TitlteBarStyle = ImGui::GetStyle();
	style.FramePadding = ImVec2(0, 0);


	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(g_pd3dDevice);

	bool done = true;

	std::string str_window_title = "Kernel Read Write Driver - User Mode";
	const char* window_title = str_window_title.c_str();

	DWORD window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiWindowFlags_NoResize;

	ImGui::StyleColorsRed();


	while (done)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				done = false;
		}
		if (!done)
			break;

		ImGuiStyle* style = &ImGui::GetStyle();

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{
			if (mainWindow)
			{
				if (reset_size) {
					ImGui::SetNextWindowSize(ImVec2(0,0), ImGuiCond_Always);
					reset_size = false;
				}

				/* RENDER START */

				//ImGui::ShowDemoWindow();

				
				ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f,0.5f));
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.5f, 4.5f));
				ImGui::Begin(window_title, &mainWindow, window_flags);

				if (currentWindowState == WindowState::Main) {
					RenderMainWindow();
				}
				else if (currentWindowState == WindowState::EspMenu) {
					RenderEspMenu();
				}
				else if (currentWindowState == WindowState::Aimbot) {
					RenderAimbotMenu();
				}
				else if (currentWindowState == WindowState::Misc) {
					RenderMiscMenu();
				}
				else if (currentWindowState == WindowState::Others) {
					RenderOthersMenu();
				};

				ImGui::PopStyleVar(2);

				ImGui::End();

				
				/* RENDER DONE */
			}
			else {
				exit(0);
			}
		}
		ImGui::EndFrame();
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
		if (g_pd3dDevice->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			g_pd3dDevice->EndScene();
		}

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

		if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			ResetDevice();
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	DestroyWindow(hwnd);
	UnregisterClass(wc.lpszClassName, wc.hInstance);

	return 0;
}
