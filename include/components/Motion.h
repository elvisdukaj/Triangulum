#ifndef MOTION_H
#define MOTION_H

#include "entityx/Entity.h"
#include "SFML/System/Vector2.hpp"

struct Motion : public entityx::Component<Motion>
{
   Motion();
   Motion(const sf::Vector2f velocity);
   Motion(const sf::Vector2f velocity, double rotation);

   sf::Vector2f velocity;
   double rotation;
};

#endif
