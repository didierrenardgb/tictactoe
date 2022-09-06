#include "MatchWindow.h"

#include "imgui.h"

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
    ImGui::Begin(mWindowTitle);
    if (mMatch)
    {
        MatchWidget widget(*mMatch);
        widget.draw();
    }
    ImGui::End();
}

} // namespace ttt
