#include "Board.h"

#include "Tile.h"
#include "Coordinates.h"

namespace ttt
{
static std::unique_ptr<Tile> sNullTile = nullptr;

struct Board::Impl
{
	Impl(int width, int height, int winCondition):
		mWinCondition(winCondition)
	{
		initialize(width, height);
	}

	Impl(Impl const& other):
		mWinCondition(other.mWinCondition)
	{
		initialize(static_cast<int>(other.mTiles.size()), static_cast<int>(other.mTiles[0].size()));
	}

	void initialize(int width, int height)
	{
		if (width >= 3 && height >= 3)
		{
			mTiles.reserve(width);
			for (int i = 0; i < width; i++)
			{
				auto column = std::vector<std::unique_ptr<Tile>>();
				column.reserve(height);
				for (int j = 0; j < height; j++)
				{
					column.push_back(std::make_unique<Tile>());
				}
				mTiles.push_back(std::move(column));
			}
		}
	}

	std::unique_ptr<Tile>& tile(Coordinates const& at);

	std::vector<std::vector<std::unique_ptr<Tile>>> mTiles;
	int mWinCondition;
};

Board::Board(int width, int height, int winCondition):
mImpl(std::make_unique<Impl>(width, height, winCondition))
{
}

Board::Board(Board const& other):
mImpl(std::make_unique<Impl>(*other.mImpl.get()))
{
}

Board::~Board() = default;

bool Board::valid() const
{
	return mImpl->mTiles.size() > 2 && mImpl->mTiles[0].size() > 2 && winCondition() > 2;
}

int Board::width() const
{
	return static_cast<int>(mImpl->mTiles.size());
}

int Board::height() const
{
	if (mImpl->mTiles.size() > 0)
	{
		return static_cast<int>(mImpl->mTiles[0].size());
	}
	return 0;
}

int Board::winCondition() const
{
	return mImpl->mWinCondition;
}

std::unique_ptr<Tile> const& Board::tile(Coordinates const& at) const
{
	if (at.x < mImpl->mTiles.size())
	{
		auto const& column = mImpl->mTiles[at.x];
		if (at.y < column.size())
		{
			return column[at.y];
		}
	}

	return sNullTile;
}

bool Board::play(IPlayer const& player, Coordinates const& at)
{
	auto& chosen = tile(at);
	if (chosen && !chosen->owner().has_value())
	{
		chosen->setOwner(player);
		return true;
	}
	return false;
}

bool Board::filled() const
{
	for (int x = 0; x < mImpl->mTiles.size(); x++)
	{
		for (int y = 0; y < mImpl->mTiles.size(); y++)
		{
			auto& t = tile({ x, y });
			if (t == nullptr || !t->owner().has_value())
			{
				return false;
			}
		}
	}

	return true;
}

bool Board::empty() const
{
	for (int x = 0; x < mImpl->mTiles.size(); x++)
	{
		for (int y = 0; y < mImpl->mTiles.size(); y++)
		{
			auto& t = tile({ x, y });
			if (t != nullptr && t->owner().has_value())
			{
				return false;
			}
		}
	}

	return true;
}

std::unique_ptr<Tile>& Board::Impl::tile(Coordinates const& at)
{
	if (at.x < mTiles.size())
	{
		auto& column = mTiles[at.x];
		if (at.y < column.size())
		{
			return column[at.y];
		}
	}

	return sNullTile;
}

} // namespace ttt