#ifndef STARTMENUVIEW_H
#define STARTMENUVIEW_H

#include "components/IMenu.h"

class StartMenuView
      : public IMenu
{
public:

   StartMenuView();

   void update(const KeyHandler &keyHandler,
               entityx::EventManager& eventManager);

   void draw();

   void onConfirm(entityx::EventManager& eventManager);

   void onCancel(entityx::EventManager &eventManager);

private:

   GLuint m_texture;

   float m_halfLogoWidth;

   float m_halfLogoHeight;

};

#endif // STARTMENUVIEW_H
