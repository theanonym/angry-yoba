#ifndef LEVELSELECT_HPP
#define LEVELSELECT_HPP

#include "pukan/core/yobaobject.hpp"
#include "pukan/scene/scenewithgui.hpp"

using namespace pukan;

class LevelSelect : public YobaObject, public SceneWithGUI
{
public:
   LevelSelect();

public:
   void onInit();
   void onRender();

private:
   void onKeyPressed(const sf::Event::KeyEvent & event);
};

#endif // LEVELSELECT_HPP
