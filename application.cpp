#include "angry_yoba/application.hpp"
#include "angry_yoba/game/game.hpp"
#include "angry_yoba/mainmenu/mainmenu.hpp"
#include "angry_yoba/leveleditor/leveleditor.hpp"
#include "angry_yoba/levelselect/levelselect.hpp"
#include "angry_yoba/settings/settings.hpp"
#include "angry_yoba/state.hpp"
#include "angry_yoba/screeninfo.hpp"
#include "angry_yoba/fpscounter.hpp"
#include "angry_yoba/cursor.hpp"
#include "pukan/core/services/resources.hpp"
#include "pukan/core/services/logger.hpp"
#include "pukan/core/services/config.hpp"
#include "pukan/core/services/renderer.hpp"

Application::Application(int argc, char ** argv)
   : BaseApplication(argc, argv)
{
}

void Application::init()
{
   Logger::get().debug() << "Инициализация";
   sf::Clock initClock;

   // Ресурсы
   Resources::get().loadFromJson(Config::get().workingDir / "resources.json");

   Resources::get().loadTexture("doll1d", Config::get().workingDir / "res/png/dolls/doll1d.png");

   Resources::get().loadTexture("lu", Config::get().workingDir / "res/png/dolls/lu.png");
   Resources::get().loadTexture("ld", Config::get().workingDir / "res/png/dolls/ld.png");
   Resources::get().loadTexture("ru", Config::get().workingDir / "res/png/dolls/ru.png");
   Resources::get().loadTexture("rd", Config::get().workingDir / "res/png/dolls/rd.png");

   Resources::get().loadTexture("d1d", Config::get().workingDir / "res/png/dolls/doll1dead.png");

   Resources::get().loadTexture("bg2", Config::get().workingDir / "res/png/background3.png");

   sf::Shader * blur = Resources::get().loadShader("blur",
                                                   Config::get().workingDir / "res/shaders/blur.frag",
                                                   sf::Shader::Fragment);
   blur->setParameter("texture", sf::Shader::CurrentTexture);
//   blur->setParameter("blur_radius", 0.000);


   // Подгрузка шрифтов нужного размера
   // Иначе будут фризы во время игры
   Resources::get().preloadFont("terminus", 18);
   Resources::get().preloadFont("comic", 25);
   Resources::get().preloadFont("comic", 30);
   Resources::get().preloadFont("comic", 40);

   sf::Text tmp("+50", *Resources::get().getFont("comic"),  30);
   tmp.setStyle(sf::Text::Bold);
   Renderer::get().draw(tmp);

   // Настройки
   Config::get().windowTitle  = L"Злые Колобки";
   Config::get().windowWidht  = 1250;
   Config::get().windowHeight = 610;
   Config::get().fpsLimit = 60;
   Config::get().destructible = true;
   Config::get().drawOutline  = true;
   Config::get().pixelFix     = true;
   Config::get().customCursor = true;
//   Config::get().drawAABB = true;
//   Config::get().drawSensors = true;
   Config::get().physicsAccuracy = 1.f;

   //Config::get().antialiasingLevel = 8;

   // Окно
   Renderer::get().resetWindow();
   Renderer::get().setWindowIcon(Resources::get().getTexture("icon")->copyToImage());


   // Информация на экране
   _cursor = new Cursor(Resources::get().getTexture("cursor"));
   _info = new ScreenInfo(sf::Vector2f(5, 5),
                              Resources::get().getFont("terminus"),
                              sf::Color::Red,
                              18);
   _fps = new FpsCounter;

   // Сцены
   _mainMenu = new MainMenu;
   _mainMenu->onInit();
   _game = new Game;
   _game->onInit();
   _levelEditor = new LevelEditor;
   _levelEditor->onInit();
   _levelSelect = new LevelSelect;
   _levelSelect->onInit();
   _settings = new Settings;
   _settings->onInit();


   State::get().screen = State::MainMenu;

   Logger::get().debug("Инициализация завершена за %.3d секунд")
         << initClock.getElapsedTime().asSeconds();
}

void Application::clean()
{
   _mainMenu->onClean();
   _game->onClean();
   _levelEditor->onClean();
   _levelSelect->onClean();

   delete _mainMenu;
   delete _game;
   delete _levelEditor;
   delete _levelSelect;
   delete _settings;

   delete _cursor;
   delete _info;
   delete _fps;

   Logger::get().info() << "Приложение завершено успешно";
}

void Application::input(const sf::Event & event)
{
   if(event.type == sf::Event::Closed)
      Renderer::get().close();

   if(event.type == sf::Event::KeyPressed &&
      event.key.code == sf::Keyboard::F1)
   {
      State::get().debug.drawInfo = !State::get().debug.drawInfo;
      return;
   }

   if(event.type == sf::Event::KeyPressed &&
      event.key.code == sf::Keyboard::F2)
   {
      Config::get().drawAABB = !Config::get().drawAABB;

      return;
   }

   if(event.type == sf::Event::KeyPressed &&
      event.key.code == sf::Keyboard::F3)
   {
      Config::get().drawOutline = !Config::get().drawOutline;
      return;
   }

   if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle)
      Logger::get().debug("Клик:\nИгра: %1%\nЭкран: %2%")
            << std::string(Renderer::get().getGameMousePos())
            << std::string(Renderer::get().getScreenMousePos());

   switch(State::get().screen)
   {
      case State::MainMenu:    _mainMenu->onEvent(event); break;
      case State::Game:        _game->onEvent(event); break;
      case State::Editor:      _levelEditor->onEvent(event); break;
      case State::LevelSelect: _levelSelect->onEvent(event);  break;
      case State::Settings:    _settings->onEvent(event); break;
   }
}

void Application::update()
{
   switch(State::get().screen)
   {
      case State::MainMenu:    _mainMenu->onUpdate(); break;
      case State::Game:        _game->onUpdate(); break;
      case State::Editor:      _levelEditor->onUpdate(); break;
      case State::LevelSelect: _levelSelect->onUpdate();  break;
      case State::Settings:    _settings->onUpdate(); break;
   }
}

void Application::render()
{
//Renderer::get().setRenderToTexture(true);
//   Renderer::get().fill();

   switch(State::get().screen)
   {
      case State::MainMenu:    _mainMenu->onRender(); break;
      case State::Game:        _game->onRender(); break;
      case State::Editor:      _levelEditor->onRender(); break;
      case State::LevelSelect: _levelSelect->onRender();  break;
      case State::Settings:    _settings->onRender(); break;
   }

   if(State::get().debug.drawInfo)
   {
      _fps->update();
      _info->setString((b::format("FPS: %1%\n"
                                  "Antialiasing: x%2%\n"
                                  "Blur: x%3%\n"
                                  "Physics accuracy: x%4%\n"
                                  "Draw AABB: %5%\n"
                                  "Draw outline: %6%\n")
                        % _fps->getFps()
                        % Config::get().antialiasingLevel
                        % (Config::get().blurLevel * 1000)
                        % Config::get().physicsAccuracy
                        % boost::io::group(std::boolalpha, Config::get().drawAABB)
                        % boost::io::group(std::boolalpha, Config::get().drawOutline)).str());
      Renderer::get().draw(*_info);
   }

   if(Config::get().customCursor)
   {
      _cursor->setPos(Renderer::get().getGameMousePos());
      Renderer::get().draw(*_cursor);
   }

   Renderer::get().render();
}


