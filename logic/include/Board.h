#pragma once

#include <memory>
#include <vector>

namespace ttt
{

class Tile;
class IPlayer;
struct Coordinates;

class Board
{
public:
	Board(int width = 5, int height = 5, int winCondition = 5);
	Board(Board const& other);
	~Board();

	bool valid() const;
	int width() const;
	int height() const;
	int winCondition() const;

	std::unique_ptr<Tile> const& tile(Coordinates const& at) const;
	bool play(IPlayer const& player, Coordinates const& at);
	bool filled() const;
	bool empty() const;

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};

} // namespace ttt