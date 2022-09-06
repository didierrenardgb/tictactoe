#pragma once

#include "IScheduleBuilder.h"
#include "Board.h"

namespace ttt
{

class ScheduleBuilderRoundRobin : public IScheduleBuilder
{
public:
	ScheduleBuilderRoundRobin(Board&& baseBoard);

	std::unique_ptr<Schedule> build(std::vector<std::unique_ptr<IPlayer>> const& players) const override;

private:
	Board mBaseBoard;
};

} // namespace ttt
