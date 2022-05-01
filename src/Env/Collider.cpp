#include <Env/Collider.hpp>
#include <cmath> // abs and fmod
#include <algorithm>
#include <Application.hpp>
#include <iostream>

Collider::Collider(Vec2d const& c, double const& r)
    : center_(c), radius_(r)
{ center_ = clamp(center_); }

Collider::Collider(const Collider& co)
    : center_(co.center_),radius_(co.radius_)
{ center_ = clamp(center_); }


Vec2d clamp(Vec2d const& center)
{
    static const auto worldSize = getApp().getEnvSize();
    static const auto width  = worldSize.x(); // largeur
    static const auto height = worldSize.y(); // hauteur

    double x(center.x());
    double y(center.y());
    x = fmod(x,width);
    y = fmod(y,height);

    if(x>=width) // on teste les différents cas possibles
    {
        x -=width;
    }
    if (x<0){
        x +=width;
    }
    if(y>=height)
    {
        y -= height;
    }
    if (y<0){
        y +=height;
    }

    Vec2d newcenter(x,y);
    return newcenter;
}

double Collider::getRadius() const
{
    return radius_;
}

const Vec2d& Collider::getPosition() const
{
    return center_;
}

Collider& Collider::operator=(Collider const& co)
{
    radius_ = co.radius_;
    center_ = co.center_;
    return *this;
}

Vec2d Collider::directionTo(const Vec2d to) const
{
    static const auto worldSize = getApp().getEnvSize();
    static const auto width  = worldSize.x(); // largeur
    static const auto height = worldSize.y(); // hauteur

    Vec2d bestTo = to;
    double shortest = distance(center_, to);
    for (int i(-1); i<=1; ++i) // on modifie d'abord la largeur puis la hauteur
    {
        for(int j(-1); j<=1; ++j)
        {
            Vec2d newto(to.x()+i*width,to.y()+j*height);
            if(distance(center_, newto)<shortest)
            {
                shortest = distance(center_,newto);
                bestTo = newto;
            }
        }
    }
    return bestTo-center_;
}

Vec2d Collider::directionTo(const Collider& co) const
{
    return directionTo(co.center_);
}

double Collider::distanceTo(const Vec2d to) const
{
    Vec2d newto = directionTo(to);
    return newto.length();
}

double Collider::distanceTo(const Collider& co) const
{
    return distanceTo(co.center_);
}

void Collider::move(const Vec2d& dx)
{
    center_ = center_ + dx;
    center_ = clamp(center_);
}

Collider& Collider::operator+=(const Vec2d& dx)
{
    move(dx);
    return *this;
}

bool Collider::isColliderInside(const Collider& other) const
{
    if(radius_>=other.radius_ and distanceTo(other.center_)<=(radius_ - other.radius_))
    {
        return true;
    } else {
        return false;
    }
}

bool Collider::isColliding(const Collider& other) const
{
    if (distanceTo(other) <= (radius_ + other.radius_))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Collider::isPointInside(const Vec2d& point) const
{
    if (distanceTo(point)<=radius_)
    {
        return true;
    }
    else {
        return false;
    }
}


bool Collider::operator>(const Collider& other) const
{
    if(isColliderInside(other))
    {
        return true;
    }
    else {
        return false;
    }
}

bool Collider::operator|(const Collider& other) const
{
    if(isColliding(other))
    {
        return true;
    }
    else {
        return false;
    }
}

bool Collider::operator>(const Vec2d& point) const
{
    if(isPointInside(point))
    {
        return true;
    }
    else {
        return false;
    }
}

void Collider::affiche(std::ostream& o) const
{
    o<<"Collider: position = <" << center_ << "> radius = < "<< radius_ << ">"<< std::endl;
}

std::ostream& operator<<(std::ostream& sortie, const Collider& co)
{
    co.affiche(sortie);
    return sortie;
}



