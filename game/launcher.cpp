#include "angry_yoba/game/launcher.hpp"
#include "angry_yoba/state.hpp"
#include "pukan/core/services/renderer.hpp"
#include "pukan/core/services/resources.hpp"

Launcher::Launcher(YobaObject * parent)
   : YobaObject(parent)
{
}

void Launcher::setPos(const Vec2f & pos)
{
   _pos = pos;
}

const Vec2f & Launcher::getPos() const
{
   return _pos;
}

bool Launcher::isValidPos() const
{
   Vec2f mousePos = Renderer::get().getGameMousePos();
   return mousePos.x < _pos.x && mousePos.x > 5 && mousePos.y < 500;
}

float Launcher::getPowerFactor() const
{
   if(!isValidPos())
      return 0;

   Vec2f mousePos = Renderer::get().getGameMousePos();
   float distance = std::sqrt(std::pow(mousePos.x - _pos.x, 2) + std::pow(mousePos.y - _pos.y, 2));

   float factor = distance / 200;
   if(factor > 1)
      factor = 1;

   return factor;
}


float Launcher::getAngleFactor() const
{
   if(!isValidPos())
      return 0;

   // -90 .. -180
   // 180 .. 90
   Vec2f mousePos = Renderer::get().getGameMousePos();
   float angle = std::atan2(mousePos.y - _pos.y + 20, mousePos.x - _pos.x) * 180 / b2_pi;

   float factor = angle < 0 ? (angle - -180) / -90 : (180 - angle) / 90;

//   P(angle  << " " << factor);

   return factor;
}

void Launcher::onInit()
{
}

void Launcher::onRender()
{
   if(State::get().game.yobas == 0)
      return;

   sf::RectangleShape rs;
   rs.setFillColor(sf::Color::Black);
   rs.setSize(Vec2f(3, _pos.y - 100));
   rs.setPosition(_pos(-1, 0));
   Renderer::get().draw(rs);

   Vec2f mousePos = Renderer::get().getGameMousePos();
   if(isValidPos())
   {



      sf::Texture * yobaTexture = Resources::get().getTexture("yoba" + b::lexical_cast<std::string>(4 - State::get().game.yobas));

      float radius = 20;
      float scale  = radius * 2 / yobaTexture->getSize().x;

      sf::Sprite yoba(*yobaTexture);
      yoba.setOrigin(radius / scale, radius / scale);
      yoba.setScale(scale, scale);

      if(State::get().launcher.touching)
      {
         yoba.setPosition(mousePos);
         Renderer::get().drawLine(getPos() - Vec2f(0, 20), mousePos);
      }
      else
      {
         yoba.setPosition(Vec2f(rs.getPosition()) - Vec2f(1, 20));
      }
      Renderer::get().draw(yoba);
   }
}

void Launcher::onMousePressed(const sf::Event::MouseButtonEvent &)
{

}

void Launcher::onMouseReleased(const sf::Event::MouseButtonEvent &)
{

}

void Launcher::onMouseMoved(const sf::Event::MouseMoveEvent &)
{

}


