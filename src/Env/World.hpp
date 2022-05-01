/*
* POOSV EPFL 2022
* @authors:
*/

#pragma once

#include <vector>

#include <Utility/Vec2d.hpp>
#include <SFML/Graphics.hpp>
#include "Interface/Drawable.hpp"

enum class Kind : short {Herbe,Eau,Roche};
struct Seed
{
  sf::Vector2i coordinates;
  Kind type;
};

class World : public Drawable

{
public:

  /**
   * @brief constructor
   */
  World() = default;

  /**
   * @brief destructor
   */
  ~World() = default;

  /**
   * @brief initialize everything that depends on the configuration file
   */
  void reloadConfig();

  /**
   * @brief draw an array of cells_
   *
   * @param RenderTarget target
   */
  void drawOn(sf::RenderTarget& target) const;

  /**
   * @brief translates each cell in cells_ into a vertex set
   */
  void reloadCacheStructure();

  /**
   * @brief generates the terrain
   */
  void updateCache();

  /**
   * @brief initialize all attributes of world
   *
   * @param a boolean variable regenerate
   */
  void reset(bool regenerate=true);

  /**
   * @brief gives the graphic size of the grid
   *
   * @return a double, the graphic size of the grid
   */
  double getSize() const;

  /**
   * @brief allows the initialization of terrain cells from files
   */
  void loadFromFile();

  /**
   * @brief allows one movement of seeds on the grid
   */
  void step();

  /**
   * @brief put the seed inside the toric world
   *
   * @param seed
   */
  void clamp_seed(Seed &seed);

  /**
   * @brief moves the seed from the squares of its neighborhood randomly
   *
   * @param seed
   */
  void move(Seed &seed);

  /**
   * @brief allows several movements of seeds on the grid and regenerates the drawing if the boolean is true
   *
   * @param an integer indicating the number of times to call step and a boolean with false as the default value
   */
  void steps(int nb, bool regenerate=0);

  /**
   * @brief associate a kind to a cell when it moves on one of the squares of its neighborhood
   *
   * @param coordinates and kind of a cell
   */
  void setCellType(int x, int y, Kind kind);

  /**
    * @brief calculates the rate of water cells around a cell
    *
    * @param coordinates x, y and kind of a cell, a cell array
    *
    * @return the rate
    */
  double neighboring_ratio(int x,int y,std::vector<Kind> cells, Kind k);

  /**
    * @brief performs a smoothing operation on the grid
    */
  void smooth();

  /**
    * @brief performs several smoothing operations on the grid
    *
    * @param an integer indicating the number of times to call smooth and a boolean with         false as the default value
    */
  void smooths(int n, bool regenerate=false);

  /**
    * @brief allows to save a new configuration in a file
    */
  void saveToFile();

  void setHumidity();

  bool isGrowable(const Vec2d& p);

  double getHumidity(const Vec2d & center);





private:

  std::vector<Kind> cells_;
  int nbCells_;
  double cellsSize_;
  std::vector <sf::Vertex> grassVertexes_;
  std::vector <sf::Vertex> waterVertexes_;
  std::vector <sf::Vertex> rockVertexes_;
  std::vector <sf::Vertex> humidityVertexes_;
  sf::RenderTexture renderingCache_;
  std::vector<Seed> seeds_;
  int nbGrassSeeds_;
  int nbWaterSeeds_;
  std::vector<double> humidity_;
  double lambda_;
  double eta_;
  double threshold_;

  int humidityRange_;
  double maxHumidity_;
  double minHumidity_;

  /**
   * @brief indicates the position of a cell in cells_
   *
   * @param the coordinates (x and y) of a cell
   *
   * @return the position of a cell in cells_
   */
  int index(int x, int y);
};

