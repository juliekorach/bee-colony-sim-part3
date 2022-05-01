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
        if (flowers_[i]->getPollen() <=0)
        {
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


    for (unsigned int i(0); i<hives_.size(); i++){
        hives_[i]->drawOn(target);
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
    hives_.clear();
}

Env::Env () {
    ground_ = new World;
    try {
        loadWorldFromFile();
    }
    catch(const std::string& error){

        Env::reset();
    }
}

Env::~Env(){
    for( size_t i(0); i < flowers_.size(); ++i)
    {
        delete flowers_[i];
    }
    flowers_.clear();

    delete ground_;

    for( size_t i(0); i < hives_.size(); ++i)
    {
        delete hives_[i];
    }
    hives_.clear();
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

std::vector<Flower *> &Env::getFlowers()
{
    return flowers_;
}

std::vector<Hive *> &Env::getHives()
{
    return hives_;
}

bool Env::IsCollidingHive(Hive* &hive) const{
    for( size_t i(0); i < hives_.size(); ++i)
    {
        if (hive->isColliding((*hives_[i]))){
            return true;
        }
    }
    return false;
}

bool Env::IsCollidingFlower(Hive* &hive) const{
    for( size_t i(0); i < flowers_.size(); ++i)
    {
        if (hive->isColliding((*flowers_[i]))){
            return true;
        }
    }
    return false;
}

bool Env::addHiveAt(const Vec2d& position){
    double r = uniform(getAppConfig().hive_min_size, getAppConfig().hive_max_size);
    Hive* newhive = new Hive(r, position);
    if (IsCollidingHive(newhive)==false and IsCollidingFlower(newhive)==false){
        hives_.push_back(newhive);
        return true;
    }
    return false;
}

Hive* Env::getCollidingHive(const Collider& body){
    for( size_t i(0); i < hives_.size(); ++i)
    {
        double r = getAppConfig().hive_manual_size*getAppConfig().hiveable_factor;
        Collider collider(hives_[i]->getPosition(),r);
        if (body.isColliding(collider)){
            return hives_[i];
        }
    }
    return nullptr;
}

Flower* Env::getCollidingFlower(const Collider& body){
    for( size_t i(0); i < flowers_.size(); ++i)
    {
        if (body.isColliding(*flowers_[i])){
            return flowers_[i];
        }
    }
    return nullptr;
}



