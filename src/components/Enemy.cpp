#include "components/Enemy.h"

Enemy::Enemy(EnemyType type)
    : type(type)
{
    switch (type) {
    case EnemyType::Asteroid:
        value = 5;
        break;

    case EnemyType::Scout:
        value = 15;
        break;

    case EnemyType::Mine:
        value = 10;
        break;

    case EnemyType::Boss:
        value = 40;
        break;

    default:
        value = 0;
        break;
    }
}
