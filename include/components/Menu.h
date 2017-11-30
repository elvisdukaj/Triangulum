#ifndef MENU_H
#define MENU_H

#include "entityx/Entity.h"
#include "components/AbstractMenu.h"

struct Menu : public entityx::Component<Menu>
{
   Menu(const IMenuSP& spMenu);

   IMenuSP spMenu;
};

#endif // MENU_H
