#include "systems/AnimationSystem.h"
#include "components/Animation.h"
#include "components/Display.h"
#include "components/Health.h"
#include "components/SpaceShip.h"

using namespace entityx;

AnimationSystem::AnimationSystem()
{
}

void AnimationSystem::update(EntityManager& entities, EventManager&, double dt)
{
    entities.each<AnimationContainer, Display>([&dt](Entity, AnimationContainer& animationContainer, Display& display)
    {
        for (auto& p : animationContainer.getAnimations())
        {
            Animator& animator(p.second.second);

            if (animator.elapsedTime > animator.timePerFrame)
            {
                if (animator.currentIndex < animator.coordList.size()-1)
                {
                    animator.currentIndex++;
                    animator.elapsedTime = 0.0;
                }
                else if (animator.style == AnimationStyle::LOOP)
                {
                    animator.currentIndex = 0;
                    animator.elapsedTime = 0.0;
                }
            }
            else
                animator.elapsedTime += dt;

            display.coord = animator.coordList[animator.currentIndex];
        }
    });

    entities.each<Display>([&dt](Entity, Display& display)
    {
        auto& blink = display.blink;

        if (blink.blinkingTime > 0.0)
        {
            blink.blinkingTime -= dt;

            if (blink.timeSinceBlink > 100.0)
            {
                blink.isOff = !blink.isOff;
                blink.timeSinceBlink = 0.0;
            }
            else
                blink.timeSinceBlink += dt;

            if (blink.isOff)
                display.coord = sf::IntRect(0, 0, 0, 0);
        }
    });
}
