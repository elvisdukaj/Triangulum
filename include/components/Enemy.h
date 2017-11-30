#ifndef ENEMY_H
#define ENEMY_H

#include "entityx/Entity.h"

enum class EnemyType
{
   Unknown,
   Asteroid,
   Scout,
   Mine,
   Boss,
};

struct Enemy : public entityx::Component<Enemy>
{
   Enemy(EnemyType type = EnemyType::Unknown);

   EnemyType type;
   int value;
};

#endif // ENEMY_H
