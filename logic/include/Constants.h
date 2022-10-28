#pragma once
namespace ttt
{	

namespace constants
{
const int NOTACOORD = -1;
enum class PLAYERS : char
{
	FIRST = 'X',
	SECOND = 'O',
	NONE = ' '
};
}

struct Spot
{
	int mScore{ 0 };
	constants::PLAYERS mOwner = constants::PLAYERS::NONE;
};

using SpotMat = std::vector<std::vector<Spot>>;
} // namespace ttt
