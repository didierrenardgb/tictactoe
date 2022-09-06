#pragma once

#include <chrono>

namespace ttt
{

class Competition;
class Match;

class CompetitionRunner
{
public:
	CompetitionRunner(Competition& competition, std::chrono::milliseconds&& updateInterval);

	void update();

private:
	bool shouldStep() const;

	Competition& mCompetition;
	std::chrono::system_clock::time_point mLastStep;
	std::chrono::milliseconds mUpdateInterval;
};

} // namespace ttt
