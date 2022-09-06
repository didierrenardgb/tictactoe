#pragma once

#include <list>
#include <memory>
#include <vector>

namespace ttt
{

class IPlayer;
class Schedule;
class Standing;

class Competition
{
public:
	Competition();
	~Competition();

	void addPlayer(std::unique_ptr<IPlayer>&& player);
	std::unique_ptr<Schedule> const& start();
	std::unique_ptr<Schedule> const& schedule() const;
	bool complete() const;
	std::vector<Standing> standings() const;

private:
	std::vector<std::unique_ptr<IPlayer>> mPlayers;
	std::unique_ptr<Schedule> mSchedule;
};

} // namespace ttt
