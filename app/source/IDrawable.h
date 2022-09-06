#pragma once

namespace ttt
{

class IDrawable
{
public:
	virtual ~IDrawable() = default;
	virtual void draw() const = 0;
};

} // namespace ttt
