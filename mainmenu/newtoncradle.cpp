#include "angry_yoba/mainmenu/newtoncradle.hpp"
#include "angry_yoba/mainmenu/newtoncradle.hpp"
#include "angry_yoba/state.hpp"
#include "angry_yoba/gcommon.hpp"
#include "pukan/core/entity.hpp"
#include "pukan/core/physics/world.hpp"
#include "pukan/core/physics/body.hpp"
#include "pukan/core/physics/fixture.hpp"
#include "pukan/core/physics/bodyiterator.hpp"
#include "pukan/core/graphics/graphics.hpp"
#include "pukan/core/services/resources.hpp"
#include "pukan/core/services/renderer.hpp"
#include "pukan/core/services/config.hpp"

NewtonCradle::NewtonCradle(YobaObject * parent)
   : YobaObject(parent),
     SceneWithWorld(this)
{
}

void NewtonCradle::onInit()
{
   {
//      Entity * e = new Entity(new Body(getWorld(), false),
//                              new Graphics(Resources::get().getTexture("black")));
//      e->getBody()->createRectangleFixture(Vec2f(250, 7));
//      e->getBody()->setPos(0,0);
   }

   Vec2f pos(Renderer::get().getWindow().getSize().x / 2, 200);

   Entity * base = new Entity(new Body(getWorld(), false),
                              new Graphics(Resources::get().getTexture("black")));
   base->getBody()->createRectangleFixture(Vec2f(250, 7));
   base->getBody()->setPos(pos, true);

   const std::vector<std::string> kolobki {
      "yoba1", "yoba2", "yoba3", "yoba4", "yoba5"
   };

   float radius = 25;
   for(int i = 0; i < 5; i++)
   {
      Entity * ball = new Entity(new Body(getWorld(), (i >= 1 && i <= 3 ? false : true)),
                                 new Graphics(Resources::get().getTexture(kolobki[i])));
      ball->getBody()->createCircleFixture(25, Materials::Steel.getPhysics());
      ball->getBody()->setPos(Vec2f(base->getBody()->getAABB().x + radius  + (i * radius * 2),
                                    pos.y + 210));
      ball->getBody()->getFirstFixture()->setFilter(Filters::Block);
      ball->getBody()->getB2Body()->SetFixedRotation(true);

      b2World * world = getWorld()->getB2World();
      b2DistanceJointDef jd;

      jd.frequencyHz  = 100.0f;
      jd.dampingRatio = 0.0f;

      jd.bodyA = base->getBody()->getB2Body();
      jd.bodyB = ball->getBody()->getB2Body();
      jd.localAnchorA.Set((base->getBody()->getAABB().x + radius  + (i * radius * 2) - pos.x) / PixelsPerMeter, 0);
      jd.localAnchorB.Set(0, 0);

      jd.length = 7;
      world->CreateJoint(&jd);
   }

   getWorld()->findById(1)->getB2Body()->SetLinearVelocity(b2Vec2(-4, 0));
   State::get().newtonCradle.left    = true;
   State::get().newtonCradle.started = true;

   YobaObject::onInit();
}

void NewtonCradle::onUpdate()
{
   Config::get().gameSpeed = 3.f;
   YobaObject::onUpdate();
   Config::get().gameSpeed = 1.f;
}

void NewtonCradle::onRender()
{
   Renderer::get().resetCamera();

   for(const b2Joint * j = getWorld()->getB2World()->GetJointList(); j; j = j->GetNext())
      Renderer::get().drawLine(Vec2f::fromB2Pos(j->GetAnchorA()),
                               Vec2f::fromB2Pos(j->GetAnchorB()),
                               sf::Color(150, 150, 150));

   for(auto it = getWorld()->getBodyIterator(); it; ++it)
   {

      Renderer::get().draw(*it.get()->getEntity());
   }
//   YobaObject::onRender();
   YobaObject::onUpdate();
}

void NewtonCradle::onBeginContact(b2Contact * contact)
{
   Body * a = static_cast<Body *>(contact->GetFixtureA()->GetBody()->GetUserData());
   Body * b = static_cast<Body *>(contact->GetFixtureB()->GetBody()->GetUserData());

   bool hit = false;

   if(State::get().newtonCradle.left && ((a->getId() == 1 && b->getId() == 2) || (a->getId() == 2 && b->getId() == 1)))
   {
      getWorld()->findById(1)->getB2Body()->SetLinearVelocity(b2Vec2(0, 0));
      getWorld()->findById(5)->getB2Body()->SetLinearVelocity(b2Vec2(4, 0));

      State::get().newtonCradle.left = false;
      hit = true;
   }

   if(!State::get().newtonCradle.left && ((a->getId() == 5 && b->getId() == 4) || (a->getId() == 4 && b->getId() == 5)))
   {
      getWorld()->findById(5)->getB2Body()->SetLinearVelocity(b2Vec2(0, 0));
      getWorld()->findById(1)->getB2Body()->SetLinearVelocity(b2Vec2(-4, 0));

      State::get().newtonCradle.left = true;
      hit = true;
   }

   if(hit)
   {
      sf::Sound * s = Resources::get().getSound("tick");

      s->setVolume(5);
      s->setPitch(3);
      s->play();
   }
}
