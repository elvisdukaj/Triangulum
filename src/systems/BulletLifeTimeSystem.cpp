#include "systems/BulletLifeTimeSystem.h"
#include "components/Bullet.h"

using namespace entityx;

BulletLifeTimeSystem::BulletLifeTimeSystem()
{
}

void BulletLifeTimeSystem::update(EntityManager& entities, EventManager&, double dt)
{
    entities.each<Bullet>([&dt](Entity entity, Bullet& bullet)
    {
        bullet.age += dt;
        if (bullet.age > bullet.lifeTime)
            entity.destroy();
    });
}
