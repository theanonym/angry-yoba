#ifndef MATERIALS_HPP
#define MATERIALS_HPP

#include "pukan/singleton.hpp"
#include "angry_yoba/material.hpp"

class Material;



class Materials : public Singleton<Materials>
{
public:
   void add(const Material & material);
   const Material & getm(const std::string & name) const;
   const std::string & getName(const Material & material);

private:
   std::unordered_map<std::string, Material> _materials;
};

#endif // MATERIALS_HPP
