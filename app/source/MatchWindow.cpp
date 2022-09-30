#include "MatchWindow.h"

#include "imgui.h"

#include "Configuration.h"
#include "MatchWidget.h"

namespace ttt
{

MatchWindow::MatchWindow(char const* windowTitle, Match const* match):
	mWindowTitle(windowTitle),
	mMatch(match)
{
}

void MatchWindow::draw() const
{
    WindowGeometry mainGeometry = mainWindowGeometry();

    ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize({ static_cast<float>(mainGeometry.width / 2), static_cast<float>(mainGeometry.height / 2) }, ImGuiCond_FirstUseEver);
    ImGui::Begin(mWindowTitle);
    if (mMatch)
    {
        MatchWidget widget(*mMatch);
        widget.draw();
    }
    ImGui::End();
}

} // namespace ttt
