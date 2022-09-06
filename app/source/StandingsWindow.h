#pragma once

#include <vector>

#include "IDrawable.h"

namespace ttt
{

class Standing;

class StandingsWindow : public IDrawable
{
public:
	StandingsWindow(std::vector<ttt::Standing> const& standings);

	void draw() const override;

private:
	std::vector<ttt::Standing> const& mStandings;
};

} // namespace ttt
