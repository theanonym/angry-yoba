#ifndef YOBA_HPP
#define YOBA_HPP

#include "pukan/common.hpp"

//using namespace pukan;

class Yoba
{
public:
   enum Type
   {
      Simple,
      Splitable
   };

   static const std::map<Type, std::string> TypeMap;

public:
   virtual Type getType() const = 0;

};

#endif // YOBA_HPP
