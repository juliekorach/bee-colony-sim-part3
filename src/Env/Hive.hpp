
/*
* POOSV EPFL 2022
* @authors:
*/

#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>

#include "Utility/Utility.hpp"
#include "Collider.hpp"

#include "Interface/Updatable.hpp"
#include "Interface/Drawable.hpp"

class Bee;

class Hive : public Collider, public Updatable, public Drawable
{
public:

  Hive( double const& radius, Vec2d const& center);

  ~Hive();

  void drawOn(sf::RenderTarget &target) const;

  void update(sf::Time dt);

  double dropPollen(double qte);

  void takeNectar(double qte);

  bool isHiveable();

protected:

  void addBee();


private:

  std::vector<Bee*> bees_;

  double nectar_;
};

