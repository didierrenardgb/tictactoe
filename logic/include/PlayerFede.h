#pragma once

#include "IPlayer.h"
#include "Coordinates.h"

namespace ttt
{	
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
