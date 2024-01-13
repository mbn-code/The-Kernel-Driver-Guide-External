#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <thread>

#include "guidev.h"
#include "Theme/Theme.h"
#include "imgui/imgui_impl_win32.h"

#include "Kernelinterface.hpp"
#include "Memory.h"

bool reset_size = true;
bool mainWindow = true;
int tabs = 1;
Theme theme;

/* RW/ImGUI Variable Handle Area */

bool showESPWin = false;

/* RW/ImGUI Variable Handle Area End */


void ReadWriteThreadController()
{

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


bool showBoxESP = false;
bool showNameESP = false;
bool showHealthESP = false;
bool showDistanceESP = false;

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

bool enableAimbot = false;
bool aimbotVisibleCheck = false;
bool aimbotSmoothing = false;
float aimbotSmoothingFactor = 0.5f;


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
