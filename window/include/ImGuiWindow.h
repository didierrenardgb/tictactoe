#pragma once

#include <memory>
#include <string>
#include <functional>

#include <windows.h>

#include "WindowGeometry.h"

struct ImVec4;

namespace ttt
{

class WindowClass;
class Window;
class DirectX;

class ImGuiWindow
{
public:
	using DrawFunction = std::function<void(ImVec4& clearColor)>;
	using UpdateFunction = std::function<void()>;

	ImGuiWindow(std::string const& windowTitle, WindowGeometry const& geometry = { 100, 100, 1280, 800 });
	~ImGuiWindow();

	bool start(DrawFunction const& drawFunction, UpdateFunction const& updateFunction);
	void mainLoop(ImVec4 const& clearColor);
	void end();

	bool checkEventLoop();

	std::unique_ptr<ttt::DirectX> const& directX() const;

private:
	void render(ImVec4 const& clearColor);
	LRESULT onWindowEvent(ttt::Window* w, UINT msg, WPARAM wParam, LPARAM lParam);

	std::string mWindowTitle;
	ttt::WindowGeometry mInitialGeometry;
	std::string mWindowClassName;
	std::unique_ptr<ttt::WindowClass> mWindowClass;
	std::shared_ptr<ttt::Window> mWindow;
	std::unique_ptr<ttt::DirectX> mDirectX;
	DrawFunction mDrawFunction;
	UpdateFunction mUpdateFunction;
};

} // namespace ttt
