#include "systems/MenuSystem.h"
#include "systems/Events.h"
#include "components/Menu.h"
#include "KeyHandler.h"
#include "SDL.h"

using namespace entityx;
MenuSystem::MenuSystem(EntityManager &entities, EventManager &eventManager)
    : m_entitiyManager(entities)
    , m_eventManager(eventManager)
{
}

void MenuSystem::configure(EventManager& eventManager)
{
   eventManager.subscribe<EvKeyboard>(*this);
}

void MenuSystem::update(EntityManager& entities,
                        EventManager& events,
                        double dt)
{
}

void MenuSystem::receive(const EvKeyboard& keyboard)
{
    Menu::Handle menu;
    int activeMenus = 0;
    for (Entity entity : m_entitiyManager.entities_with_components(menu))
    {
       if(keyboard.id == SDLK_SPACE && keyboard.isDown)
          menu->spMenu->onConfirm(m_eventManager);    
       else if(keyboard.id == SDLK_ESCAPE && keyboard.isDown)
          menu->spMenu->onCancel(m_eventManager);
       activeMenus++;
    }

    if( !activeMenus ){
        if(keyboard.id == SDLK_ESCAPE && keyboard.isDown)
           m_eventManager.emit<EvPauseGame>();
    }

    // Don't really know where to put stuff like this
    // Pretty sure it's not menu's business
    if(keyboard.id == SDLK_m  && keyboard.isDown)
    {
        m_eventManager.emit<EvPauseMusic>();
    }
}
