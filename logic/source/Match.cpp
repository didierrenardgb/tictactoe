#include "Match.h"

#include "Coordinates.h"
#include "Tile.h"

namespace ttt
{

Match::Match(Board&& board, std::list<std::reference_wrapper<IPlayer const>>& players) :
	mBoard(std::forward<Board>(board))
	, mPlayers(players.begin(), players.end())
{
}

bool Match::hasPlayer(IPlayer const& player) const
{
	for (auto const& p : mPlayers)
	{
		if (&p.get() == &player)
		{
			return true;
		}
	}

	return false;
}

void Match::setPlayerSymbol(IPlayer const& player, char symbol)
{
	mSymbols[&player] = symbol;
}

char Match::playerSymbol(IPlayer const& player) const
{
	auto iter = mSymbols.find(&player);
	if (iter != mSymbols.end())
	{
		return iter->second;
	}
	return ' ';
}

Board const& Match::board() const
{
	return mBoard;
}

Board& Match::board()
{
	return mBoard;
}

bool Match::played() const
{
	for (auto const& player : mPlayers)
	{
		if (wonBy(player))
		{
			return true;
		}
	}

	return mBoard.filled();
}

bool Match::started() const
{
	return !mBoard.empty() || (mSymbols.size() == mPlayers.size());
}

bool equal(std::unique_ptr<Tile> const& a, std::unique_ptr<Tile> const& b)
{
	return a && b && a->owner().has_value() && b->owner().has_value() && &a->owner().value().get() == &b->owner().value().get();
}

bool equal(std::unique_ptr<Tile> const& a, IPlayer const& b)
{
	return a && a->owner().has_value() && &a->owner().value().get() == &b;
}

bool check(Board const& board, std::unique_ptr<Tile> const& prev, Coordinates const& at, IPlayer const& who)
{
	auto const& tile = board.tile({ at.x, at.y });
	return equal(prev, tile) && equal(tile, who);
}

bool Match::wonBy(IPlayer const& who) const
{
	for (int x = 0; x < board().width(); x++)
	{
		for (int y = 0; y < board().height(); y++)
		{
			int vertical = 1;
			int horizontal = 1;
			int diagonal = 1;
			int diagonal2 = 1;
			for (int w = 1; w < board().winCondition(); w++)
			{
				auto const& prev = board().tile({ x, y });

				if (check(board(), prev, { x + w, y }, who))
				{
					horizontal++;
				}

				if (check(board(), prev, { x, y + w }, who))
				{
					vertical++;
				}

				if (check(board(), prev, { x + w, y + w }, who))
				{
					diagonal++;
				}

				if (check(board(), prev, { x - w, y + w }, who))
				{
					diagonal2++;
				}
			}

			int wc = board().winCondition();
			if ((vertical == wc) || (horizontal == wc) || (diagonal == wc) || (diagonal2 == wc))
			{
				return true;
			}
		}
	}

	return false;
}

bool Match::drawn() const
{
	if (played())
	{
		for (auto const& player : mPlayers)
		{
			if (wonBy(player))
			{
				return false;
			}
		}
	}

	return mBoard.filled();
}

std::list<std::reference_wrapper<IPlayer const>> const& Match::players() const
{
	return mPlayers;
}

} // namespace ttt
