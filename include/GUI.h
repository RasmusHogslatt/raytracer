#pragma once

#include <Parameters.h>
#include <imgui.h>

class GUI
{
public:
	GUI(Parameters& appParameters);

	
private:
	Parameters& params;
};