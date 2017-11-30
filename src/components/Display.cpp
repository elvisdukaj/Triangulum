#include "components/Display.h"

Blink::Blink()
: blinkingTime(0.0)
, timeSinceBlink(0.0)
, isOff(false)
{
}

void Blink::startBlink(double blinkingTime)
{
   blinkingTime = blinkingTime;
   timeSinceBlink = 0.0;
   isOff = false;
}

Display::Display()
: coord()
, blink()
{
}

Display::Display(const sf::IntRect& coord)
: coord(coord)
, color(sf::Color(255, 255, 255, 255))
, blink()
{
}

Display::Display(const sf::IntRect& coord, sf::Color color)
: coord(coord)
, color(color)
, blink()
{
}

