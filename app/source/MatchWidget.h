#pragma once

#include "IDrawable.h"

namespace ttt
{
class Match;

class MatchWidget : public IDrawable
{
public:
	MatchWidget(Match const& match);

	void draw() const override;

private:
	Match const& mMatch;
};

} // namespace ttt
