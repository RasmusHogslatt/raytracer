#pragma once

#include <Parameters.h>
#include <imgui.h>

class GUI {
public:
	GUI(Parameters& appParameters);
	void menu();
	void controlPanel();
	void camerasettings();
	void sceneconstruction();
	void rendersettings();
	void viewportPanel();
	void renderport();
	void glviewport();
private:
	Parameters& params;
	ImVec2 renderTexturePos_;
};