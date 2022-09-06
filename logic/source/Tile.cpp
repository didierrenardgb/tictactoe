#include "Tile.h"

namespace ttt
{

std::optional<std::reference_wrapper<IPlayer const>> const& Tile::owner() const
{
	return mOwner;
}

void Tile::setOwner(IPlayer const& owner)
{
	mOwner.emplace(owner);
}

} // namespace ttt
