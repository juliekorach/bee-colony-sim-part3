#ifndef FLOWER_HPP
#define FLOWER_HPP

#endif // FLOWER_HPP

/*
* POOSV EPFL 2022
* @authors:
*/

#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Collider.hpp"
#include "Utility/Utility.hpp"

class Flower : public Collider {
public:
   Flower(double const& p, double const& r, Vec2d const& c);
   ~Flower() = default;
   double takePollen();
   void drawOn(sf::RenderTarget& target) const;
   void update(sf::Time dt);
   double getPollen();

private:
   double pollen;
   sf::Texture loaded_texture;
};


