#include "angry_yoba/levelselect/levelselect.hpp"
#include "angry_yoba/state.hpp"
#include "pukan/core/services/resources.hpp"
#include "pukan/core/services/config.hpp"
#include "pukan/core/services/logger.hpp"
#include "pukan/core/services/renderer.hpp"
#include "pukan/gui/gui.hpp"
#include "pukan/gui/simplebutton.hpp"

LevelSelect::LevelSelect()
   : SceneWithGUI(this)
{
}

void LevelSelect::onInit()
{   
   std::vector<bfs::path> aviableLevels;

   bfs::path levels(Config::get().workingDir / "levels");
   bfs::directory_iterator end;
   for(bfs::directory_iterator it(levels); it != end; it++)
   {
      bfs::path path = it->path();
      if(bfs::is_regular_file(path) && path.extension() == ".json")
      {
         aviableLevels.push_back(path);
      }
   }

   unsigned count = 0;
   for(int y = 0; y < 5; y++)
   {
      for(int x = 0; x < 10; x++)
      {
         if(count == aviableLevels.size())
            return;
         count++;

         Recti rect(50 + (x * 100) + (x * 15),
                    25 + (y * 100) + (y * 15),
                    100, 100);

         gui::SimpleButton * button =
               new gui::SimpleButton(rect.getPos(),
                                     rect.getSize(),
                                     sf::Color(255, 255, 148),
                                     sf::Color::Black,
                                     -2,
                                     b::lexical_cast<std::string>(count),
                                     Resources::get().getFont("comic"),
                                     sf::Color::Black,
                                     50);

         button->setMouseClickCallback([count] ()
         {  State::get().screen = State::Game;
            State::get().game.level = count;
            State::get().game.reset = true;
         });

         getGUI()->addElement(button);
      }
   }

   YobaObject::onInit();
}

void LevelSelect::onRender()
{
   Renderer::get().fill(sf::Color(255, 226, 77));

   YobaObject::onRender();
}

void LevelSelect::onKeyPressed(const sf::Event::KeyEvent & event)
{
   switch(event.code)
   {
      case sf::Keyboard::Escape:
         State::get().screen = State::MainMenu;
         break;

      default: break;
   }
}
