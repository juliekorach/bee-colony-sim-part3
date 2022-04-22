#ifndef ENV_HPP
#define ENV_HPP
#endif // ENV_HPP
/*
* POOSV EPFL 2022
* @authors:
*/

#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "Flower.hpp"
#include "FlowerGenerator.hpp"

class Env {

public :
   Env ();
   ~Env();
   void update(sf::Time dt);
   void drawOn(sf::RenderTarget& target);
   void reset();
   void loadWorldFromFile();
   void saveWorldToFile();
   double getSize();
   void resetControls();
   bool addFlowerAt(const Vec2d& p);
   void drawFlowerZone(sf::RenderTarget& target, Vec2d const& position);
   World& getWorld();
   std::vector<Flower *>& getFlowers();
private :
   World* ground_;
   std::vector<Flower*> flowers_;
   FlowerGenerator flowerGenerator_;
};

