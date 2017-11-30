#include "components/Bullet.h"

using namespace entityx;

Bullet::Bullet()
    : ownerId(Entity::INVALID)
    , lifeTime(0.0)
    , age(0.0)
    , damage(0)
{
}

Bullet::Bullet(Entity::Id ownerId,
               double lifeTime,
               long damage)
    : ownerId(ownerId)
    , lifeTime(lifeTime)
    , age(0.0)
    , damage(damage)
{
}
