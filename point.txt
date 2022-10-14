#include "point.h"

Point::Point()
{
    x = 0;
    y = 0;
    NP = 0;
    colour = 0;
}
Point::~Point(){}

Point::Point(double x1, double y1, int NP1, int colour1)
{
    x = x1;
    y = y1;
    NP = NP1;
    colour = colour1;
}
void Point::setx(double x1)
{
    x = x1;
}
void Point::sety(double x2)
{
    y = x2;
}
void Point::setnp(int np)
{
    NP = np;
}
void Point::setc(int c)
{
    colour = c;
}
double Point::getx()
{
    return x;
}
double Point::gety()
{
    return y;
}
double Point::getNP()
{
    return NP;
}
double Point::getcolour()
{
    return colour;
}
