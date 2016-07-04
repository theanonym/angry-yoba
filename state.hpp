#ifndef STATE_HPP
#define STATE_HPP

#include "pukan/common.hpp"
#include "pukan/singleton.hpp"

using namespace pukan;

struct State : public Singleton<State>
{
   enum Screen
   {
      Game,
      MainMenu,
      Editor,
      LevelSelect,
      Settings
   } screen;

   struct Game
   {
      bool reset  = false;
      bool pause = false;
      bool isStarted = false;
      bool aiming = false;
      bool shot   = false;
      bool dragCamera = false;
      bool mouseJoint = false;
      bool drawMiniMap = false;


      int level = 0;

      int yobas = 3;
      int dolls = 0;
   } game;

   struct Editor
   {
      bool reset = false;
      bool simulation = false;
   } editor;

   struct LevelSelect
   {
      int page = 0;
   } levelSelect;

   struct Launcher
   {
      bool touching = false;
   } launcher;


   struct Debug
   {
      bool drawInfo = false;
   } debug;

   struct Camera
   {
      bool drag = false;
   } getCamera;

   struct NewtonCradle
   {
      bool started = false;
      bool left    = false;
   } newtonCradle;
};

#endif // STATE_HPP
