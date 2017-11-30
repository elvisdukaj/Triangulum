#ifndef SCOREVIEW_H
#define SCOREVIEW_H

#include <map>
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Text.hpp"

enum class TextAlign {right, center, left };

class ScoreView {
public:
   ScoreView(const sf::Font& font, int fontSize);

   void setAlign(TextAlign align);
   void setPosition(float x, float y);
   void draw(int score, sf::RenderWindow& window);

private:
   void updateAlign();

private:
   const int m_maxScore;
   const int m_maxNrOfDigits;
   TextAlign m_align;
   sf::Text m_scoreText;
};

#endif // SCOREVIEW_H
