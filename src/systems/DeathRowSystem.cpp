#include "systems/DeathRowSystem.h"
#include "systems/Events.h"
#include "components/DeathSentence.h"
#include "components/SpaceShip.h"
#include "components/Enemy.h"
#include "components/Animation.h"
#include "components/Gun.h"

using namespace entityx;

void DeathRowSystem::update(EntityManager& entities, EventManager& events, double dt)
{
    entities.each<DeathSentence>([&events, &dt](Entity entity, DeathSentence& deathSentence)
    {
        if(!deathSentence.haveBeenPrepared)
        {
            if(entity.has_component<AnimationContainer>())
            {
                auto animationContainer = entity.component<AnimationContainer>();
                animationContainer->resetAnimation(AnimationType::Movement);
                animationContainer->setAnimation(AnimationId(AnimationType::Death, DestroyedDeathAnimation));
            }

            if(entity.has_component<Gun>())
                entity.component<Gun>().remove();

            deathSentence.haveBeenPrepared = true;
        }

        if (deathSentence.timeToExecution <= 0.0)
        {
            if (entity.has_component<SpaceShip>())
            {
                entity.destroy();
                events.emit<EvGameOver>();
            }
            else
            {
                if (entity.has_component<Enemy>())
                {
                    Enemy* pEnemy =  entity.component<Enemy>().get();
                    if (pEnemy->type == EnemyType::Boss)
                    {
                        entity.destroy();
                        events.emit<EvBossKilled>();
                    }
                    else
                        entity.destroy();
                }
            }
        }
        else
        {
            deathSentence.timeToExecution -= dt;
        }
    });
}

