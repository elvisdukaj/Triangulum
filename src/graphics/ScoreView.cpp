#include <sstream>
#include <vector>
#include <algorithm>
#include "graphics/ScoreView.h"

ScoreView::ScoreView(const sf::Font& font, int fontSize)
    : m_maxScore(1000)
    , m_maxNrOfDigits(4)
    , m_align(TextAlign::right)
    , m_scoreText()
{
   m_scoreText.setFont(font);
   m_scoreText.setCharacterSize(fontSize);
   m_scoreText.setString("0");
   updateAlign();
}

void ScoreView::setAlign(TextAlign align)
{
   m_align = align;
   updateAlign();
}

void ScoreView::setPosition(float x, float y)
{
   m_scoreText.setPosition(x, y);
}

void ScoreView::draw(int score, sf::RenderWindow& window)
{
   if (score > m_maxScore)
      score = m_maxScore;

   m_scoreText.setString(std::to_string(score));
   updateAlign();
   window.draw(m_scoreText);

}

void ScoreView::updateAlign()
{
   sf::FloatRect bounds(m_scoreText.getLocalBounds());
   m_scoreText.setOrigin(bounds.width/2.0, bounds.height/2.0);

   switch(m_align) {
   case TextAlign::right:
       m_scoreText.setOrigin(0.0, bounds.height/2.0);
       break;

   case TextAlign::center:
       m_scoreText.setOrigin(bounds.width/2.0, bounds.height/2.0);
       break;

   case TextAlign::left:
       m_scoreText.setOrigin(bounds.width, bounds.height/2.0);
       break;
    }
}
