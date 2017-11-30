#ifndef VOLUME_H
#define VOLUME_H

#include "entityx/Entity.h"
#include "utils/CollisionBox.h"

struct Volume : public entityx::Component<Volume>
{
   Volume();

   using BoxList = std::vector<CollisionBox>;

   BoxList m_boxes;
};

#endif // VOLUME_H
