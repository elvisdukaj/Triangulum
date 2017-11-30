#ifndef PLAYERCONTROLSYSTEM_H
#define PLAYERCONTROLSYSTEM_H

#include "entityx/System.h"
#include "SFML/System/Vector2.hpp"

class KeyHandler;

class PlayerControlSystem : public entityx::System<PlayerControlSystem> {
public:
   PlayerControlSystem(const KeyHandler& keyHandler);

   void update(entityx::EntityManager &entities,
               entityx::EventManager &events,
               double dt);

private:
   const KeyHandler& m_keyHandler;
};

#endif
