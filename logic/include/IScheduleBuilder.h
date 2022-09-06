#pragma once

#include <memory>
#include <vector>

namespace ttt
{

class Schedule;
class IPlayer;

class IScheduleBuilder
{
public:
	virtual std::unique_ptr<Schedule> build(std::vector<std::unique_ptr<IPlayer>> const& players) const = 0;
};

} // namespace ttt
