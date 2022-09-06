#pragma once

#include <list>
#include <map>

#include "Board.h"

namespace ttt
{
class IPlayer;

class Match
{
public:
	Match(Board&& board, std::list<std::reference_wrapper<IPlayer const>>& players);

	bool hasPlayer(IPlayer const& player) const;
	void setPlayerSymbol(IPlayer const& player, char symbol);
	char playerSymbol(IPlayer const& player) const;

	Board const& board() const;
	Board& board();
	bool played() const;
	bool started() const;

	bool wonBy(IPlayer const& who) const;
	bool drawn() const;

	std::list<std::reference_wrapper<IPlayer const>> const& players() const;

private:
	Board mBoard;
	std::list<std::reference_wrapper<IPlayer const>> mPlayers;
	std::map<IPlayer const*, char> mSymbols;
};

} // namespace ttt
