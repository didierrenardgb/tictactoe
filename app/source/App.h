#pragma once

#include <memory>
#include <chrono>

#include "imgui.h"

#include "ImGuiWindow.h"

namespace ttt
{

class Competition;
class CompetitionRunner;

class App
{
public:
	App(std::chrono::milliseconds&& updateInterval, ImVec4&& clearColor);
	~App();

	int run();

private:
	void update();
	void draw();

	std::unique_ptr<Competition> mCompetition;
	std::unique_ptr<CompetitionRunner> mCompetitionRunner;
	ImVec4 mClearColor;
	ImGuiWindow mWindow;
};

} // namespace ttt
