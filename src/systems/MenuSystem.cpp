#include "systems/MenuSystem.h"
#include "systems/Events.h"
#include "components/Menu.h"
#include "KeyHandler.h"

using namespace entityx;
MenuSystem::MenuSystem(EntityManager& entities,
                       EventManager& eventManager,
                       sf::RenderWindow& window)
    : m_entitiyManager(entities)
    , m_eventManager(eventManager)
    , m_window(window)
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
    entities.each<Menu>([this, &events, &dt](Entity, Menu& menu)
    {
        menu.spMenu->update(events, dt);
        menu.spMenu->draw(m_window);
    });
}

void MenuSystem::receive(const EvKeyboard& keyboard)
{
    int activeMenus = 0;
    m_entitiyManager.each<Menu>([this, &keyboard, &activeMenus](Entity, Menu& menu)
    {
        if (keyboard.isDown)
            menu.spMenu->onKey(keyboard.key);

        if(keyboard.key == sf::Keyboard::Return && keyboard.isDown)
            menu.spMenu->onConfirm(m_eventManager);
        else if(keyboard.key == sf::Keyboard::Escape && keyboard.isDown)
            menu.spMenu->onCancel(m_eventManager);
        else if(keyboard.key == sf::Keyboard::Up && keyboard.isDown)
            menu.spMenu->onUp(m_eventManager);
        else if(keyboard.key == sf::Keyboard::Down && keyboard.isDown)
            menu.spMenu->onDown(m_eventManager);
        else if(keyboard.key == sf::Keyboard::Left && keyboard.isDown)
            menu.spMenu->onLeft(m_eventManager);
        else if(keyboard.key == sf::Keyboard::Right && keyboard.isDown)
            menu.spMenu->onRight(m_eventManager);
        activeMenus++;
    });

    if( !activeMenus ){
        if(keyboard.key == sf::Keyboard::Escape && keyboard.isDown)
            m_eventManager.emit<EvPauseGame>();
    }

    // Don't really know where to put stuff like this
    // Pretty sure it's not menu's business
    if(keyboard.key == sf::Keyboard::F1  && keyboard.isDown)
    {
        m_eventManager.emit<EvMusicVolume>(0);
    }

    if(keyboard.key == sf::Keyboard::F2  && keyboard.isDown)
    {
        m_eventManager.emit<EvQuitGame>();
    }

}
