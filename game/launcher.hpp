#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include "pukan/core/yobaobject.hpp"

using namespace pukan;

class Launcher : public YobaObject
{
public:
   Launcher(YobaObject * parent);

   void setPos(const Vec2f & pos);
   const Vec2f & getPos() const;

   bool isValidPos() const;
   float getPowerFactor() const;
   float getAngleFactor() const;

public:
   void onInit();
   void onRender();

private:
   void onMousePressed(const sf::Event::MouseButtonEvent & event);
   void onMouseReleased(const sf::Event::MouseButtonEvent & event);
   void onMouseMoved(const sf::Event::MouseMoveEvent & event);

   Vec2f _pos;
};

#endif // LAUNCHER_HPP
