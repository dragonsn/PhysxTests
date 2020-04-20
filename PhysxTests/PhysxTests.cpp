// PhysxTests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "im3d.h"
#include "imgui.h"
#include "app/im3d_example.h"

void Test1();
void Test2();
void Test3();

int main(int, char**)
{
	Test3();
	Im3d::Example example;
	if (!example.init(-1, -1, "Im3d Example")) {
		return 1;
	}

	while (example.update()) {
		Im3d::RandSeed(0);

		Im3d::Context& ctx = Im3d::GetContext();
		Im3d::AppData& ad  = Im3d::GetAppData();

		ImGui::Begin("Im3d Demo", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
		if (ImGui::TreeNode("About")) {
			ImGui::Text("Welcome to the Im3d demo!");
			ImGui::Spacing();
			ImGui::Text("WASD   = forward/left/backward/right");
			ImGui::Text("QE     = down/up");
			ImGui::Text("RMouse = camera orientation");
			ImGui::Text("LShift = move faster");
			ImGui::Spacing();

			ImGui::TreePop();
		}
		ImGui::Spacing();
		if (ImGui::TreeNode("Camera")) {
			ImGui::Checkbox("Ortho", &example.m_camOrtho);

			ImGui::TreePop();
		}

		ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
		if (ImGui::TreeNode("Grid")) {
			static int gridSize = 20;
			ImGui::SliderInt("Grid Size", &gridSize, 1, 50);
			const float gridHalf = (float)gridSize * 0.5f;
			Im3d::SetAlpha(1.0f);
			Im3d::SetSize(1.0f);
			Im3d::BeginLines();
				for (int x = 0; x <= gridSize; ++x) {
					Im3d::Vertex(-gridHalf, 0.0f, (float)x - gridHalf, Im3d::Color(0.0f, 0.0f, 0.0f));
					Im3d::Vertex( gridHalf, 0.0f, (float)x - gridHalf, Im3d::Color(1.0f, 0.0f, 0.0f));
				}
				for (int z = 0; z <= gridSize; ++z) {
					Im3d::Vertex((float)z - gridHalf, 0.0f, -gridHalf,  Im3d::Color(0.0f, 0.0f, 0.0f));
					Im3d::Vertex((float)z - gridHalf, 0.0f,  gridHalf,  Im3d::Color(0.0f, 0.0f, 1.0f));
				}
			Im3d::End();
		
			ImGui::TreePop();
		}

		ImGui::End();

		example.draw();
	}
	example.shutdown();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
