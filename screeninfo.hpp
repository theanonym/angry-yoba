#ifndef SCREENINFO_HPP
#define SCREENINFO_HPP

#include "pukan/common.hpp"
#include "pukan/math/math.hpp"

using namespace pukan;

class ScreenInfo : public sf::Drawable
{
public:
   ScreenInfo(const sf::Vector2f & pos,
              const sf::Font * font,
              const sf::Color & color,
              unsigned charSize);

   void draw(sf::RenderTarget & renderTarget, sf::RenderStates renderStates) const;

   void setString(const sf::String & string);

private:
   std::unique_ptr<sf::Text> _text;
};

#endif // SCREENINFO_HPP
