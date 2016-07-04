#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "pukan/core/yobaobject.hpp"
#include "pukan/scene/scenewithgui.hpp"

using namespace pukan;

class Settings : public YobaObject, public SceneWithGUI
{
public:
   Settings();

   void onInit();
   void onReset();
   void onRender();

   void onKeyPressed(const sf::Event::KeyEvent & event);
};

#endif // SETTINGS_HPP
