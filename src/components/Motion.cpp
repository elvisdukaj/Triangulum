#include "components/Motion.h"

Motion::Motion()
: velocity()
, rotation(0.0)
{
}

Motion::Motion(const sf::Vector2f velocity)
: velocity(velocity)
, rotation(0.0)
{
}

Motion::Motion(const sf::Vector2f velocity, double rotation)
: velocity(velocity)
, rotation(rotation)
{
}
