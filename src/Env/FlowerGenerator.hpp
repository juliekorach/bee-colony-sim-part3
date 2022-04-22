#ifndef FLOWERGENERATOR_HPP
#define FLOWERGENERATOR_HPP

#endif // FLOWERGENERATOR_HPP

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

    FlowerGenerator()=default;

    void update(sf::Time dt);

    void reset();

private:

    sf::Time counter;
};
