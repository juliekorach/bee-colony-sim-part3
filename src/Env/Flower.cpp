/*
* POOSV EPFL 2022
* @authors:
* Marco Antognini
*/

#include <Env/Flower.hpp>
#include <Application.hpp>
#include <Utility/Vec2d.hpp>
#include <Random/Random.hpp>

Flower :: Flower(double const& p, double const& r, Vec2d const& c) : Collider(c,r)
{
    pollen = p;
    j::Value textures = getValueConfig()["simulation"]["flower"]["textures"];
    int texture_indx = uniform(0, (int)(textures.size()-1));
    this->loaded_texture = getAppTexture(textures[texture_indx].toString());
}


void Flower :: drawOn(sf::RenderTarget& target) const {
    double radius = getRadius();
    Vec2d center = getPosition();
    auto flowerSprite = buildSprite(center, radius, loaded_texture);
    target.draw(flowerSprite);
}

void Flower::update(sf::Time dt)
{
    double hum = getAppEnv().getWorld().getHumidity(getPosition());
    double growthThreshold = getAppConfig().flower_growth_threshold;

    pollen += (dt.asSeconds() * log(hum/growthThreshold));
    double pollenLimit =  getAppConfig().flower_growth_split;
    if (pollen>pollenLimit)
    {
        pollen = pollen/2;
        int a=0;
        Vec2d newCenter;
        do {
            double distance = uniform(1.5 * getRadius(),2.5 * getRadius());
            newCenter = getPosition() + Vec2d::fromRandomAngle()*distance;
            ++a;
        } while (getAppEnv().getWorld().isGrowable(newCenter)==false and a<100);
        if ((getAppEnv().getWorld().isGrowable(newCenter)))
        {
            Flower *newFlower = new Flower(pollen,getAppConfig().flower_manual_size,newCenter);
            std::vector<Flower *> flowers_list = getAppEnv().getFlowers();
            std::cout << "Created flower pollen:" << newFlower << " number:" << flowers_list.size() << " at:" << newCenter.x() << "," << newCenter.y() << std::endl;
            getAppEnv().getFlowers().push_back(newFlower);
        }
    }
}

double Flower::getPollen()
{
    return pollen;
}
