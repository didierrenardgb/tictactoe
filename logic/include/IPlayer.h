#pragma once

#include <memory>
#include <string>

namespace ttt
{
class Board;
struct Coordinates;

class IPlayer
{
public:
	virtual ~IPlayer() = default;

	virtual std::string const& name() const = 0;
	virtual Coordinates play(Board const& board) const = 0;
};
} // namespace ttt
