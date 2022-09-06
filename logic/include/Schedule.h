#pragma once

#include <list>

namespace ttt
{

class Fixture;
class Match;

class Schedule
{
public:
    std::list<Fixture> const& fixtures() const;
    std::list<Fixture>& fixtures();
    bool addFixture(Fixture&& fixture);

    bool complete() const;

    Match const* ongoingMatch() const;

private:
    std::list<Fixture> mFixtures;
};

} // namespace ttt
