#ifndef NEWTONCRADLE_HPP
#define NEWTONCRADLE_HPP

#include "pukan/common.hpp"
#include "pukan/core/yobaobject.hpp"
#include "pukan/scene/scenewithworld.hpp"

using namespace pukan;

class NewtonCradle : public YobaObject, public SceneWithWorld
{
public:
   NewtonCradle(YobaObject * parent);

   void onInit();
   void onUpdate();
   void onRender();

   void onBeginContact(b2Contact * contact);
};

#endif // NEWTONCRADLE_HPP
