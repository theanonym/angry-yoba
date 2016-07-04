#include "angry_yoba/leveleditor/pickeditem.hpp"
#include "angry_yoba/gcommon.hpp"
#include "pukan/core/physics/world.hpp"
#include "pukan/core/physics/bodyiterator.hpp"
#include "pukan/core/physics/body.hpp"
#include "pukan/core/physics/fixture.hpp"
#include "pukan/core/graphics/graphics.hpp"
#include "pukan/core/entity.hpp"
#include "pukan/core/services/resources.hpp"
#include "pukan/core/services/renderer.hpp"
#include "pukan/core/services/config.hpp"
#include "pukan/core/services/logger.hpp"
#include "json/jparser.hpp"
#include "json/jobject.hpp"

PickedItem::PickedItem(YobaObject * parent, World * world)
   : YobaObject(parent),
     _world(world)
{
}

void PickedItem::onInit()
{
//   _state.mode = State::Block;
   _material = Materials::Wood;
   _enemyTexture = Resources::get().getTexture("doll1");
   _size = Vec2f(10, 50);
}

void PickedItem::onRender()
{
   Vec2f mousePos = Renderer::get().getGameMousePos();

   if(!_selectedItems.empty())
      for(auto it = _world->getBodyIterator(); it; it++)
      {
         const Body * body = it.get();
         if(std::find(_selectedItems.begin(),
                      _selectedItems.end(),
                      body->getId()) != _selectedItems.end())
         {
            Rectf aabb = body->getAABB();
            sf::RectangleShape selected(aabb.getSize());
            selected.setFillColor(sf::Color::Transparent);
            selected.setOutlineColor(sf::Color::Green);
            selected.setOutlineThickness(1);
            selected.setOrigin(aabb.getSize() / 2.f);
            selected.setPosition(body->getPos());
            Renderer::get().draw(selected);
         }
      }

   switch(_state.mode)
   {
      case State::Block:
      {

         sf::RectangleShape block(_size);
         block.setOutlineColor(sf::Color::Black);
         block.setOutlineThickness(-1);
         block.setTexture(Resources::get().getTexture(_material.getTextureName()));
         block.setOrigin(_size / 2.f);
         block.setRotation(_angle);
         block.setPosition(mousePos);

         if(int(block.getRotation()) % 90 == 0)
         {
            Rectf aabb = block.getGlobalBounds();
            Vec2f offset = aabb.getTopLeft() - aabb.getTopLeft().getRoundedTo(5);
            block.setPosition(mousePos - offset);
         }

         Renderer::get().draw(block);
      } break;

      case State::Polygon:
      {
         if(_state.polygonCreated)
         {
            sf::ConvexShape polygon(_polygonPoints.size());
            for(unsigned i = 0; i < _polygonPoints.size(); i++)
               polygon.setPoint(i, _polygonPoints[i]);
            polygon.setTexture(Resources::get().getTexture(_material.getTextureName()));
            polygon.setOutlineColor(sf::Color::Black);
            polygon.setOutlineThickness(-1);
            polygon.setPosition(mousePos);
            polygon.setRotation(_angle);
            Renderer::get().draw(polygon);
         }
         else
         {
            if(!_polygonPoints.empty())
            {
               sf::CircleShape point(3);
               point.setOrigin(3, 3);
               point.setFillColor(sf::Color::Green);

               Vec2f prevPos = _polygonPoints[0];
               for(const Vec2f & pos : _polygonPoints)
               {
                  point.setPosition(pos);
                  Renderer::get().draw(point);
                  if(_polygonPoints.size() > 1)
                     Renderer::get().drawLine(prevPos, pos);
                  prevPos = pos;
               }
            }
         }
      } break;

      case State::Enemy:
      {
         sf::Sprite enemy(*_enemyTexture);
         enemy.setOrigin(Rectf(enemy.getGlobalBounds()).getSize() / 2.f);
         enemy.setPosition(mousePos);
         Renderer::get().draw(enemy);
      } break;

      case State::Load:
      {
         sf::RectangleShape load(_loadSize);
         load.setFillColor(sf::Color(255, 0, 0, 100));
         load.setOutlineColor(sf::Color(255, 0, 0, 200));
         load.setOutlineThickness(-1);
         load.setPosition(mousePos);
         Renderer::get().draw(load);
      } break;

      case State::Selection:
      {
         sf::RectangleShape selection(mousePos - _selectionStart);
         selection.setFillColor(sf::Color(0, 255, 0, 100));
         selection.setOutlineColor(sf::Color(0, 255, 0, 200));
         selection.setOutlineThickness(-1);
         selection.setPosition(_selectionStart);
         Renderer::get().draw(selection);
      } break;

      default: break;
   }
}

void PickedItem::onLeftMousePressed(const sf::Event::MouseButtonEvent &)
{
   switch(_state.mode)
   {
      case State::Block:
      {
         Entity * entity = new Entity(new Body(_world, true),
                                      new Graphics(Resources::get().getTexture(_material.getTextureName()),
                                                   _size, true, true),
                                      "block");
         entity->getBody()->createRectangleFixture(_size, _material.getPhysics());
         entity->getBody()->getFirstFixture()->setFilter(Filters::Block);
         entity->getBody()->setAngle(_angle);
         entity->getBody()->setPos(Renderer::get().getGameMousePos());

         sf::RectangleShape block(_size);
         block.setOrigin(_size / 2.f);
         block.setRotation(_angle);
         block.setPosition(Renderer::get().getGameMousePos());
         if(int(block.getRotation()) % 90 == 0)
         {
            Rectf aabb = block.getGlobalBounds();
            Vec2f offset = aabb.getTopLeft() - aabb.getTopLeft().getRoundedTo(5);
            block.setPosition(Renderer::get().getGameMousePos() - offset);
         }

         entity->getBody()->setPos(block.getPosition());
      } break;

      case State::Polygon:
      {
         if(_state.polygonCreated)
         {
            Entity * entity = new Entity(new Body(_world, true),
                                         new Graphics(Resources::get().getTexture(_material.getTextureName())),
                                         "block");
            entity->getBody()->createPolygonFixture(_polygonPoints, _material.getPhysics());
            entity->getBody()->getFirstFixture()->setFilter(Filters::Block);
            entity->getBody()->setAngle(_angle);
            entity->getBody()->setPos(Renderer::get().getGameMousePos());
         }
         else
         {
            if(!_polygonPoints.empty() && sf::FloatRect(Renderer::get().getGameMousePos(), Vec2f(1, 1)).intersects(sf::FloatRect(_polygonPoints[0] - Vec2f(5, 5), Vec2f(10, 10))))
            {
               if(_polygonPoints.size() < 3 || _polygonPoints.size() > 8)
                  ythrow(std::runtime_error, "Слишком много или слишком мало точек для полигона.");

               Vec2f topLeftBound(std::numeric_limits<float>::max(),
                                 std::numeric_limits<float>::max());
               for(const Vec2f & p : _polygonPoints)
               {
                  if(p.x < topLeftBound.x) topLeftBound.x = p.x;
                  if(p.y < topLeftBound.y) topLeftBound.y = p.y;
               }

               for(Vec2f & p : _polygonPoints)
                  p -= topLeftBound;

               _state.polygonCreated = true;
            }
            else
            {
               _polygonPoints.push_back(Renderer::get().getGameMousePos());
            }
         }
      } break;

      case State::Enemy:
      {
         Animation * a1 =
               new Animation(Resources::get().getTexture("doll1_dead"),
                             Vec2i(0, 0), Vec2i(35, 50), 1, 1.f, true);
         Animation * a2 =
               new Animation(Resources::get().getTexture("doll1_dead"),
                             Vec2i(35, 0), Vec2i(35, 50), 3, 0.1f, false);

         Vec2f size = a1->getCurrentFrameRect().getSize();

         Entity * entity = new Entity(new Body(_world, true),
                                      new Graphics(new AnimationMgr({ a1, a2 })),
                                      "doll");
         entity->getBody()->createRectangleFixture(a1->getCurrentFrameRect().getSize(),
                                                   Materials::Wood.getPhysics());
         entity->getBody()->setPos(Renderer::get().getGameMousePos());



         float offset = 3;
         const std::vector<Vec2f> sensor {
            Vec2f(-size.x / 2 - offset, -size.y / 2 - offset),
            Vec2f(+size.x / 2 + offset, -size.y / 2 - offset),
            Vec2f(+size.x / 2 + offset, +size.y / 2 - 3),
            Vec2f(-size.x / 2 - offset, +size.y / 2 - 3),
         };
         entity->getBody()->createPolygonFixture(sensor)->setSensor(true);


         entity->getBody()->getFirstFixture()->setFilter(Filters::Enemy);
         entity->getBody()->getLastFixture()->setFilter(Filters::EnemySensor);
      } break;

      case State::Load:
      {
         _world->loadFromFile(Config::get().workingDir / "levels/1.json",
                              Renderer::get().getGameMousePos());
      } break;

      default: break;
   }
}

void PickedItem::onMouseWheelMoved(const sf::Event::MouseWheelEvent & event)
{
   switch(_state.mode)
   {
      case State::Block:
      case State::Polygon:
      {
         if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            _size.y += event.delta * 5;
         else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            _size.x += event.delta * 5;
         else
            _angle += event.delta * 10;

         if(_size.x < 5)   _size.x = 5;
         if(_size.y < 5)   _size.y = 5;
         if(_size.x > 200) _size.x = 200;
         if(_size.y > 200) _size.y = 200;
      } break;

      default: break;
   }
}

void PickedItem::onRightMousePressed(const sf::Event::MouseButtonEvent & event)
{
   _state.mode = State::Selection;
   _selectionStart = Vec2f(event.x, event.y);
}

void PickedItem::onRightMouseReleased(const sf::Event::MouseButtonEvent & event)
{
   switch(_state.mode)
   {
      case State::Selection:
      {
         _state.mode = State::Block;

         Rectf selectionArea(_selectionStart, Vec2f(event.x, event.y) - _selectionStart);

         if(selectionArea.w < 0)
         {
            selectionArea.w = std::abs(selectionArea.w);
            selectionArea.x -= selectionArea.w;
         }

         if(selectionArea.h < 0)
         {
            selectionArea.h = std::abs(selectionArea.h);
            selectionArea.y -= selectionArea.h;
         }

         std::vector<Body *> selected(_world->findByRect(selectionArea));
         if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            _selectedItems.clear();
         for(auto body : selected)
            _selectedItems.push_back(body->getId());
      } break;

      default: break;
   }
}

void PickedItem::onKeyPressed(const sf::Event::KeyEvent & event)
{
   switch(event.code)
   {
      case sf::Keyboard::Q:
         _state.mode = State::Block;
         break;

      case sf::Keyboard::W:
         _state.mode = State::Polygon;
         _state.polygonCreated = false;
         _polygonPoints.clear();
         break;

      case sf::Keyboard::E:
         _state.mode = State::Enemy;
         break;

      case sf::Keyboard::R:
      {
         _state.mode = State::Load;
         json::Parser parser;
         parser.readFile("levels/1.json");
         _loadSize = Rectf::fromJson(parser.getRoot().get("aabb")).getSize();
      } break;

      case sf::Keyboard::Num1: _material = Materials::Wood; break;
      case sf::Keyboard::Num2: _material = Materials::Steel; break;
      case sf::Keyboard::Num3: _material = Materials::Stone; break;
      case sf::Keyboard::Num4: _material = Materials::Textile; break;

      case sf::Keyboard::S:
         _world->saveToFile(Config::get().workingDir / "levels/1.json");
         break;

      case sf::Keyboard::D:
      {
         if(!_selectedItems.empty())
            for(auto id : _selectedItems)
               delete _world->findById(id)->getEntity();
//            {
//               try { delete _world->findById(id)->getEntity(); }
//               catch(std::exception & e) { Logger::get().error() << e.what(); }
//            }
      } break;

      default:
         break;
   }
}
