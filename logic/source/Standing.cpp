#include "Standing.h"

#include "Match.h"
#include "IPlayer.h"

namespace ttt
{

Standing::Standing(std::unique_ptr<IPlayer> const& player):
	mPlayer(player)
	, mPlayed(0)
	, mWon(0)
	, mDrawn(0)
	, mLost(0)
	, mPoints(0)
{
}

std::unique_ptr<IPlayer> const& Standing::player() const
{
	return mPlayer;
}

int Standing::played() const
{
	return mPlayed;
}

int Standing::won() const
{
	return mWon;
}

int Standing::drawn() const
{
	return mDrawn;
}

int Standing::lost() const
{
	return mLost;
}

int Standing::points() const
{
	return mPoints;
}

void Standing::processMatch(Match const& match)
{
	if (match.played() && match.hasPlayer(*player().get()))
	{
		mPlayed++;
		if (match.wonBy(*player().get()))
		{
			mWon++;
			mPoints += 3;
		}
		else if (match.drawn())
		{
			mDrawn++;
			mPoints++;
		}
		else
		{
			mLost++;
		}
	}
}

bool Standing::operator <(Standing const& other) const
{
	if (points() > other.points())
	{
		return true;
	}
	else if (points() == other.points())
	{
		if (won() > other.won())
		{
			return true;
		}
		else if (played() < other.played())
		{
			return true;
		}
	}
	return false;
}

} // namespace ttt
