#pragma once

#include <memory>
#include <optional>

#include "IPlayer.h"

namespace ttt
{

class Tile
{
public:
	Tile() = default;

	std::optional<std::reference_wrapper<IPlayer const>> const& owner() const;
	void setOwner(IPlayer const& owner);

private:
	std::optional<std::reference_wrapper<IPlayer const>> mOwner;
};

} // namespace ttt
