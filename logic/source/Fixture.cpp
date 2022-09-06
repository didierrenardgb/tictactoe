#include "Fixture.h"

#include "Match.h"

namespace ttt
{

Fixture::~Fixture() = default;

std::list<Match> const& Fixture::matches() const
{
	return mMatches;
}

std::list<Match>& Fixture::matches()
{
	return mMatches;
}

bool Fixture::addMatch(Match&& match)
{
	mMatches.push_back(std::forward<Match>(match));
	return true;
}

bool Fixture::complete() const
{
	for (auto const& match : mMatches)
	{
		if (!match.played())
		{
			return false;
		}
	}

	return true;
}

} // namespace ttt
