#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "pukan/core/yobaobject.hpp"
#include "pukan/scene/scenewithgui.hpp"

using namespace pukan;

class NewtonCradle;

class MainMenu : public YobaObject, public SceneWithGUI
{
public:
   MainMenu();

public:
   void onInit();
   void onUpdate();
   void onRender();

protected:
   void onKeyPressed(const sf::Event::KeyEvent & key);

private:
   NewtonCradle * _nc = nullptr;
};

#endif // MAINMENU_HPP
