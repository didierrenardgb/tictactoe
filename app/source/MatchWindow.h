#pragma once

#include "IDrawable.h"

namespace ttt
{
class Match;

class MatchWindow : public IDrawable
{
public:
	MatchWindow(char const* windowTitle, Match const* match);

	void draw() const override;

private:
	char const* mWindowTitle;
	Match const* mMatch;
};

} // namespace ttt
