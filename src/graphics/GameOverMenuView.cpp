#include <iostream>
#include "SDL_image.h"
#include "graphics/GameOverMenuView.h"
#include "components/Position.h"
#include "systems/Events.h"
#include "KeyHandler.h"

using namespace entityx;

GameOverMenuView::GameOverMenuView()
: m_texture()
, m_halfLogoWidth(0.0)
, m_halfLogoHeight(0.0)
{
   SDL_Surface* surface = IMG_Load("../images/game_over.png");

   if (!surface)
   {
      std::cout << "IMG_Load error " << IMG_GetError() << std::endl;
   }

   m_halfLogoWidth = surface->w/2.0;
   m_halfLogoHeight = surface->h/2.0;

   glGenTextures(1, &m_texture);

   glBindTexture(GL_TEXTURE_2D, m_texture);

   glTexImage2D(GL_TEXTURE_2D,
                0,
                GL_RGBA,
                surface->w,
                surface->h,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                surface->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   SDL_FreeSurface(surface);
}

void GameOverMenuView::update(const KeyHandler &keyHandler,
                              EventManager &eventManager)
{
   if (keyHandler.isPressed(SDLK_SPACE))
   {
      eventManager.emit<EvStartGame>();
   }
}

void GameOverMenuView::draw()
{
   glBindTexture(GL_TEXTURE_2D, m_texture);

   glEnable(GL_TEXTURE_2D);

   glBegin(GL_QUADS);

   glColor3f(1.0f, 1.0f, 1.0f);

   glTexCoord2f(0.0f, 1.0f);
   glVertex3f(-m_halfLogoWidth, m_halfLogoHeight, 0.0f);
   glTexCoord2f(1.0f, 1.0f);
   glVertex3f(m_halfLogoWidth, m_halfLogoHeight, 0.0f);
   glTexCoord2f(1.0f, 0.0f);
   glVertex3f(m_halfLogoWidth, -m_halfLogoHeight, 0.0f);
   glTexCoord2f(0.0f, 0.0f);
   glVertex3f(-m_halfLogoWidth, -m_halfLogoHeight, 0.0f);

   glEnd();

   glDisable(GL_TEXTURE_2D);
}


void GameOverMenuView::onConfirm(entityx::EventManager& eventManager){
    eventManager.emit<EvStartGame>();
}

void GameOverMenuView::onCancel(entityx::EventManager& eventManager){
    // Nothing
}
