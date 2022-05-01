#include "Hive.hpp"
#include "Utility/Utility.hpp"
#include <Application.hpp>
#include <Utility/Vec2d.hpp>

#include <SFML/Graphics.hpp>
#include "Bee.hpp"


Hive::Hive( double const& radius, Vec2d const& center)
   : Collider(center,radius)
{
   nectar_ = (getValueConfig()["simulation"]["hive"]["initial"]["nectar"]).toDouble();
}

Hive::~Hive()
{
   for (size_t i(0); i < bees_.size(); ++i)
   {
       delete bees_[i];
   }
   bees_.clear();

}

void Hive::drawOn(sf::RenderTarget &target) const
{
   Vec2d center = getPosition();
   double radius = getRadius()*2.5;
   auto hiveSprite = buildSprite(center,radius,getAppTexture(getAppConfig().hive_texture));
   target.draw(hiveSprite);
   if(isDebugOn()){
       Vec2d curseur=getApp().getCursorPositionInView();
       double text_to_display = nectar_;
       auto const text = buildText(to_nice_string(text_to_display), curseur, getAppFont(), 30, sf::Color::Red);
       target.draw(text);
   }
}

void Hive::update(sf::Time dt)
{}

void Hive::addBee()
{
    Bee bee(10.0, getPosition(),10.0,10.0, *this);
    Bee* beeptr = &bee;
    bees_.push_back(beeptr);
}

