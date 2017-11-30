#include "components/Obstacle.h"

Obstacle::Obstacle()
: type(OT_Unknown)
{
}

Obstacle::Obstacle(ObstacleType type)
: type(type)
{
}
