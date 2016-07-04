#ifndef GCOMMON_HPP
#define GCOMMON_HPP

#include "pukan/common.hpp"
#include "pukan/core/physics/material.hpp"
#include "pukan/core/physics/filter.hpp"

using namespace pukan;

struct Materials
{
   static const Material Wood;
   static const Material Steel;
   static const Material Stone;
   static const Material Textile;
};

struct Filters
{
   static const Filter Wall;
   static const Filter Block;
   static const Filter Yoba;
   static const Filter Enemy;
   static const Filter EnemySensor;
};

#endif // GCOMMON_HPP
