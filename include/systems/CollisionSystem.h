#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "entityx/System.h"
#include "components/Position.h"
#include "components/Volume.h"
#include "components/Enemy.h"
#include "SoundId.h"

class CollisionSystem : public entityx::System<CollisionSystem> {
public:
   void update(entityx::EntityManager &entities, entityx::EventManager &events, double dt);

private:
   bool checkCollision(const Position& pos1, const Volume& vol1, const Position& pos2, const Volume& vol2);
   void spaceShipDamaged(entityx::Entity& spaceShip, entityx::EventManager& events);
   void enemyDamaged(entityx::Entity& enemy, entityx::EventManager& events);

   SoundId getHitSound(EnemyType type);
   SoundId getDeathSound(EnemyType type);
};

#endif // COLLISIONSYSTEM_H
