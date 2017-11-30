#ifndef ANIMATION_H
#define ANIMATION_H

#include <map>
#include "SFML/Graphics/Rect.hpp"
#include "entityx/Entity.h"

enum class AnimationStyle
{
    RUN_ONCE,
    LOOP
};

enum class AnimationType
{
    Death,
    Movement
};

using SpriteCoordList = std::vector<sf::IntRect>;

struct Animator
{
    Animator();

    double elapsedTime;
    double timePerFrame;

    AnimationStyle style;
    SpriteCoordList coordList;
    size_t currentIndex;
};

using Animation = uint8_t;
using AnimationId = std::pair<AnimationType, Animation>;

class AnimationContainer : public entityx::Component<AnimationContainer> {
public:
    using AnimationMap = std::map<AnimationId, Animator>;
    using ActiveAnimator = std::pair<Animation, Animator>;
    using ActiveAnimatorMap = std::map<AnimationType, ActiveAnimator>;

    AnimationContainer();

    void addAnimation(const AnimationId& id, const Animator& animator);
    void setAnimation(const AnimationId& id);
    void resetAnimation(AnimationType type);

    ActiveAnimatorMap& getAnimations();

private:
    AnimationMap m_registeredAnimations;
    ActiveAnimatorMap m_activeAnimations;
};

static const Animation NoAction = 0;

// Death animation id's
static const Animation DestroyedDeathAnimation = 1;

// Movement animation id's
static const Animation IdleMovementAnimation  = 1;
static const Animation LeftMovementAnimation  = 2;
static const Animation RightMovementAnimation = 3;

#endif // ANIMATION_H
