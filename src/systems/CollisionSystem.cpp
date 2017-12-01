#include "systems/CollisionSystem.h"
#include "systems/Events.h"
#include "components/SpaceShip.h"
#include "components/Bullet.h"
#include "components/Health.h"
#include "components/Animation.h"
#include "components/DeathSentence.h"
#include "components/PlayerControl.h"
#include "components/Gun.h"
#include "components/Display.h"
#include "components/Motion.h"
#include "AnimationFactory.h"

using namespace entityx;

void CollisionSystem::update(EntityManager& entities, EventManager& events, double dt)
{
    // Spaceships hitting enemies or pickups
    entities.each<SpaceShip, Position, Volume, Health>(
        [this, &events, &entities, &dt]
        (Entity spaceShipEntity, SpaceShip& spaceShip, Position& spaceShipPos, Volume& spaceShipVol, Health& spaceShipHealth)
    {
        if (spaceShipEntity.has_component<DeathSentence>())
            return;

        if (spaceShipHealth.isInvulnerable())
            // Beware! Must be done only once in this update!
            spaceShipHealth.invulnerableTime -= dt;
        else
        {
            entities.each<Enemy, Position, Volume>(
                [this, &events, &spaceShipEntity, &spaceShipPos, &spaceShipVol]
                (Entity enemyEntity, Enemy& /*enemy*/, Position& enemyPos, Volume& enemyVol)
            {
                if (checkCollision(spaceShipPos, spaceShipVol, enemyPos, enemyVol))
                {
                    spaceShipDamaged(spaceShipEntity, events);
                    return;
                }
            });

            entities.each<PickUp, Position, Volume>(
                [this, &events, &spaceShipEntity, &spaceShip, &spaceShipPos, &spaceShipVol]
                (Entity pickUpEntity, PickUp& pickUp, Position& pickUpPos, Volume& pickUpVol)
            {
                if (checkCollision(spaceShipPos, spaceShipVol, pickUpPos, pickUpVol))
                {
                    events.emit<EvPickUp>(spaceShipEntity.id(), pickUp.pickUpType);
                    pickUpEntity.destroy();
                    return;
                }
            });
        }
    });

    // Bullets hitting things
    entities.each<Bullet, Position, Volume>(
        [this, &events, &entities] (Entity bulletEntity, Bullet& bullet, Position& bulletPos, Volume& bulletVol)
    {
        // Bullets hitting spaceships
        entities.each<SpaceShip, Position, Volume, Health>(
                    [this, &events, &entities, &bulletEntity, &bullet, &bulletPos, &bulletVol]
                    (Entity spaceShipEntity, SpaceShip&, Position& spaceShipPos, Volume& spaceShipVol, Health& spaceShipHealth)
        {
            if (spaceShipEntity.has_component<DeathSentence>() || bullet.ownerId == spaceShipEntity.id())
                return;

            if (checkCollision(bulletPos, bulletVol, spaceShipPos, spaceShipVol))
            {
                if (!spaceShipHealth.isInvulnerable())
                    spaceShipDamaged(spaceShipEntity, events);

                bulletEntity.destroy();
                return;
            }
        });

        if (!bulletEntity.valid())
            return;

        // Bullets hitting Enemies
        entities.each<Enemy, Position, Volume, Health>(
            [this, &events,&entities, &bulletEntity, &bullet, &bulletPos, &bulletVol]
            (Entity enemyEntity, Enemy& enemy, Position& enemyPos, Volume& enemyVol, Health& health)
        {
            if (!bulletEntity.valid())
                return;

            if (bullet.ownerId == enemyEntity.id() || enemyEntity.has_component<DeathSentence>())
                return;

            if (checkCollision(bulletPos, bulletVol, enemyPos, enemyVol))
            {
                if (!health.isInvulnerable())
                {
                    enemyDamaged(enemyEntity, events);

                    auto spaceShipEntity = entities.get(bullet.ownerId);
                    auto spaceShip = spaceShipEntity.component<SpaceShip>();

                    if(health.health == 0 && entities.get(bullet.ownerId).has_component<SpaceShip>())
                    {
                        spaceShip->score += enemy.value;
                        events.emit<EvCurrentScore>(spaceShip->score);
                    }
                }

                bulletEntity.destroy();
                return;
            }
        });
    });
}

bool CollisionSystem::checkCollision(const Position& pos1, const Volume& vol1, const Position& pos2, const Volume& vol2)
{
    for (auto box1 : vol1.m_boxes)
    {
        box1.setObjectPosition(pos1.position);

        for (auto box2 : vol2.m_boxes)
        {
            box2.setObjectPosition(pos2.position);

            if (box1.checkCollision(box2))
                return true;
        }
    }

    return false;
}

void CollisionSystem::spaceShipDamaged(Entity& spaceShip, EventManager& events)
{
    Health::Handle health = spaceShip.component<Health>();
    Display::Handle display = spaceShip.component<Display>();

    if (health->health == 0)
    {
        spaceShip.assign<DeathSentence>(2000.0);
    }
    else
    {
        health->health -= 1;
        health->invulnerableTime = 2000.0;
        display->blink.startBlink(2000.0);
    }

    events.emit<EvPlaySound>(SHIP_EXPLOSION);
}

void CollisionSystem::enemyDamaged(Entity& enemyEntity, EventManager& events)
{
    Health::Handle health = enemyEntity.component<Health>();
    Enemy::Handle enemy = enemyEntity.component<Enemy>();

    health->health -= 1;
    events.emit<EvPlaySound>(getHitSound(enemy->type));

    if (health->health == 0)
    {
        enemyEntity.component<Volume>().remove();

        events.emit<EvPlaySound>(getDeathSound(enemy->type));

        if (enemy->type == EnemyType::Boss)
        {
            enemyEntity.component<Motion>().remove();
            enemyEntity.assign<DeathSentence>(5000.0);
        }
        else
            enemyEntity.assign<DeathSentence>(700.0);
    }
}

SoundId CollisionSystem::getHitSound(EnemyType type)
{
    SoundId soundId = NO_SOUND;

    switch (type) {
    case EnemyType::Asteroid:
        soundId = ASTEROID_HIT;
        break;
    case EnemyType::Boss:
    case EnemyType::Scout:
        soundId = SCOUT_HIT;
        break;
    default:
        break;
    }

    return soundId;
}

SoundId CollisionSystem::getDeathSound(EnemyType type)
{
    SoundId soundId = NO_SOUND;

    switch (type) {
    case EnemyType::Asteroid:
        soundId = ASTEROID_EXPLOSION;
        break;
    case EnemyType::Boss:
    case EnemyType::Scout:
        soundId = SCOUT_EXPLOSION;
        break;
    default:
        break;
    }

    return soundId;
}
