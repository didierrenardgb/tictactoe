#pragma once

#include <list>

namespace ttt
{

class Match;

class Fixture
{
public:
	~Fixture();

	std::list<Match> const& matches() const;
	std::list<Match>& matches();
	bool addMatch(Match&& match);

	bool complete() const;

private:
	std::list<Match> mMatches;
};

} // namespace ttt
