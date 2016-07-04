#ifndef MINIMAP_HPP
#define MINIMAP_HPP

#include "pukan/core/yobaobject.hpp"
#include "pukan/core/physics/world.hpp"

namespace pukan

class MiniMap : public YobaObject
{
public:
   MiniMap(YobaObject * parent, World * world);

   void setPos(const Vec2f & pos);
   const Vec2f & getPos();

public:
   void onRender();

private:
   void onMousePressed(const sf::Event::MouseButtonEvent & event);

   World * _world;
   Vec2f _pos;
};

#endif // MINIMAP_HPP
