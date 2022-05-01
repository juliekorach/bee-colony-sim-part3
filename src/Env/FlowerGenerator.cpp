/*
* POOSV EPFL 2022
* @authors:
* Marco Antognini
*/

#include <Env/Flower.hpp>
#include <Application.hpp>
#include <Utility/Vec2d.hpp>
#include <Random/Random.hpp>

FlowerGenerator ::FlowerGenerator():
    counter(sf::Time::Zero) {}

void FlowerGenerator::update(sf::Time dt)
{
    counter += dt;
    if (counter > sf::seconds(getAppConfig().flower_generator_delay) and getAppConfig().active_flower_generator)
    {
        counter = sf::Time::Zero;

        Vec2d newCenter(uniform(0,(int)getApp().getEnvSize().x()), uniform(0,(int)getApp().getEnvSize().y()));
        getAppEnv().addFlowerAt(newCenter);
    }
}

void FlowerGenerator::reset()
{
    counter = sf::Time::Zero;
}
