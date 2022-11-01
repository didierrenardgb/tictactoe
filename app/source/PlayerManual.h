#pragma once

#include "IPlayer.h"

namespace ttt
{

class PlayerManual : public IPlayer
{
public:
	PlayerManual(std::string const& name);
	std::string const& name() const override;
	Coordinates play(Board const& board) const override;

private:
	std::string mName;
};

} // namespace ttt
