
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
#include "Hive.hpp"

class Env {

public :

 /**
   * @brief constructor
   */
 Env ();

 /**
  * @brief destructor
  */
 ~Env();

 /**
  * @brief makes the environment evolve over a time step dt
  *
  * @param Time dt
  */
 void update(sf::Time dt);

 /**
  * @brief draws an environment
  *
  * @param RenderTarget target
  */
 void drawOn(sf::RenderTarget& target);

 /**
  * @brief regenerates the environment
  */
 void reset();

 /**
  * @brief loads the environment terrain from a file
  */
 void loadWorldFromFile();

 /**
  * @brief saves a new configuration in a file
  */
 void saveWorldToFile();

 /**
  * @brief getter
  *
  * @return a double : the size of the land
  */
 double getSize();

 /**
  * @brief
  */
 void resetControls();

 /**
  * @brief creates a flower positioned at p if it is possible
  *
  * @param a Vec2d p : a position (coordinates)
  *
  * @return true or false
  */
 bool addFlowerAt(const Vec2d& p);

 /**
  * @brief draws a ring centered in the center of the flower : green if it can receive a flower and red otherwise
  *
  * @param RenderTarget target and a Vec2d position
  */
 void drawFlowerZone(sf::RenderTarget& target, Vec2d const& position);

 /**
  * @brief getter
  *
  * @return a World : a land
  */
 World& getWorld();

 /**
  * @brief getter
  *
  * @return a vector of Flower*
  */
 std::vector<Flower *>& getFlowers();

 std::vector<Hive *>& getHives();

 bool addHiveAt(const Vec2d& position);

 bool IsCollidingHive(Hive* &hive) const;

 bool IsCollidingFlower(Hive* &hive) const;

 Hive* getCollidingHive(const Collider& body);

 Flower* getCollidingFlower(const Collider& body);



private :
 World* ground_;
 std::vector<Flower*> flowers_;
 FlowerGenerator flowerGenerator_;
 std::vector<Hive*> hives_;
};



