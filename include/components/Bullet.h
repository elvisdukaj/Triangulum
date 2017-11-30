#ifndef BULLET_H
#define BULLET_H

#include "entityx/Entity.h"

struct Bullet : public entityx::Component<Bullet>
{
   Bullet();

   Bullet(entityx::Entity::Id ownerId,
          double lifeTime,
          long damage);

   entityx::Entity::Id ownerId;

   double lifeTime;
   double age;
   double producedHeat;
   long damage;
};

#endif // BULLET_H
