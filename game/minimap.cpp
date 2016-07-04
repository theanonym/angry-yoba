#include "angry_yoba/game/minimap.hpp"
#include "pukan/core/physics/world.hpp"
#include "pukan/core/physics/body.hpp"
#include "pukan/core/services/renderer.hpp"

MiniMap::MiniMap(YobaObject * parent, World * world)
   : YobaObject(parent),
     _world(world)
{
}

void MiniMap::setPos(const Vec2f & pos)
{
   _pos = pos;
}

const Vec2f & MiniMap::getPos()
{
   return _pos;
}

void MiniMap::onRender()
{
	
   sf::RectangleShape miniBackground;
   miniBackground.setFillColor(sf::Color(255, 255, 148));
   miniBackground.setSize(Vec2f(400, 100));
   miniBackground.setPosition(Renderer::get().getWindow().mapPixelToCoords(Vec2i(849, 509), Renderer::get().getCamera()));
   renderTarget.draw(miniBackground, renderStates);
   for(const b2Body * body = _world->getB2World()->GetBodyList(); body; body = body->GetNext())
   {
      const Body * object = static_cast<const Body *>(body->GetUserData());
      //      if(object->getName() == "ground") continue;
      Renderer::get().drawBody(object, Vec2f(0.2, 0.2));
   }

   sf::RectangleShape cameraRect;
   cameraRect.setFillColor(sf::Color::Transparent);
   cameraRect.setOutlineColor(sf::Color::Red);
   cameraRect.setOutlineThickness(-1);

   float cameraMinPos = renderTarget.getView().getCenter().x - renderTarget.getSize().x / 2;
   float cameraMaxPos = 750; //2000 - Renderer::get().getWindow().getSize().x / 2;
   float cameraCoef = cameraMinPos / cameraMaxPos;

   cameraRect.setSize(Vec2f(Renderer::get().getWindow().getSize().x * 0.2 + 2, 103));
   cameraRect.setPosition(renderTarget.mapPixelToCoords(Vec2i(848 + 150 * cameraCoef,
                                                               507),
                                                  Renderer::get().getCamera()));
   renderTarget.draw(cameraRect, renderStates);

}

void MiniMap::onMousePressed(const sf::Event::MouseButtonEvent & event)
{
   if(event.button == sf::Mouse::Right)
   {
   }
}
