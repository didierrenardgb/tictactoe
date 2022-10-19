#pragma once

#include "IPlayer.h"
#include "Coordinates.h"

namespace ttt
{
enum constants
{
	FASTEST = 2,
	WIN = 1,
	DRAW = 0,
	LOSE = -1,
	UNDEFINED = -2,
	MINIMUM = -10,
	MAXIMUM = 10,
	NOTACOORD = -1
};
enum players
{
	PLAYER = 'X',
	OTHER = 'O',
	NONE = ' '
};
class Spot
{
	int mScore { 0 }; 
	char mOwner = NONE;
public:

	char owner() const;
	int score() const;
	int addScore(int amount);
	void setOwner(char owner);
};
	
class PlayerFede : public IPlayer
{
public:
	PlayerFede(std::string const& name);
	std::string const& name() const override;
	Coordinates play(Board const& board) const override;

private:
	std::string mName;
};


} // namespace ttt
