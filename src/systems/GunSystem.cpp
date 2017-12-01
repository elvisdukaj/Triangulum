#include "systems/GunSystem.h"
#include "systems/Events.h"
#include "components/Position.h"
#include "components/Gun.h"
#include "components/Enemy.h"
#include "components/SpaceShip.h"
#include "utils/TMath.h"
#include "EntityCreator.h"

#include "SFML/Graphics/Color.hpp"

using namespace entityx;

GunSystem::GunSystem()
{
}

void GunSystem::update(EntityManager &entities,
                       EventManager &events,
                       double dt)
{
    entities.each<Gun, Position>(
                [&entities, &events, &dt](Entity entity, Gun& gun, Position& position)
    {
        gun.shootTimer -= dt;
        gun.heat -= dt / gun.cooldownTime * 100.0f;

        if (gun.heat < 0.0f)
            gun.heat = 0.0f;

        auto bulletHeat = BulletDataTable::lookupHeat(gun.bulletType);

        if (gun.isMainFirePressed && gun.shootTimer <= 0.0f && gun.heat < 100.0f - bulletHeat)
        {
            auto initVelocity = sf::Vector2f(500.0f * gun.direction.x, 500.0f * gun.direction.y);
            auto bulletPosition = position;
            bulletPosition.heading = math::angleBetween(gun.direction);

            // Enemies have evil red shoots,  our hero have green, star wars style
            // Civilians have white.
            auto color = sf::Color();
            if(entity.has_component<Enemy>())
                color = sf::Color(255, 0, 0);
            else if(entity.has_component<SpaceShip>())
                color = sf::Color(0, 255, 0);

            BulletCreator(entity.id(),
                          initVelocity,
                          bulletPosition.position,
                          bulletPosition.heading,
                          gun.bulletType,
                          color)
                    .create(entities.create());

            gun.shootTimer = 5.0f / 100.0f * 2500.0f;
            gun.heat += bulletHeat;

            events.emit<EvPlaySound>(GUN_SHOOT);
        }
    });
}
