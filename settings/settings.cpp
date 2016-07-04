#include "angry_yoba/settings/settings.hpp"
#include "angry_yoba/state.hpp"
#include "pukan/core/services/resources.hpp"
#include "pukan/core/services/renderer.hpp"
#include "pukan/core/services/config.hpp"
#include "pukan/gui/gui.hpp"
#include "pukan/gui/rectangle.hpp"
#include "pukan/gui/checkbox.hpp"
#include "pukan/gui/text.hpp"
#include "pukan/gui/indicator.hpp"
#include "pukan/gui/simplebutton.hpp"

Settings::Settings()
   : SceneWithGUI(this)
{
}

void Settings::onInit()
{
   Vec2f screenSize(Renderer::get().getWindow().getSize());
   getGUI()->addElement(new gui::Rectangle(Vec2f(0, 0), Vec2f(screenSize.x, 5), sf::Color::Black));
   getGUI()->addElement(new gui::Rectangle(Vec2f(0, screenSize.y - 5), Vec2f(screenSize.x, 5), sf::Color::Black));
   getGUI()->addElement(new gui::Rectangle(Vec2f(0, 0), Vec2f(5, screenSize.y), sf::Color::Black));
   getGUI()->addElement(new gui::Rectangle(Vec2f(screenSize.x - 5, 0), Vec2f(5, screenSize.y), sf::Color::Black));

   {
      gui::CheckBox * box = new gui::CheckBox(Vec2f(100, 100), Vec2f(20, 20),
                                              sf::Color::Transparent, sf::Color::Black, -2);
      box->setMouseOverCallback([box]() { box->getShape()->setFillColor(sf::Color(255, 226, 76)); });
      box->setMouseOutCallback([box]()  { box->getShape()->setFillColor(sf::Color::Transparent); });
      box->setMouseClickCallback([box](){ box->setChecked(!box->isChecked()); });

      gui::Text * text = new gui::Text(
                            box->getScreenBounds().getCenter() - Vec2i(0, 20),
                            true, L"Рамки",
                            Resources::get().getFont("comic"),
                            sf::Color::Black,
                            20);

      text->setScreenPos(box->getScreenBounds().getCenter() -
                         Vec2i(text->getScreenBounds().w / 1.4, -5));

      getGUI()->addElement(box);
      getGUI()->addElement(text);
   }


   {
      gui::CheckBox * box = new gui::CheckBox(Vec2f(200, 200), Vec2f(20, 20),
                                              sf::Color::Transparent, sf::Color::Black, -2);
      box->setMouseOverCallback([box]() { box->getShape()->setFillColor(sf::Color(255, 226, 76)); });
      box->setMouseOutCallback([box]()  { box->getShape()->setFillColor(sf::Color::Transparent); });
      box->setMouseClickCallback([box](){ box->setChecked(!box->isChecked()); });

      gui::Text * text = new gui::Text(
                            Vec2f(),
                            true, sf::String(L"Pазpyшаемость"),
                            Resources::get().getFont("comic"),
                            sf::Color::Black,
                            20);

      text->setScreenPos(box->getScreenBounds().getCenter() -
                         Vec2f(text->getScreenBounds().w / 1.6, -5));

      getGUI()->addElement(box);
      getGUI()->addElement(text);
   }

   {
      gui::CheckBox * box = new gui::CheckBox(Vec2f(100, 300), Vec2f(20, 20),
                                              sf::Color::Transparent, sf::Color::Black, -2);
      box->setMouseOverCallback([box]() { box->getShape()->setFillColor(sf::Color(255, 226, 76)); });
      box->setMouseOutCallback([box]()  { box->getShape()->setFillColor(sf::Color::Transparent); });
      box->setMouseClickCallback([box](){ box->setChecked(!box->isChecked()); });

      gui::Text * text = new gui::Text(
                            Vec2f(),
                            true, L"AABB",
                            Resources::get().getFont("comic"),
                            sf::Color::Black,
                            20);
      text->setScreenPos(box->getScreenBounds().getCenter() -
                         Vec2i(text->getScreenBounds().w, -5));

      getGUI()->addElement(text);
      getGUI()->addElement(box);
   }


   {
      gui::Rectangle * ib = new gui::Rectangle(Vec2f(200, 100),
                                               Vec2f(150, 15),
                                               sf::Color::Transparent,
                                               sf::Color::Black,
                                               1);
      getGUI()->addElement(ib);


      gui::Indicator * i = new gui::Indicator(Vec2f(200, 100),
                                              Vec2f(150, 15),
                                              sf::Color::Red);
      i->setFactor(1.f);




      i->setMouseClickCallback([i]()
      {
         Recti bounds(i->getScreenBounds());

         float a = Renderer::get().getGameMousePos().x - bounds.getPos().x;
         float b = bounds.getTopRight().x - bounds.getTopLeft().x;
         float factor = a / b;

         i->setFactor(factor);
      });

      getGUI()->addElement(i);
   }

   {
      gui::SimpleButton * button =
            new gui::SimpleButton(Vec2f(500, 500),
                                  Vec2f(120, 50),
                                  sf::Color::Transparent,
                                  sf::Color::Black,
                                  -2,
                                  L"Сохранить",
                                  Resources::get().getFont("comic"),
                                  sf::Color::Black,
                                  22);
      getGUI()->addElement(button);


      button->setMouseClickCallback([button]()
      {
//         Config::get().drawAABB = getGUI()->getElement("")
      });
   }
}

void Settings::onReset()
{
}

void Settings::onRender()
{

   Renderer::get().fill(sf::Color(255, 255, 148));

   YobaObject::onRender();
}

void Settings::onKeyPressed(const sf::Event::KeyEvent & event)
{
   switch(event.code)
   {
      case sf::Keyboard::Escape:
         State::get().screen = State::MainMenu;
         break;

      default: break;
   }
}
