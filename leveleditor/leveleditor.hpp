#ifndef LEVELEDITOR_HPP
#define LEVELEDITOR_HPP

#include "pukan/common.hpp"
#include "pukan/core/yobaobject.hpp"
#include "pukan/scene/scenewithworld.hpp"

using namespace pukan;

class PickedItem;

class LevelEditor : public YobaObject, public SceneWithWorld
{
public:
   LevelEditor();

   void onInit();
   void onReset();
   void onUpdate();
   void onRender();


private:
   void onKeyPressed(const sf::Event::KeyEvent & event);

private:
   PickedItem * _picked;
};

#endif // LEVELEDITOR_HPP
