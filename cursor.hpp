#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "pukan/common.hpp"
#include "pukan/math/math.hpp"

using namespace pukan;

class Cursor : public sf::Drawable
{
public:
   Cursor(const sf::Texture * texture);

   void draw(sf::RenderTarget & renderTarget, sf::RenderStates renderStates) const;

   void setPos(const Vec2f & mousePos);

private:
   const sf::Texture * _texture;
   Vec2f _pos;
};

inline void Cursor::setPos(const Vec2f & mousePos)
{
   _pos = mousePos;
}

#endif // CURSOR_HPP
