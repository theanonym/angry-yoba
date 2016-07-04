#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "pukan/core/baseapplication.hpp"

using namespace pukan;

class Application : public BaseApplication
{
public:
   Application(int argc, char ** argv);

private:
   void init();
   void clean();

   void input(const sf::Event & event);
   void update();
   void render();

private:
   class MainMenu *    _mainMenu;
   class Game *        _game;
   class LevelEditor * _levelEditor;
   class LevelSelect * _levelSelect;
   class Settings *    _settings;

   class Cursor *     _cursor;
   class ScreenInfo * _info;
   class FpsCounter * _fps;
};

#endif // APPLICATION_HPP
