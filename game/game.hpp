#ifndef GAME_HPP
#define GAME_HPP

#include "pukan/core/yobaobject.hpp"
#include "pukan/scene/scenewithworld.hpp"
#include "pukan/scene/scenewithgui.hpp"

using namespace pukan;

class Game : public YobaObject, public SceneWithWorld, public SceneWithGUI
{
public:
   Game();

   void shot();
   void loadLevel();

public:
   void onInit();
   void onReset();
   void onUpdate();
   void onRender();

private:
   void onKeyPressed(const sf::Event::KeyEvent & event);
   void onKeyReleased(const sf::Event::KeyEvent & event);
   void onLeftMousePressed(const sf::Event::MouseButtonEvent & event);
   void onLeftMouseReleased(const sf::Event::MouseButtonEvent & event);
   void onRightMousePressed(const sf::Event::MouseButtonEvent & event);
   void onRightMouseReleased(const sf::Event::MouseButtonEvent & event);
   void onMiddleMousePressed(const sf::Event::MouseButtonEvent & event);
   void onMouseMoved(const sf::Event::MouseMoveEvent & event);
   void onMouseWheelMoved(const sf::Event::MouseWheelEvent & event);

   void onBeginContact(b2Contact * b2contact);
   void onPreSolveContact(b2Contact * b2contact);

private:
//   class MiniMap  * _minimap = nullptr;
   class Launcher * _launcher = nullptr;

//   class Body * _lastYoba    = nullptr;
};

#endif // GAME_HPP
