#include "components/Position.h"

Position::Position()
: position()
, heading(0.0)
{
}

Position::Position(const sf::Vector2f &position)
: position(position)
, heading(0.0)
{
}

Position::Position(const sf::Vector2f &position, double heading)
: position(position)
, heading(heading)
{
}
