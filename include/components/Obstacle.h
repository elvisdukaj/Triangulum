#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "entityx/Entity.h"

enum ObstacleType
{
   OT_Unknown,
};

struct Obstacle : public entityx::Component<Obstacle>
{
   Obstacle();
   Obstacle(ObstacleType type);

   ObstacleType type;
};

#endif // OBSTACLE_H
