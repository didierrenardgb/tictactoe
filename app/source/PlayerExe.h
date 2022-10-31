#pragma once

#include "IPlayer.h"

namespace ttt
{
	class PlayerExe : public IPlayer
	{
	public:
		PlayerExe(std::string const &name);
		std::string const &name() const override;
		Coordinates play(Board const &board) const override;

	private:
		std::string mName;
	};

} // namespace ttt
