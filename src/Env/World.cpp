#include <Env/World.hpp>
#include <Application.hpp>
#include <Utility/Vertex.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include <float.h>
#include <Random/Random.hpp>




int World::index(int x, int y)
{
    return y * nbCells_ + x;
}

void World::reloadConfig()
{
    this->nbCells_ = getAppConfig().world_cells;
    this->cells_= std::vector<Kind>(nbCells_ * nbCells_, Kind::Roche);
    this->cellsSize_ = getAppConfig().world_size/nbCells_;
    this->nbGrassSeeds_ = getAppConfig().world_nb_grass_seeds;
    this->nbWaterSeeds_ = getAppConfig().world_nb_water_seeds;
    std::vector<Seed> seeds(nbGrassSeeds_ + nbWaterSeeds_);
    this->seeds_ = seeds;
    this->humidity_.assign(nbCells_*nbCells_, 0.0);
    this->eta_=getAppConfig().world_humidity_init_level;
    this->lambda_=getAppConfig().world_humidity_decay_rate;
    this->threshold_=getAppConfig().world_humidity_threshold;
    this->humidityRange_=0;
    while(eta_*std::exp(-humidityRange_/lambda_)>threshold_)
    {
        ++humidityRange_;
    }

}

void World::drawOn(sf::RenderTarget& target)
{

    if (getAppConfig().showHumidity())
    {
        target.draw(humidityVertexes_.data(), humidityVertexes_.size(), sf::Quads);
    }
    else
    {
        sf::Sprite cache(renderingCache_.getTexture());
        target.draw(cache);
    }

}

void World::reloadCacheStructure()
{
    grassVertexes_ = generateVertexes(
                getValueConfig()["simulation"]["world"]["textures"],
            nbCells_,
            cellsSize_);

    waterVertexes_ = generateVertexes(
                getValueConfig()["simulation"]["world"]["textures"],
            nbCells_,
            cellsSize_);

    rockVertexes_ = generateVertexes(
                getValueConfig()["simulation"]["world"]["textures"],
            nbCells_,
            cellsSize_);

    humidityVertexes_ = generateVertexes(
                getValueConfig()["simulation"]["world"]["textures"],
            nbCells_,
            cellsSize_);

    renderingCache_.create(nbCells_*cellsSize_, nbCells_*cellsSize_);
}



void World::updateCache()
{
    renderingCache_.clear();
    sf::RenderStates rs;
    minHumidity_ = DBL_MAX;
    maxHumidity_ = 0;
    for (size_t i(0); i < humidity_.size(); ++i)
    {
        if(humidity_[i]>maxHumidity_)
            maxHumidity_ = humidity_[i];

        if(humidity_[i]<minHumidity_)
            minHumidity_ = humidity_[i];
    }

    for (int i(0); i<nbCells_; ++i)
    {
        for (int j(0); j<nbCells_; ++j)
        {
            std::vector<size_t> ind_ = indexesForCellVertexes(i, j, nbCells_ );
            switch(cells_[index(i,j)])
            {
            case Kind::Eau :
                for (auto ind:ind_) {
                    waterVertexes_[ind].color.a = 255;
                    grassVertexes_[ind].color.a = 0;
                    rockVertexes_[ind].color.a = 0;
                }
                break;
            case Kind::Roche :
                for (auto ind : ind_) {
                    waterVertexes_[ind].color.a = 0;
                    grassVertexes_[ind].color.a = 0;
                    rockVertexes_[ind].color.a = 255;
                }
                break;
            case Kind::Herbe :
                for (auto ind:ind_) {
                    waterVertexes_[ind].color.a = 0;
                    grassVertexes_[ind].color.a = 255;
                    rockVertexes_[ind].color.a = 0;
                }
                break;
            }



            for (auto ind:ind_)
            {
                double niveau_bleu = (humidity_[index(i, j)] - minHumidity_)/(maxHumidity_ - minHumidity_) * 255;
                humidityVertexes_[ind].color = sf::Color(0, 0, niveau_bleu);
            }
        }
    }

    rs.texture = &getAppTexture(getAppConfig().rock_texture); // texture liée à la roche
    renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rs);
    rs.texture = &getAppTexture(getAppConfig().water_texture); // texture liée à l'eau
    renderingCache_.draw(waterVertexes_.data(), waterVertexes_.size(), sf::Quads, rs);
    rs.texture = &getAppTexture(getAppConfig().grass_texture); // texture liée à l'herbe
    renderingCache_.draw(grassVertexes_.data(), grassVertexes_.size(), sf::Quads, rs);
    renderingCache_.display();

}

void World::reset(bool regenerate)
{
    reloadConfig();
    reloadCacheStructure();

    for(int h(0); h < nbGrassSeeds_ + nbWaterSeeds_; ++h)
    {
        int x = uniform(0,nbCells_-1);
        int y = uniform(0,nbCells_-1) ;
        sf::Vector2i co(x,y);
        seeds_[h].coordinates = co;
        if (h < nbGrassSeeds_)
        {
            seeds_[h].type = Kind::Herbe;
            cells_[index(x, y)] = Kind::Herbe;
        }
        else
        {
            seeds_[h].type = Kind::Eau;
            cells_[index(x, y)] = Kind::Eau;
        }
    }
    if (regenerate) {
        steps(getAppConfig().world_generation_steps);
        smooths(getAppConfig().world_generation_smoothness_level);
    }

    setHumidity();
    updateCache();
}

double World::getSize() const
{
    return nbCells_*cellsSize_;
}

void World::loadFromFile()
{
    reloadConfig();
    std::string line1;
    std::string line2;
    std::ifstream in;
    std::string nom(getApp().getResPath() + getAppConfig().world_init_file);
    in.open(nom.c_str());
    if (!in.fail()){
        std::cout << "fichier map = " << nom << std::endl;
        in >> std::ws >> line1;
        nbCells_ = std::stoi(line1);
        in >> std::ws >> line2;
        cellsSize_ = std::stod(line2);
        //cells_= std::vector<Kind>(nbCells_*nbCells_);
        for(size_t i(0); i<cells_.size(); ++i)
        {
            short var;
            in >> std::ws >> var;
            cells_[i] = static_cast<Kind>(var);

        }


        for (size_t i(0); i<cells_.size(); ++i)
        {
            double hum;
            in >> std::ws >> hum;
            humidity_[i] = static_cast<double>(hum);

        }

    }
    else
    {
        throw std::runtime_error("file does not exist");
    }
    in.close();
    reloadCacheStructure();
    updateCache();
    setHumidity();
}

void World::clamp_seed(Seed &seed)
{
    seed.coordinates.x = std::min(nbCells_ - 1, seed.coordinates.x);
    seed.coordinates.x = std::max(0, seed.coordinates.x);
    seed.coordinates.y = std::min(nbCells_ - 1, seed.coordinates.y);
    seed.coordinates.y = std::max(0, seed.coordinates.y);
}

void World::move(Seed &seed)
{
    int x;
    int y;
    do {
        x = (uniform(-1,1));
        y = (uniform(-1,1));

    } while (not(abs(x-y)==1));

    seed.coordinates.x += x;
    seed.coordinates.y += y;
    clamp_seed(seed);
    setCellType(seed.coordinates.x, seed.coordinates.y, seed.type);
}

void World::setCellType(int x, int y, Kind kind)
{
    int idx = index(x, y);
    if (cells_[idx] != Kind::Eau)
    {
        cells_[idx] = kind;
    }
}

void World::step()
{
    double proba = getAppConfig().water_seeds_teleport_proba;
    for (size_t  i(0); i < seeds_.size(); ++i)
    {
        if (seeds_[i].type == Kind::Herbe)
        {
            move(seeds_[i]);
        }
        else if (seeds_[i].type == Kind::Eau)
        {

            if (bernoulli(proba))
            {
                int x = uniform(0, nbCells_-1);
                int y = uniform(0, nbCells_-1) ;
                sf::Vector2i co(x, y);
                seeds_[i].coordinates = co;
                setCellType(x, y, Kind::Eau);

            }
            else {
                move(seeds_[i]);
            }
        }
    }
}



void World::steps(int nb, bool regenerate)
{
    for ( int i(0); i < nb; ++i)
    {
        step();
    }
    if (regenerate)
    {
        updateCache();
    }
}

double World::neighboring_ratio(int x,int y,std::vector<Kind> cells, Kind k)
{
    int countKind(0);
    int countNeighbors(0);
    for (int j(y-1); j<=(y+1); ++j)
    {
        if ( j >= 0 and j <= nbCells_-1)
        {
            for (int i(x-1); i<=(x+1); ++i)
            {
                if ( i >= 0 and i <= nbCells_-1)
                {
                    countNeighbors += 1;
                    if (cells[index(i,j)]== k )
                        countKind += 1;
                }
            }
        }
    }


    return (double(countKind)/(countNeighbors-1));
}


void World::smooth()
{
    auto cells_copy = cells_;
    for (int i(0); i < nbCells_*nbCells_; ++i)
    {
        if (cells_[i]==Kind::Roche)
        {
            int y = i/nbCells_;
            int x = i%nbCells_;
            double ratio = neighboring_ratio(x, y,cells_, Kind::Herbe);

            if(ratio > getAppConfig().smoothness_grass_neighbor_ratio)
            {
                cells_copy[i] = Kind::Herbe;
            }


        }

        if (cells_[i]==Kind::Roche or cells_[i]==Kind::Herbe)
        {
            int y = i/nbCells_;
            int x = i%nbCells_;
            double ratio = neighboring_ratio(x, y, cells_, Kind::Eau);

            if(ratio > getAppConfig().smoothness_water_neighbor_ratio)
            {
                cells_copy[i] = Kind::Eau;
            }

        }


    }

    std::swap(cells_,cells_copy);
}

void World::smooths(int nb, bool regenerate)
{
    for (int i(0); i < nb; ++i)
    {
        smooth();
    }

    if(regenerate)
    {
        updateCache();
    }
}


void World::saveToFile()
{
    std::ofstream out;
    std::string nom(getApp().getResPath() + getAppConfig().world_init_file);
    out.open(nom.c_str());
    if (!out.fail()){
        std::cout << "fichier map = " << nom << std::endl;
        out << nbCells_ << std::endl;
        out << cellsSize_ << std::endl;
        for(size_t i(0); i<cells_.size(); ++i)
        {
            short var;
            var = static_cast<short>(cells_[i]);
            out << var << " ";
        }

        out << std::endl;

        for (size_t i(0); i<cells_.size(); ++i)
        {
            double hum;
            hum = static_cast<double>(humidity_[i]);
            out << hum << " ";

        }
    }
    else
    {
        throw std::runtime_error("error opening the file");
    }
    out.close();
    reloadCacheStructure();
    updateCache();
    setHumidity();
}

void World::setHumidity()
{
    maxHumidity_=0;
    minHumidity_=DBL_MAX;
    double distance;

    this->humidity_.assign(nbCells_*nbCells_, 0.0);
    for (int i(0); i<nbCells_*nbCells_; ++i)
    {
        if (cells_[i]==Kind::Eau)
        {
            int x=i%nbCells_;
            int y=i/nbCells_;

            for (int k(y-humidityRange_); k <= (y+humidityRange_); ++k)
            {
                for (int l(x-humidityRange_); l <= (x+humidityRange_); ++l)
                {
                    if(l>=0 and l<nbCells_ and k>=0 and k<nbCells_)
                    {
                        distance=std::hypot(l-x,k-y);

                        if (eta_*std::exp(-distance/lambda_)>threshold_)
                        {
                            humidity_[index(l,k)] += eta_*std::exp(-distance/lambda_);
                        }

                    }
                }
            }
        }
    }
}

bool World::isGrowable(const Vec2d& p){
    double i = p.x() / cellsSize_;
    double j = p.y() / cellsSize_;
    return (cells_[index(i,j)] == Kind::Herbe );
}

double World::getHumidity(const Vec2d& center)
{
    return humidity_[index(center.x()/cellsSize_, center.y()/cellsSize_)];
}





