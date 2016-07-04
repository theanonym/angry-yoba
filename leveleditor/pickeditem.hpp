#ifndef PICKEDITEM_HPP
#define PICKEDITEM_HPP

#include "pukan/core/yobaobject.hpp"
#include "pukan/core/physics/material.hpp"

namespace pukan { class World; }

using namespace pukan;

class PickedItem : public YobaObject
{
public:
   PickedItem(YobaObject * parent, World * world);

   void onInit();
   void onRender();

   void onKeyPressed(const sf::Event::KeyEvent & event);
   void onLeftMousePressed(const sf::Event::MouseButtonEvent & event);
   void onRightMousePressed(const sf::Event::MouseButtonEvent & event);
   void onRightMouseReleased(const sf::Event::MouseButtonEvent & event);
   void onMouseWheelMoved(const sf::Event::MouseWheelEvent & event);

private:
   struct State
   {
      enum
      {
         Block,
         Polygon,
         Enemy,
         Load,
         Selection
      } mode;

      bool polygonCreated = false;
   } _state;

private:
   World * _world;

   Vec2f  _size;
   float _angle = 0;
   Material _material;
   const sf::Texture * _enemyTexture = nullptr;
   std::vector<Vec2f> _polygonPoints;
   Vec2f _loadSize;

   Vec2f _selectionStart;
   std::vector<int> _selectedItems;
};

#endif // PICKEDITEM_HPP
