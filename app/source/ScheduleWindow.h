#pragma once

#include "IDrawable.h"

namespace ttt
{
class Schedule;

class ScheduleWindow : public IDrawable
{
public:
	ScheduleWindow(Schedule const& schedule);

	void draw() const override;

private:
	Schedule const& mSchedule;
};

} // namespace ttt
