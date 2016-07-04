#include "angry_yoba/leveleditor/leveleditor.hpp"
#include "angry_yoba/leveleditor/pickeditem.hpp"
#include "angry_yoba/state.hpp"
#include "angry_yoba/gcommon.hpp"
#include "pukan/core/services/resources.hpp"
#include "pukan/core/services/renderer.hpp"
#include "pukan/core/services/config.hpp"
#include "pukan/core/entity.hpp"
#include "pukan/core/physics/world.hpp"
#include "pukan/core/physics/body.hpp"
#include "pukan/core/physics/fixture.hpp"
#include "pukan/core/physics/bodyiterator.hpp"
#include "pukan/core/graphics/graphics.hpp"
#include "json/jparser.hpp"

LevelEditor::LevelEditor()
   : SceneWithWorld(this)
{
   _picked = new PickedItem(this, getWorld());
}

void LevelEditor::onInit()
{
   YobaObject::onInit();
}

void LevelEditor::onReset()
{
   Vec2f winsize = Renderer::get().getWindow().getSize();

   getWorld()->removeAll();


//   Entity * leftWall = new Entity(new Body(getWorld(), false),
//                                  new Graphics(Resources::get().getTexture("black")));
//   leftWall->getBody()->createRectangleFixture(Vec2f(5, winsize.y - 10));
//   leftWall->getBody()->getFirstFixture()->setCategoryFlags(Collision::Wall);
//   leftWall->getBody()->setPos(0, 5, false);
//   Entity * rightWall = leftWall->clone();
//   rightWall->getBody()->setPos(winsize.x - 5, 5, false);

   Entity * upperWall = new Entity(new Body(getWorld(), false),
                                   new Graphics(Resources::get().getTexture("black")));
   upperWall->getBody()->createRectangleFixture(Vec2f(winsize.x, 5));
   upperWall->getBody()->setPos(0, 0, false);
   Entity * bottomWall = upperWall->clone();
   bottomWall->getBody()->setPos(0, winsize.y - 5, false);

   bottomWall->getBody()->getFirstFixture()->setFilter(Filters::Wall);

   YobaObject::onReset();
}

void LevelEditor::onUpdate()
{
   if(State::get().editor.reset)
   {
      onReset();

      State::get().editor.reset = false;
   }

   if(State::get().editor.simulation)
      YobaObject::onUpdate();
}

void LevelEditor::onRender()
{
   Renderer::get().getCamera().setSize(Vec2f(Renderer::get().getWindow().getSize()));
   Renderer::get().getCamera().setCenter(Renderer::get().getWindow().getSize().x / 2.f,
                                         Renderer::get().getWindow().getSize().y / 2.f);


   Renderer::get().fill(sf::Color(255, 255, 148));


   // Сетка
   //--------------------------------------------
   float offset = 5;
   Vec2f windowSize = Renderer::get().getWindow().getSize();
   sf::RectangleShape line;
   line.setFillColor(sf::Color::Black);

   line.setSize(Vec2f(windowSize.x, 1));
   for(int y = 0; y < windowSize.y / offset; y++)
   {
      line.setPosition(0, 5 + y * offset);
      Renderer::get().draw(line);
   }

   line.setSize(Vec2f(1, windowSize.y));
   for(int x = 0; x < windowSize.x / offset; x++)
   {
      line.setPosition(5 + x * offset, 0);
      Renderer::get().draw(line);
   }
   //--------------------------------------------

   for(auto it = getWorld()->getBodyIterator(); it; it++)
      Renderer::get().draw(*it.get()->getEntity());

   YobaObject::onRender();
}

void LevelEditor::onKeyPressed(const sf::Event::KeyEvent & event)
{
   switch(event.code)
   {
      case sf::Keyboard::Escape:
         State::get().screen = State::MainMenu;
         return;

      case sf::Keyboard::F5:
         State::get().editor.reset = true;
         return;

      case sf::Keyboard::D:
      case sf::Keyboard::Delete:
      {
         Body * x = getWorld()->findFirstByPos(Renderer::get().getGameMousePos());
         if(x) delete x->getEntity();
      } break;

      case sf::Keyboard::BackSpace:
         delete getWorld()->getLast()->getEntity();
         break;

      case sf::Keyboard::Space:
         State::get().editor.simulation = !State::get().editor.simulation;
         break;

      default:
         break;
   }
}

