
/*
* POOSV EPFL 2022
* @authors:
*/

#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "Utility/Utility.hpp"
#include "Collider.hpp"
#include "Hive.hpp"
#include "Interface/Updatable.hpp"
#include "Interface/Drawable.hpp"

class Bee : public Drawable, public Updatable, public Collider
{
public :

    Bee(double const& radius, Vec2d const& center, double energyLevel, double speed, Hive &hive);

    j::Value const& getConfig(); virtual

    void update(sf::Time dt);

    void drawOn(sf::RenderTarget &target) const;


private:
    Vec2d speed_;
    double energyLevel_;
    Hive & hive_;
};
