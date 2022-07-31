#pragma once

#include "GUI.h"

GUI::GUI(Parameters& appParameters) : params{ appParameters }
{
	ImGui::ShowMetricsWindow();
}
