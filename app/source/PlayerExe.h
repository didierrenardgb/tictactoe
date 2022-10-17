#pragma once

#include "IPlayer.h"

namespace ttt
{

class PlayerExe : public IPlayer
{
public:
	PlayerExe(std::string const& name);                          //Player Name.
	std::string const& name() const override;
	Coordinates play(Board const& board) const override; 
	void initializeAlphaBeta(Board const& board, std::string const& name); 

private:
	std::string mName;

	













};

} // namespace ttt
