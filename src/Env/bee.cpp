#include "Bee.hpp"
#include "Application.hpp"
#include <cmath>
#include <Random/Random.hpp>

Bee::Bee(double const& radius, Vec2d const& center, double energyLevel, double speed, Hive &hive)
    :Collider(center,radius), hive_(hive)
{
    this->energyLevel_ = energyLevel;
    this->speed_ = Vec2d::fromRandomAngle()*speed;

}

j::Value const& Bee::getConfig()
{
    return getValueConfig()["simulation"]["bees"]["scout"];
}




void Bee::update(sf::Time dt)
{
    double alpha;
    double p = getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble();
    double alphaMax = getConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble();
    if (bernoulli(p))
    {
        alpha = uniform(-alphaMax,alphaMax);
        speed_.rotate(alpha);
    }

    Vec2d dx = speed_ * dt.asSeconds();
    Vec2d possible_position = getPosition() + dx;
    if(getAppEnv().getWorld().isFlyable(possible_position))
    {
        move(dx);
    }
    else
    {
        double beta;
        if(bernoulli(0.5))
        {
            beta = PI/4;
        }
        else
        {
            beta = -PI/4;
        }
       speed_.rotate(beta);
    }
}

void Bee::drawOn(sf::RenderTarget &target) const
{
    Vec2d center = getPosition();
    double radius = getRadius();
    double alpha = speed_.angle();

    auto beeSprite = buildSprite(center,radius,getAppTexture(getValueConfig()["simulation"]["bees"]["scout"]["texture"].toString())) ;
    if (cos(alpha)<=0)
    {

        beeSprite.scale(1, -1);
        beeSprite.rotate(alpha/DEG_TO_RAD);
    }
    target.draw(beeSprite);
    //beeprite.rotate(alpha/57.296);
}

