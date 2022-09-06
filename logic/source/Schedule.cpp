#include "Schedule.h"

#include "Fixture.h"
#include "Match.h"

namespace ttt
{

std::list<Fixture> const& Schedule::fixtures() const
{
	return mFixtures;
}

std::list<Fixture>& Schedule::fixtures()
{
	return mFixtures;
}

bool Schedule::addFixture(Fixture&& fixture)
{
	mFixtures.push_back(std::move(fixture));
	return true;
}

bool Schedule::complete() const
{
	for (Fixture const& fixture : mFixtures)
	{
		if (!fixture.complete())
		{
			return false;
		}
	}

	return true;
}

Match const* Schedule::ongoingMatch() const
{
	for (Fixture const& fixture : mFixtures)
	{
		for (Match const& match : fixture.matches())
		{
			if (match.started() && !match.played())
			{
				return &match;
			}
		}
	}

	return nullptr;
}

} // namespace ttt
