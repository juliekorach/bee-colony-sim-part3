/*
* POOSV EPFL 2022
* @authors:
* Marco Antognini
*/

#include <Env/Env.hpp>
#include <Application.hpp>
#include <Utility/Vertex.hpp>
#include <iostream>
#include <string>
#include <Random/Random.hpp>


void Env::update(sf::Time dt)
{
    size_t initialSize = flowers_.size();
    flowerGenerator_.update(dt);
    for (size_t i(0); i < initialSize; ++i)
    {
        flowers_[i]->update(dt);
        if (flowers_[i]->getPollen() <0)
        {
            std::cout << "Delete flower:" << flowers_[i] << " number:" << i << std::endl;
            delete flowers_[i];
            flowers_[i]=nullptr;
        }
    }
    flowers_.erase(std::remove(flowers_.begin(), flowers_.end(), nullptr), flowers_.end());
}

void Env::drawOn(sf::RenderTarget& target){
   ground_->drawOn(target);
   for (unsigned int i(0); i<flowers_.size(); i++){
       flowers_[i]->drawOn(target);
   }
}

World& Env::getWorld()
{
    return *ground_;
}

void Env::reset()
{
   ground_->reset();
   flowerGenerator_.reset();
   flowers_.clear();
   flowerGenerator_.reset();
}

Env::Env () {
    ground_ = new World;
   try {
       loadWorldFromFile();
   }
   catch(const std::string& error){
       std::cerr << "Error : " << error << std::endl;
       Env::reset();
   }
}

Env::~Env(){
   // flowers_.~vector();
    flowers_.clear();
    delete ground_;
}

void Env::loadWorldFromFile(){
   ground_->loadFromFile();
}

void Env::saveWorldToFile(){
   ground_->saveToFile();
}

double Env::getSize(){
   return (ground_->getSize());
}

void Env::resetControls(){

}


bool Env::addFlowerAt(const Vec2d& c) {
   double nbflower(flowers_.size());
   double threshold = (getValueConfig()["simulation"]["env"]["max flowers"]).toDouble();
   if (ground_->isGrowable(c) && nbflower<threshold)
   {
       double r = getAppConfig().flower_manual_size;
       double p = uniform(getAppConfig().flower_nectar_min, getAppConfig().flower_nectar_max);
       Flower* newflower = new Flower(p, r, c);
       flowers_.push_back(newflower);
       return true;
   }
   return false;
}

void Env::drawFlowerZone(sf::RenderTarget& target, Vec2d const& position){
   double thickness = 3.0;
   int size = (getValueConfig()["simulation"]["env"]["initial"]["flower"]["size"]["manual"]).toInt();
   if(ground_->isGrowable(position)){
       auto shape = buildAnnulus(position, size, sf::Color::Green, thickness);
       target.draw(shape);
   } else {
       auto shape = buildAnnulus(position, size, sf::Color::Red, thickness);
       target.draw(shape);
   }
}

std::vector<Flower *> & Env::getFlowers()
{
    return flowers_;
}


