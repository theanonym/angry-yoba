#ifndef FPSCOUNTER_HPP
#define FPSCOUNTER_HPP

#include "pukan/common.hpp"

class FpsCounter
{
public:
   void update();
   int getFps() const;

private:
   int _frameCount = 0;
   int _fps = 0;
   sf::Clock _clock;
};

inline void FpsCounter::update()
{
   _frameCount++;

   if(_clock.getElapsedTime().asSeconds() >= 1.0f)
   {
      _fps = _frameCount;
      _frameCount = 0;
      _clock.restart();
   }
}

inline int FpsCounter::getFps() const
{
   return _fps;
}

#endif // FPSCOUNTER_HPP
