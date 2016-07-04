#include "angry_yoba/mainmenu/mainmenu.hpp"
#include "angry_yoba/mainmenu/newtoncradle.hpp"
#include "angry_yoba/state.hpp"
#include "pukan/core/services/resources.hpp"
#include "pukan/core/services/renderer.hpp"
#include "pukan/gui/gui.hpp"
#include "pukan/gui/simplebutton.hpp"

MainMenu::MainMenu()
   : SceneWithGUI(this)
{
   _nc = new NewtonCradle(this);
}

void MainMenu::onInit()
{
   Vec2f startPos(1000, 300);
   Vec2f size(200, 50);

   const wchar_t * buttons[5] = {
      L"Продолжить",
      L"Новая игра",
      L"Редактор",
      L"Настройки",
      L"Выход"
   };

   std::function<void ()> callbacks[5] = {
      // Продолжить
      []() {
         if(State::get().game.isStarted == false)
            return;
         State::get().screen = State::Game;
      },
      // Новая игра
      []() {
         State::get().screen = State::LevelSelect;
//         State::get().game.reset = true;
      },
      // Редактор
      []() {
         State::get().screen = State::Editor;
         State::get().editor.reset = true;
      },
      // Настройки
      []() {
         State::get().screen = State::Settings;
      },
      // Выход
      []() {
         Renderer::get().getWindow().close();
      }
   };

   for(int i = 0; i < 5; i++)
   {
      gui::SimpleButton * button =
            new gui::SimpleButton(startPos + Vec2f(0, 50 * i), size,
                                  sf::Color::Transparent, sf::Color::Black, 0,
                                  buttons[i], Resources::get().getFont("comic"),
                                  sf::Color::Black, 30);
      button->setMouseOverCallback([button]()
      { button->getText()->getText()->setColor(sf::Color(130, 130, 130)); });

      button->setMouseOutCallback([button]()
      { button->getText()->getText()->setColor(sf::Color::Black); });

      button->setMouseClickCallback(callbacks[i]);

      getGUI()->addElement(button);
   }


   YobaObject::onInit();
}

void MainMenu::onUpdate()
{
   YobaObject::onUpdate();
}

void MainMenu::onRender()
{
   Renderer::get().resetCamera();


//   Renderer::get().fill(sf::Color(255, 226, 76));
   Renderer::get().fill(sf::Color(255, 225, 105));


   YobaObject::onRender();
}

void MainMenu::onKeyPressed(const sf::Event::KeyEvent & key)
{
   if(key.code == sf::Keyboard::Escape)
      Renderer::get().getWindow().close();
}
