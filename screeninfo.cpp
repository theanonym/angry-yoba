#include "angry_yoba/screeninfo.hpp"

ScreenInfo::ScreenInfo(const sf::Vector2f & pos,
                       const sf::Font * font,
                       const sf::Color & color,
                       unsigned charSize)
   : _text(new sf::Text("", *font, charSize))
{
   _text->setColor(color);
   _text->setPosition(pos);
}

void ScreenInfo::draw(sf::RenderTarget & renderTarget, sf::RenderStates renderStates) const
{
   Vec2f originalPos(_text->getPosition());
   _text->setPosition(renderTarget.mapPixelToCoords(Vec2i(originalPos)));
   renderTarget.draw(*_text, renderStates);
   _text->setPosition(originalPos);
}

void ScreenInfo::setString(const sf::String & string)
{
   _text->setString(string);
}
