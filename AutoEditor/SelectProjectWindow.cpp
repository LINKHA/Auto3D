#include "SelectProjectWindow.h"

#include <ThirdParty/Imgui/imgui.h>

void ShowSelectProjectWindow()
{
	static float f = 0.0f;
	static int counter = 0;

	bool state;
	ImGuiWindowFlags windowFlag = 0;
	windowFlag |= ImGuiWindowFlags_NoTitleBar;
	windowFlag |= ImGuiWindowFlags_NoResize;
	windowFlag |= ImGuiWindowFlags_NoMove;
	//windowFlag |= ImGuiWindowFlags_NoBackground;
	ImGui::Begin("Hello, world!", &state, windowFlag);// Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}