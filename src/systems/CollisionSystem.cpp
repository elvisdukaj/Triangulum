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

CollisionSystem::CollisionSystem()
{
}

void CollisionSystem::update(EntityManager& entities,
                             EventManager& events,
                             double dt)
{
   // Spaceships hitting enemies
   SpaceShip::Handle spaceShip;
   Enemy::Handle enemy;
   PickUp::Handle pickUp;
   Position::Handle spaceShipPos, enemyPos, pickUpPos;
   Volume::Handle spaceShipVol, enemyVol, pickUpVol;
   Health::Handle spaceShipHealth;
   for (Entity spaceShipEntity : entities.entities_with_components(spaceShip, spaceShipPos, spaceShipVol, spaceShipHealth))
   {
      if (spaceShipEntity.has_component<DeathSentence>())
      {
         break;
      }

      if (spaceShipHealth->isInvulnerable())
      {
        // Beware! Must be done only once in this update!
         spaceShipHealth->invulnerableTime -= dt;
      }
      else
      {
         for (Entity enemyEntity : entities.entities_with_components(enemy, enemyPos, enemyVol))
         {
            if (checkCollision(spaceShipPos.get(),
                               spaceShipVol.get(),
                               enemyPos.get(),
                               enemyVol.get()))
            {
               spaceShipDamaged(spaceShipEntity, events);
               break;
            }
         }

         for (Entity pickUpEntity : entities.entities_with_components(pickUp, pickUpPos, pickUpVol))
         {
            if (checkCollision(spaceShipPos.get(),
                               spaceShipVol.get(),
                               pickUpPos.get(),
                               pickUpVol.get()))
            {
               events.emit<EvPickUp>(spaceShipEntity.id(), pickUp->pickUpType);
               pickUpEntity.destroy();
               break;
            }
         }
      }
   }

   // Bullets hitting things
   Bullet::Handle bullet;
   Position::Handle bulletPos;
   Volume::Handle bulletVol;
   Health::Handle health;
   for (Entity bulletEntity : entities.entities_with_components(bullet, bulletPos, bulletVol))
   {
      // Bullets hitting spaceships
      for (Entity spaceShipEntity : entities.entities_with_components(spaceShip, spaceShipPos, spaceShipVol, spaceShipHealth))
      {
         if (spaceShipEntity.has_component<DeathSentence>() || bullet->ownerId == spaceShipEntity.id())
         {
            break;
         }

         if (checkCollision(bulletPos.get(),
                            bulletVol.get(),
                            spaceShipPos.get(),
                            spaceShipVol.get()))
          {
             if (!spaceShipHealth->isInvulnerable())
             {
                spaceShipDamaged(spaceShipEntity, events);
             }
              
             bulletEntity.destroy();
              
             break;
          }
      }

      if (!bulletEntity.valid())
      {
        break;
      }

      // Bullets hitting Enemies
      for (Entity enemyEntity : entities.entities_with_components(enemy, enemyPos, enemyVol, health))
      {
         if (   bullet->ownerId == enemyEntity.id()
             || enemyEntity.has_component<DeathSentence>())
         {
            continue;
         }

         if (checkCollision(bulletPos.get(),
                            bulletVol.get(),
                            enemyPos.get(),
                            enemyVol.get()))
         {
            if (!health->isInvulnerable())
            {
               enemyDamaged(enemyEntity, events);

               if(health->health == 0 && entities.get(bullet->ownerId).has_component<SpaceShip>())
               {
                 spaceShip->score += enemy->value;
                 events.emit<EvCurrentScore>(spaceShip->score);
               }
            }

            bulletEntity.destroy();

            break;
         }
      }
   }
}

bool CollisionSystem::checkCollision(Position* pos1,
                                     Volume* vol1,
                                     Position* pos2,
                                     Volume* vol2)
{
   for (auto box1 : vol1->m_boxes)
   {
      box1.setObjectPosition(pos1->position);

      for (auto box2 : vol2->m_boxes)
      {
        box2.setObjectPosition(pos2->position);

        if (box1.checkCollision(box2))
        {
          return true;
        }
      }
   }

   return false;
}

void CollisionSystem::spaceShipDamaged(Entity& spaceShip,
                                       EventManager& events)
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

void CollisionSystem::enemyDamaged(Entity& enemyEntity,
                                   EventManager& events)
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
      {
          enemyEntity.assign<DeathSentence>(700.0);
      }
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
