#pragma once

#include <memory>

namespace ttt
{

class IPlayer;
class Match;

class Standing
{
public:
	Standing(std::unique_ptr<IPlayer> const& player = nullptr);

	std::unique_ptr<IPlayer> const& player() const;
	int played() const;
	int won() const;
	int drawn() const;
	int lost() const;
	int points() const;

	void processMatch(Match const& match);

	bool operator <(Standing const& other) const;

private:
	std::reference_wrapper<std::unique_ptr<IPlayer> const> mPlayer;
	int mPlayed;
	int mWon;
	int mDrawn;
	int mLost;
	int mPoints;
};

} // namespace ttt
