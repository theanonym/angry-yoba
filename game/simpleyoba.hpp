#ifndef SIMPLEYOBA_HPP
#define SIMPLEYOBA_HPP

#include "angry_yoba/game/yoba.hpp"

class SimpleYoba : public Yoba
{
public:
   SimpleYoba();

   virtual Yoba::Type getType() const;
};

#endif // SIMPLEYOBA_HPP
