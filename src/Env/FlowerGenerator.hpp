
/*
* POOSV EPFL 2022
* @authors:
*/

#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Collider.hpp"
#include "Utility/Utility.hpp"

class FlowerGenerator
{

public:

  /**
    * @brief contructor
    */
  FlowerGenerator();

  /**
    * @brief destructor
    */
  ~FlowerGenerator() = default;

  /**
    * @brief manages the evolution of its instances at each time step dt
    *
    * @param Time dt
    */
  void update(sf::Time dt);

  /**
    * @brief resets its time counter to zero
    */
  void reset();

private:
  sf::Time counter;
};

