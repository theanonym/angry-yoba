#include "angry_yoba/game/game.hpp"
#include "angry_yoba/gcommon.hpp"
//#include "angry_yoba/game/minimap.hpp"
#include "angry_yoba/game/launcher.hpp"
#include "angry_yoba/state.hpp"
#include "pukan/core/physics/world.hpp"
#include "pukan/core/physics/bodyiterator.hpp"
#include "pukan/core/physics/body.hpp"
#include "pukan/core/physics/fixture.hpp"
#include "pukan/core/graphics/graphics.hpp"
#include "pukan/core/entity.hpp"
#include "pukan/core/services/renderer.hpp"
#include "pukan/core/services/resources.hpp"
#include "pukan/core/services/logger.hpp"
#include "pukan/core/services/random.hpp"
#include "pukan/core/services/config.hpp"
#include "pukan/gui/gui.hpp"
#include "pukan/gui/indicator.hpp"
#include "pukan/gui/sprite.hpp"
#include "pukan/gui/text.hpp"
#include "json/jparser.hpp"
#include "json/jobject.hpp"


Game::Game()
   : SceneWithWorld(this),
     SceneWithGUI(this)
{
}

void Game::shot()
{

      //         State::get().game.yobas = 3;

      //      getWorld()->removeAllByName("shell");

/*
   float powerFactor = _launcher->getPowerFactor();
   float angleFactor = _launcher->getAngleFactor();

   Vec2f linearVelocity(50 * powerFactor * (angleFactor >= 0 ? (1 - angleFactor) : 1 - -angleFactor),
                        50 * powerFactor * angleFactor);
   float angularVelocity = -20 * powerFactor;
  */

      Entity * yoba = new Entity(new Body(getWorld(), true),
                                 new Graphics(Resources::get().getTexture("yoba" + b::lexical_cast<std::string>(4 - State::get().game.yobas))),
                                 "yoba");
      yoba->getBody()->createCircleFixture(20, Materials::Wood.getPhysics());
      yoba->getBody()->getFirstFixture()->setFilter(Filters::Yoba);
      yoba->getBody()->setPos(Renderer::get().getGameMousePos());

      float pc = _launcher->getPowerFactor();
      float ac = _launcher->getAngleFactor();
      Body * yobaBody = yoba->getBody();
      yobaBody->getB2Body()->SetLinearVelocity(b2Vec2(50 * pc * (ac >= 0 ? (1 - ac) : 1 - -ac),
                                                      50 * pc * ac));
      yobaBody->getB2Body()->SetAngularVelocity(-20 * pc);
      yobaBody->getB2Body()->SetAngularDamping(2);
      yobaBody->getB2Body()->SetBullet(true);

//      _lastYoba = yoba;

      State::get().game.shot   = false;

      State::get().game.yobas--;
      if(State::get().game.yobas < 1)
         State::get().game.aiming=false;

}

void Game::loadLevel()
{

	
   int level = State::get().game.level;
//   assert(level);
//   int level = 1;

   std::string file = std::string("levels/") + b::lexical_cast<std::string>(level) + ".json";
//std::string file = "editor.json";

   json::Parser parser;
   parser.readFile(file);

   Rectf aabb = Rectf::fromJson(parser.getRoot().get("aabb"));

   getWorld()->loadFromFile(file, Vec2f(500, 500 -aabb.h));
//   getWorld()->loadFromFile(file, ground->getAABB().getPos() -
//                            Vec2f(900, -aabb.height - 1));

      _launcher->setPos(Vec2f(250, 500-200));

   State::get().game.dolls = getWorld()->findByName("doll").size();
}

void Game::onInit()
{


//   sf::Text text(sf::String(L"Колобки:\n"), *Resources::get().getFont("comic"), 30);
//   text.setColor(sf::Color::Black);
//   text.setPosition(shellsRect.getPosition().x + (shellsRect.getSize().x - text.getGlobalBounds().width) / 2,
//                    shellsRect.getPosition().y + 5);
//   Renderer::get().draw(text);

   gui::Rectangle * baseLayout =
         new gui::Rectangle(Vec2f(0, 505),
                            Vec2f(1250, 105),
                            sf::Color(255, 226, 77));
   getGUI()->addElement(baseLayout);

   gui::Rectangle * yobasRect =
         new gui::Rectangle(Vec2f(388, 507),
                            Vec2f(150, 103),
                            sf::Color(255, 255, 148),
                            sf::Color::Black,
                            -2);
   getGUI()->addElement("yobasRect", yobasRect);

   gui::Text * yobasText =
         new gui::Text(yobasRect->getScreenBounds().getCenter() + Vec2f(0, -20),
                       true,
                       L"Колобки",
                       Resources::get().getFont("comic"),
                       sf::Color::Black,
                       30);
   getGUI()->addElement("yobasText", yobasText);

   for(int i = 1; i <= 3; i++)
   {
      std::string name = "yoba" + b::lexical_cast<std::string>(i);
      gui::Sprite * yobaSprite =
            new gui::Sprite(yobasRect->getScreenBounds().getMiddleLeft() + Vec2f(25 + (i - 1) * 50, 20),
                            true,
                            Resources::get().getTexture(name),
                            Vec2f(0.8, 0.8));
      getGUI()->addElement(name, yobaSprite);
   }




   gui::Rectangle * dollsRect =
         new gui::Rectangle(Vec2f(688, 507),
                            Vec2f(150, 103),
                            sf::Color(255, 255, 148),
                            sf::Color::Black,
                            -2);
   getGUI()->addElement("dollsRect", dollsRect);

   gui::Text * dollsText =
         new gui::Text(dollsRect->getScreenBounds().getCenter() + Vec2f(0, -20),
                       true,
                       L"Куклы",
                       Resources::get().getFont("comic"),
                       sf::Color::Black,
                       30);
   getGUI()->addElement("dollsText", dollsText);

//   for(int y = 0; y < 2; y++)
      for(int x = 1; x <= 8; x++)
      {
         std::string name = "doll" + b::lexical_cast<std::string>(x);
         gui::Sprite * dollSprite =
               new gui::Sprite(dollsRect->getScreenBounds().getMiddleLeft() + Vec2f(12 + (x - 1) * 18, 9),
                               true,
                               Resources::get().getTexture("doll1"),
                               Vec2f(0.5, 0.5));
         getGUI()->addElement(name, dollSprite);
      }

      for(int x = 1; x <= 8; x++)
      {
         std::string name = "doll" + b::lexical_cast<std::string>(x + 8);
         gui::Sprite * dollSprite =
               new gui::Sprite(dollsRect->getScreenBounds().getMiddleLeft() + Vec2f(12 + (x - 1) * 18, 35),
                               true,
                               Resources::get().getTexture("doll1"),
                               Vec2f(0.5, 0.5));
         getGUI()->addElement(name, dollSprite);
      }


   gui::Rectangle * powerRect =
         new gui::Rectangle(Vec2f(5, 510),
                            Vec2f(250, 30),
                            sf::Color(255, 255, 148),
                            sf::Color::Black,
                            -2);
   getGUI()->addElement("powerRect", powerRect);

//   _minimap  = new MiniMap(this, getWorld());
   _launcher = new Launcher(this);

   gui::Indicator * powerIndicator =
         new gui::Indicator(powerRect->getScreenBounds().getTopLeft() + Vec2f(2, 2),
                            powerRect->getScreenBounds().getSize() - Vec2f(4, 4),
                            sf::Color(170, 0, 0));
   getGUI()->addElement("powerIndicator", powerIndicator);
   powerIndicator->setFactor(1);

   gui::Text * powerText =
         new gui::Text(powerRect->getScreenBounds().getMiddleRight() + Vec2f(5, -5),
                       false,
                       "0%",
                       Resources::get().getFont("comic"),
                       sf::Color::Black,
                       25);
   getGUI()->addElement("powerText", powerText);


   gui::Rectangle * angleRect =
         new gui::Rectangle(powerRect->getScreenBounds().getBottomLeft() + Vec2f(0, 20),
                            Vec2f(250, 30),
                            sf::Color(255, 255, 148),
                            sf::Color::Black,
                            -2);
   getGUI()->addElement("angleRect", angleRect);

   gui::Indicator * angleIndicator =
         new gui::Indicator(angleRect->getScreenBounds().getMiddleTop() - Vec2f(1, -2),
                            Vec2f(angleRect->getScreenBounds().w / 2 - 1,
                                 angleRect->getScreenBounds().h - 4),
                            sf::Color(170, 0, 0));
   angleIndicator->setFactor(-1);
   getGUI()->addElement("angleIndicator", angleIndicator);

   gui::Rectangle * angleDelimiter =
         new gui::Rectangle(angleRect->getScreenBounds().getMiddleTop() - Vec2f(1, 0),
                            Vec2f(3, angleRect->getScreenBounds().h),
                            sf::Color::Black);
   getGUI()->addElement("angleDelimiter", angleDelimiter);

   gui::Text * angleText =
         new gui::Text(angleRect->getScreenBounds().getMiddleRight() + Vec2f(5, -5),
                       false,
                       "0%",
                       Resources::get().getFont("comic"),
                       sf::Color::Black,
                       25);
   getGUI()->addElement("angleText", angleText);

   YobaObject::onInit();
}

void Game::onReset()
{
   getWorld()->removeAll();

   getGUI()->removeTemporaryElements();

   State::get().game.aiming = true;
   State::get().game.drawMiniMap = false;
   State::get().game.yobas = 3;

   Entity * bottomWall = new Entity(new Body(getWorld(), false),
                                    new Graphics(Resources::get().getTexture("black")));
   bottomWall->getBody()->createRectangleFixture(Vec2f(2000, 5));
   bottomWall->getBody()->getFirstFixture()->setFilter(Filters::Wall);

   bottomWall->getBody()->setPos(0, 500, false);
   Entity * upperWall = bottomWall->clone();
   upperWall->getBody()->setPos(0, 0, false);

   Entity * leftWall = new Entity(new Body(getWorld(), false),
                                  new Graphics(Resources::get().getTexture("black")));
   leftWall->getBody()->createRectangleFixture(Vec2f(5, 500));
   leftWall->getBody()->getFirstFixture()->setFilter(Filters::Wall);

   leftWall->getBody()->setPos(0, 0, false);
   Entity * rightWall = leftWall->clone();
   rightWall->getBody()->setPos(2000 - 5, 0, false);




   //   getWorld()->createGround(Vec2f(0, -500), Vec2f(5, 500));
   //   getWorld()->createGround(Vec2f(2000 - 5, -500), Vec2f(5, 500));

//   getWorld()->createGround(Vec2f(0, 0), Vec2f(2000, 5));
//   getWorld()->createGround(Vec2f(0, -500), Vec2f(2000, 5));




   loadLevel();

   YobaObject::onReset();
}

void Game::onUpdate()
{
   if(State::get().game.reset)
   {
      onReset();

      State::get().game.reset     = false;
      State::get().game.isStarted = true;
   }

   if(State::get().game.pause)
      return;

   if(State::get().game.shot)
      shot();

   // Обновление гуя
   //--------------------------------------------

   // Лаунчер
   bool isActive = State::get().launcher.touching;

   float powerFactor = isActive ? _launcher->getPowerFactor() : 0;
   float angleFactor = isActive ? _launcher->getAngleFactor() : 0;

   getGUI()->getElement<gui::Indicator>("powerIndicator")
         ->setFactor(powerFactor);
   getGUI()->getElement<gui::Text>("powerText")->getText()
         ->setString(b::lexical_cast<std::string>(std::ceil(powerFactor * 100)) + "%");
   getGUI()->getElement<gui::Indicator>("angleIndicator")
         ->setFactor(angleFactor);
   getGUI()->getElement<gui::Text>("angleText")->getText()
         ->setString(b::lexical_cast<std::string>(std::ceil(angleFactor * 100)) + "%");


   // Колобки
   for(int i = 1; i <= 3; i++)
      getGUI()->getElement("yoba" + b::lexical_cast<std::string>(4 - i))
            ->setActive(State::get().game.yobas >= i);

   // Куклы
   for(int i = 1; i <= 16; i++)
      getGUI()->getElement("doll" + b::lexical_cast<std::string>(i))
            ->setActive(State::get().game.dolls >= i);

   //--------------------------------------------

   YobaObject::onUpdate();
}

void Game::onRender()
{
//      Renderer::get().setRenderToTexture(true);

   Renderer::get().updateCamera();

   Renderer::get().fill(sf::Color(255, 226, 77));

   sf::Sprite bg(*Resources::get().getTexture("bg2"));
   bg.setPosition(5, 0);
   Renderer::get().draw(bg); // Без этого не рисует на текстуру


//   sf::RectangleShape background;
//   background.setTexture(Resources::get().getTexture("background"));
//   background.setSize(Vec2f(649, 500));
//   for(int i = 0; i < 4; i++)
//   {
//      background.setPosition(5 + 649 * i, -500);
//      Renderer::get().draw(background);
//   }




//   static sf::RenderTexture renderTexture;
//   if(renderTexture.getSize() == sf::Vector2u())
//   {
//      Logger::get().debug() % "Create render texture";
//      renderTexture.create(1250/1,610/1);
//   }
//   renderTexture.clear(sf::Color::Black);
////   renderTexture.draw(bg);

//   {
//   sf::RectangleShape bg(sf::Vector2f(1250, 610));
//   bg.setPosition(5,-500);
//   bg.setFillColor(sf::Color::Red);
//   renderTexture.draw(bg);
//   }

//  sf::Sprite s(*Resources::get().getTexture("yoba1"));
//  s.setPosition(Renderer::get().getGameMousePos());
//  renderTexture.draw(s);


   for(auto it = getWorld()->getBodyIterator(); it; it++)
   {

      if(it.get()->getEntity()->getName() == "yoba")
      {
         Body * yoba = it.get();

         Vec2f pos = yoba->getPos();
         Vec2f vel = yoba->getLinearVelocity();
         Vec2f echo = pos - Vec2f(vel.x, -vel.y) * 8.f;
         //P(echo);

//         sf::CircleShape cs;
         //         cs.setRadius(3);
         //         cs.setOrigin(3, 3);
         //         cs.setPosition(pukan::yoba(pos - Vec2f(0, yoba->getFirstFixture()->getCircleRadius()), pos, yoba->getAngle()));
         //         cs.setFillColor(sf::Color::Red);
         //         Renderer::get().draw(cs);

         //         sf::CircleShape cs2(cs);
         //         cs2.setFillColor(sf::Color::Blue);
         //         cs2.setPosition(pukan::yoba(pos + Vec2f(0, yoba->getFirstFixture()->getCircleRadius()), pos, yoba->getAngle()));
         //         Renderer::get().draw(cs2);


         //         Renderer::get().drawLine(echo, pos - Vec2f(-vel.x, vel.y) * 5.f, sf::Color::Red);
         //         Renderer::get().drawLine(echo, pos, sf::Color::Red);

         float dirAngle = pos.getAngleTo(pos + Vec2f(vel.x, -vel.y));

         sf::VertexArray tail(sf::Triangles, 3);
         tail[0].position = echo;
         tail[1].position = pos(-6, yoba->getFirstFixture()->getCircleRadius() - 4).getRotated(dirAngle, pos);
         tail[2].position = pos(-6, -yoba->getFirstFixture()->getCircleRadius() + 4).getRotated(dirAngle, pos);

         tail[0].color = sf::Color(255, 226, 76, 00);
         tail[1].color = sf::Color(255, 226, 76);
         tail[2].color = sf::Color(255, 226, 76);

         Renderer::get().draw(tail);
//         renderTexture.draw(tail);
      }


//      renderTexture.draw(*it.get()->getEntity());
      Renderer::get().draw(*it.get()->getEntity());
   }


//   renderTexture.display();
////   sf::Texture tmp(renderTexture.getTexture());
//   sf::Sprite renderSprite(renderTexture.getTexture());
////   P(std::string(Vec2f(renderTexture.getView().getCenter() - renderTexture.getView().getSize() / 2.f)));
//   renderSprite.setPosition(Vec2f(renderTexture.getView().getCenter() - renderTexture.getView().getSize() / 2.f));
//   renderSprite.setScale(1.f,1.f);


//   if(Config::get().blurLevel)
//      Renderer::get().getWindow().draw(renderSprite,Resources::get().getShader("blur"));
//   else
//      Renderer::get().getWindow().draw(renderSprite);

   YobaObject::onRender();
}

void Game::onKeyPressed(const sf::Event::KeyEvent & event)
{
   switch(event.code)
   {
      case sf::Keyboard::Escape:
         State::get().screen = State::MainMenu;
         return;

      case sf::Keyboard::F5:
         State::get().game.reset = true;
         return;

      case sf::Keyboard::R:
      {
         if(!event.shift)
         {
            getWorld()->loadFromFile("levels/1.json", Renderer::get().getGameMousePos());
         }
         else
         {
            json::Parser parser;
            parser.readFile("levels/1.json");
            Rectf aabb = Rectf::fromJson(parser.getRoot().get("aabb"));
            getWorld()->loadFromFile("levels/1.json", Vec2f(Renderer::get().getGameMousePos().x,
                                                         500-aabb.h));
         }
         State::get().game.dolls = getWorld()->findByName("doll").size();
      } break;



      case sf::Keyboard::P:
         State::get().game.pause = !State::get().game.pause;
         break;

//      case sf::Keyboard::Space:
//      case sf::Keyboard::Q:
//         State::get().game.aiming  = false;
//         State::get().game.shot    = true;
//         State::get().aim.yobaType = event.shift ? 1 : event.control ? 2 : 0;
//         break;


//      case sf::Keyboard::F5:
//         sf::Image image = window().capture();
//         image.saveToFile("screenshot.png");
//         break;

      case sf::Keyboard::F:
         Config::get().gameSpeed = 0.10f;
         break;

      case sf::Keyboard::M:
         State::get().game.drawMiniMap = !State::get().game.drawMiniMap;
         break;

      default:
         break;
   }
}

void Game::onKeyReleased(const sf::Event::KeyEvent & event)
{
   switch(event.code)
   {
      case sf::Keyboard::F:
          Config::get().gameSpeed = 1.0f;
         break;

      default:
         break;
   }
}

void Game::onLeftMousePressed(const sf::Event::MouseButtonEvent &)
{
   if(!State::get().game.mouseJoint)
   {
      if(getWorld()->createMouseJoint(Renderer::get().getGameMousePos()))
         State::get().game.mouseJoint = true;
   }

   if(State::get().game.aiming)
   {
      State::get().launcher.touching = true;
   }
}

void Game::onLeftMouseReleased(const sf::Event::MouseButtonEvent &)
{
   if(State::get().game.mouseJoint)
   {
      getWorld()->removeMouseJoint();
      State::get().game.mouseJoint = false;
   }

   if(State::get().game.aiming && State::get().launcher.touching)
   {
      State::get().launcher.touching = false;
      if(_launcher->isValidPos())
         State::get().game.shot    = true;
   }
}

void Game::onRightMousePressed(const sf::Event::MouseButtonEvent &)
{
   Vec2f mpos = Renderer::get().getGameMousePos();
   Vec2f cpos = Renderer::get().getCamera().getCenter();

   Renderer::get().getCamera().setCenter(mpos.x, cpos.y);

   Logger::get().debug() << "Move camera";
}

void Game::onRightMouseReleased(const sf::Event::MouseButtonEvent &)
{

}

void Game::onMiddleMousePressed(const sf::Event::MouseButtonEvent &)
{
   Body * body = getWorld()->findFirstByPos(Renderer::get().getGameMousePos());
   if(body == nullptr)
      return;
   Logger::get().debug("Тело под курсором:\n%1%") << std::string(*body);
}

void Game::onMouseMoved(const sf::Event::MouseMoveEvent & event)
{
   YOBA_UNUSED(event);

   if(State::get().game.mouseJoint)
   {
      getWorld()->getMouseJoint()->SetTarget(Renderer::get().getGameMousePos().toB2Pos());
   }

   /*
   if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
   {
      Vec2f cpos = Renderer::get().getCamera().getCenter();
      Vec2f mpos = Renderer::get().getGameMousePos();

      int max = 10;
      if(mpos.x < cpos.x - max) mpos.x = cpos.x - max;
      if(mpos.x > cpos.x + max) mpos.x = cpos.x + max;



      if(mpos.x < 0 + Renderer::get().getWindow().getSize().x / 2)
         mpos.x = Renderer::get().getWindow().getSize().x / 2;

      if(mpos.x > 2000 - Renderer::get().getWindow().getSize().x / 2)
         mpos.x = 2000 - Renderer::get().getWindow().getSize().x / 2;

      //      if(mpos.x > 2000 - 350) mpos.x = 2000 - 350;

      Renderer::get().getCamera().setCenter(Vec2f(mpos.x, cpos.y));
   }
*/
}

void Game::onMouseWheelMoved(const sf::Event::MouseWheelEvent & event)
{
   Config::get().blurLevel += event.delta * 0.0001;
   if(Config::get().blurLevel < 0)
      Config::get().blurLevel = 0;
   P(Config::get().blurLevel);
   Resources::get().getShader("blur")->setParameter("blur_radius", Config::get().blurLevel);
}

void Game::onBeginContact(b2Contact * b2contact)
{
   Fixture * aFixture = static_cast<Fixture *>(b2contact->GetFixtureA()->GetUserData());
   Fixture * bFixture = static_cast<Fixture *>(b2contact->GetFixtureB()->GetUserData());
   Entity  * aBody = aFixture->getBody()->getEntity();
   Entity  * bBody = bFixture->getBody()->getEntity();
   std::string aName = aBody->getName();
   std::string bName = bBody->getName();

   // Снаряд должен быть A
   if(bName == "yoba" || aName == "doll")
   {
      std::swap(aName, bName);
      std::swap(aBody, bBody);
      std::swap(aFixture, bFixture);
   }

//   if(aName == "yoba" && bName == "part")// && bBody->getBody()->getElaspedTime() < 0.5f)
//   {
//      b2contact->SetEnabled(false);
//      return;
//   }

//   Logger::get().debug("%1%(%3%) -> %2%(%4%)")
//         % aName % bName
//         % aFixture->isSensor() % bFixture->isSensor();

   // Уничтожение куклы через сенсор
   if(bName == "doll" && bFixture->isSensor()
   && !bBody->getBody()->hasTimeLimit())
   {
//      bBody->getGraphics()->getAnimationMgr()->setAnimationId(1);
//      bBody->getBody()->removeAfter(bBody->getGraphics()->getAnimationMgr()->getCurrentAnimation()->getAnimationDuration());
      bBody->getBody()->removeAfter(0);


      getWorld()->addUpdateCallback(
      [this, aBody, bBody]()
      {
         Vec2f center = bBody->getBody()->getPos() + Vec2f(0, 9);
         Vec2f corpsePos = center.getRotated(bBody->getBody()->getAngle(), bBody->getBody()->getPos());
         Vec2f corpseSize(35, 16);

         Entity * corpse = new Entity(new Body(getWorld(), true),
                                      new Graphics(Resources::get().getTexture("doll1d"), corpseSize, false));
         corpse->getBody()->createRectangleFixture(corpseSize, Materials::Wood.getPhysics());
         corpse->getBody()->getFirstFixture()->setFilter(Filters::Block);
         corpse->getBody()->setPos(corpsePos);
         corpse->getBody()->setAngle(bBody->getBody()->getAngle());

//         Vec2f center = bBody->getBody()->getPos();
//         Vec2f partSize(16, 25);
//         float angle = bBody->getBody()->getAngle();

//         Entity * lu = new Entity(new Body(getWorld(), true),
//                                  new Graphics(Resources::get().getTexture("d1d"),
//                                               Recti(0, 0, partSize.x, partSize.y),
//                                               false));
//         lu->getBody()->createRectangleFixture(partSize, Materials::Wood.getPhysics());
//         lu->getBody()->getFirstFixture()->setFilter(Filters::Block);
//         lu->getBody()->setPos(pukan::yoba(center + Vec2f(-partSize.x / 2.f, -partSize.y / 2),
//                                           center,
//                                           angle));
//         lu->getBody()->setAngle(angle);

//         Entity * ru = new Entity(new Body(getWorld(), true),
//                                  new Graphics(Resources::get().getTexture("ru"), partSize, false));
//         ru->getBody()->createRectangleFixture(partSize, Materials::Wood.getPhysics());
//         ru->getBody()->getFirstFixture()->setFilter(Filters::Block);
//         ru->getBody()->setPos(pukan::yoba(center + Vec2f(partSize.x / 2.f, -partSize.y / 2.f),
//                                           center,
//                                           angle));
//         ru->getBody()->setAngle(angle);

//         Entity * ld = new Entity(new Body(getWorld(), true),
//                                  new Graphics(Resources::get().getTexture("ld"), partSize, false));
//         ld->getBody()->createRectangleFixture(partSize, Materials::Wood.getPhysics());
//         ld->getBody()->getFirstFixture()->setFilter(Filters::Block);
//         ld->getBody()->setPos(pukan::yoba(center + Vec2f(-partSize.x / 2.f, partSize.y / 2.f),
//                                           center,
//                                           angle));
//         ld->getBody()->setAngle(angle);

//         Entity * rd = new Entity(new Body(getWorld(), true),
//                                  new Graphics(Resources::get().getTexture("rd"), partSize, false));
//         rd->getBody()->createRectangleFixture(partSize, Materials::Wood.getPhysics());
//         rd->getBody()->getFirstFixture()->setFilter(Filters::Block);
//         rd->getBody()->setPos(pukan::yoba(center + Vec2f(partSize.x / 2.f, partSize.y / 2.f),
//                                           center,
//                                           angle));
//         rd->getBody()->setAngle(angle);

//         lu->getBody()->getB2Body()->SetLinearVelocity(aBody->getBody()->getB2Body()->GetLinearVelocity());
//         ld->getBody()->getB2Body()->SetLinearVelocity(aBody->getBody()->getB2Body()->GetLinearVelocity());
//         ru->getBody()->getB2Body()->SetLinearVelocity(aBody->getBody()->getB2Body()->GetLinearVelocity());
//         rd->getBody()->getB2Body()->SetLinearVelocity(aBody->getBody()->getB2Body()->GetLinearVelocity());

      });


      State::get().game.dolls--;
      assert(State::get().game.dolls >= 0);

      // Попап
      gui::Text * popup =
            new gui::Text(Renderer::get().gameToScreen(bBody->getBody()->getAABB().getCenter()),
                          true,
                          "+50",
                          Resources::get().getFont("comic"),
                          sf::Color::Green,
                          30);
      popup->getText()->setStyle(sf::Text::Bold);
      popup->removeAfter(0.75f);
      getGUI()->addElement(popup);
   }

   /*
   if(false&&(aName == "yoba" || aName == "block") && bName == "part")
   {
      b2Vec2 yobaVelocity = aBody->getBody()->getB2Body()->GetLinearVelocity();

      // Разрушено?
      if(std::abs(yobaVelocity.x) > 5 || std::abs(yobaVelocity.y) > 5)
      {
         bBody->getBody()->removeAfter(0);
         aBody->getBody()->getB2Body()->SetLinearVelocity(b2Vec2(yobaVelocity.x * 0.99,
                                                                 yobaVelocity.y * 0.99));
         b2contact->SetEnabled(false);

         // Попапы
         gui::Sprite * smokePopup =
               new gui::Sprite(Renderer::get().gameToScreen(bBody->getBody()->getPos()),
                               true,
                               Resources::get().getTexture("smoke"),
                               Vec2f(0.33, 0.33));
         smokePopup->removeAfter(0.4);
         getGUI()->addElement(smokePopup);
      }
   }
*/
   if((aName == "yoba" || aName == "block") && bName == "block"
   && bBody->getBody()->getFirstFixture()->getPhysics().density <= 1)
   {
      Body * yoba = aBody->getBody();
      b2Vec2 yobaVelocity = yoba->getB2Body()->GetLinearVelocity();

      // Разрушено?
//      if(bBody->getBody()->getFirstFixture()->getRectangleSize().x > 3
//      && bBody->getBody()->getFirstFixture()->getRectangleSize().y > 3)
      if(std::abs(yobaVelocity.x) > 5 || std::abs(yobaVelocity.y) > 5)
      {
         aBody->getBody()->getB2Body()->SetLinearVelocity(b2Vec2(yobaVelocity.x * 0.75,
                                                                 yobaVelocity.y * 0.75));
         bBody->getBody()->removeAfter(0);
         b2contact->SetEnabled(false);


         // Осколки
               if(bBody->getBody()->getFirstFixture()->getRectangleSize().x >= 10
               && bBody->getBody()->getFirstFixture()->getRectangleSize().y >= 10)
         if(Config::get().destructible)
         {
//            for(int i = 0; i < 5; i++)
//            {
               getWorld()->addUpdateCallback(
               [this, bBody, bFixture]()
               {
                  Vec2f pos = bBody->getBody()->getPos();
                  float angle = bBody->getBody()->getAngle();
                  const Vec2f blockSize = bFixture->getRectangleSize();
//                  Vec2f partSize = blockSize / 2.f;

                  Vec2f topLeft = pos - blockSize / 2.f;

                  Vec2f partSize = Vec2f(10, 10);
                  int horisontalParts = std::floor(blockSize.x / partSize.x);
                  int verticalParts =   std::floor(blockSize.y / partSize.y);

                  for(int y = 0; y < verticalParts; y++)
                     for(int x = 0; x < horisontalParts; x++)
                     {
                        Vec2f center = topLeft(partSize.getHalf())(partSize.x * x, partSize.y * y).getRotated(angle, pos);

                        Entity * part = new Entity(new Body(getWorld(), true),
                                                   new Graphics(Resources::get().getTexture("wood"),//bBody->getGraphics()->getTexture(),
                                                                partSize,
                                                                true,
                                                                true),
                                                   "part");
                        part->getBody()->createRectangleFixture(partSize, bFixture->getPhysics());
                        part->getBody()->getFirstFixture()->setFilter(bFixture->getFilter());
                        part->getBody()->setPos(center);
                        part->getBody()->setAngle(angle);
                     }
               });

               if(aBody->getName() == "block")
               {
                  aBody->getBody()->removeAfter(0);
               getWorld()->addUpdateCallback(
               [this, aBody, aFixture]()
               {
                  Vec2f pos = aBody->getBody()->getPos();
                  float angle = aBody->getBody()->getAngle();
                  const Vec2f blockSize = aFixture->getRectangleSize();
//                  Vec2f partSize = blockSize / 2.f;

                  Vec2f topLeft = pos - blockSize / 2.f;

                  Vec2f partSize = Vec2f(10, 10);
                  int horisontalParts = std::floor(blockSize.x / partSize.x);
                  int verticalParts =   std::floor(blockSize.y / partSize.y);

                  for(int y = 0; y < verticalParts; y++)
                     for(int x = 0; x < horisontalParts; x++)
                     {
                        Vec2f center = topLeft(partSize.getHalf())(partSize.x * x, partSize.y * y).getRotated(angle, pos);

//                        Vec2f center = pukan::yoba((topLeft + partSize / 2.f) + Vec2f(partSize.x * x, partSize.y * y),
//                                                  pos,
//                                                  angle);

                        Entity * part = new Entity(new Body(getWorld(), true),
                                                   new Graphics(Resources::get().getTexture("wood"),//bBody->getGraphics()->getTexture(),
                                                                partSize,
                                                                true,
                                                                true),
                                                   "part");
                        part->getBody()->createRectangleFixture(partSize, aFixture->getPhysics());
                        part->getBody()->getFirstFixture()->setFilter(aFixture->getFilter());
                        part->getBody()->setPos(center);
                        part->getBody()->setAngle(angle);
                     }
               });
               }

         }

               b2WorldManifold manifold;
               b2contact->GetWorldManifold(&manifold);
               Vec2f contact = Vec2f::fromB2Pos(manifold.points[0]);

         // Попапы
               {
                  gui::Sprite * smokePopup =
                        new gui::Sprite(Renderer::get().gameToScreen(contact),
                                        true,
                                        Resources::get().getTexture("smoke"));
                  smokePopup->removeAfter(0.15);
                  getGUI()->addElement(smokePopup);

                  gui::Text * scorePopup =
                        new gui::Text(Renderer::get().gameToScreen(bBody->getBody()->getPos()),
                                      true,
                                      "+1",
                                      Resources::get().getFont("comic"),
                                      sf::Color::Green,
                                      25);
                  scorePopup->getText()->setStyle(sf::Text::Bold);
                  scorePopup->removeAfter(0.75f);
                  getGUI()->addElement(scorePopup);
               }
      }
   }
}

void Game::onPreSolveContact(b2Contact * b2contact)
{
   Fixture * aFixture = static_cast<Fixture *>(b2contact->GetFixtureA()->GetUserData());
   Fixture * bFixture = static_cast<Fixture *>(b2contact->GetFixtureB()->GetUserData());
   Entity  * aBody = aFixture->getBody()->getEntity();
   Entity  * bBody = bFixture->getBody()->getEntity();
   std::string aName = aBody->getName();
   std::string bName = bBody->getName();

   // Снаряд должен быть A
   if(bName == "yoba" || aName == "doll")
   {
      std::swap(aName, bName);
      std::swap(aBody, bBody);
      std::swap(aFixture, bFixture);
   }



//   if(aName == "yoba" && bName == "part" && bBody->getBody()->getElaspedTime() < 0.1f)
//   {
//      P("disabled");
//      b2contact->SetEnabled(false);
//      return;
//   }

//   if(aName == "yoba" && bName == "block")
//      b2contact->SetEnabled(false);
}
