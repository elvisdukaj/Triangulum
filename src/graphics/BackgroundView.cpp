#include "SDL_image.h"
#include "SDL_opengl.h"
#include "graphics/BackgroundView.h"
#include "components/Position.h"

BackgroundView::BackgroundView()
: m_texture()
, m_ratio(800.0/600.0)
, m_textureOffset(0.0)
, m_textureHeightStart(1.0/3.0)
{
   SDL_Surface* surface = IMG_Load("../images/bg.jpg");

   if (!surface)
   {
      std::cout << "IMG_Load error " << IMG_GetError() << std::endl;
   }

   glGenTextures(1, &m_texture);

   glBindTexture(GL_TEXTURE_2D, m_texture);

   glTexImage2D(GL_TEXTURE_2D,
                0,
                GL_RGBA,
                surface->w,
                surface->h,
                0,
                GL_BGRA,
                GL_UNSIGNED_BYTE,
                surface->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   SDL_FreeSurface(surface);
}

void BackgroundView::draw(const Position& pos)
{
   const float offset = pos.position.y();

   // chose to work with this spriteSheets texture.
   glBindTexture(GL_TEXTURE_2D, m_texture);

   // time to use textures!
   glEnable(GL_TEXTURE_2D);

   glBegin(GL_QUADS);

   glColor3f(1.0f, 1.0f, 1.0f);

   glTexCoord2f(0.0f, m_textureHeightStart+offset);
   glVertex3f(-1.0f - m_ratio, 1.0f, -0.5f);
   glTexCoord2f(1.0f, m_textureHeightStart+offset);
   glVertex3f(1.0f + m_ratio, 1.0f, -0.5f);
   glTexCoord2f(1.0f, offset);
   glVertex3f(1.0f + m_ratio, -1.0f, -0.5f);
   glTexCoord2f(0.0f, offset);
   glVertex3f(-1.0f - m_ratio, -1.0f, -0.5f);

   glEnd();

   glDisable(GL_TEXTURE_2D);
}