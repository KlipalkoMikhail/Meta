#include <iostream>
#include <list>
#include <vector>
#include <cmath>
#include "headers.h"
#include <cfloat>
#define EPSILON 0.000001
#include "controller.h"

void Cluster::ccenter(std::vector <Point> &points, int N)
{
    double xmean = 0;
    double ymean = 0;

    int ln = 0;
    for (int i = 0; i < N; i++)
    {
        ln += blong[i];
    }
    xmean = scalx(points, blong, N)/ln;
    ymean = scaly(points, blong, N)/ln;

    center.setx(xmean);
    center.sety(ymean);
    center.setc(colour);
}

void Cloud::set_par_to_p(double x, double y, int k, int np, int c)
{
    points[k].setx(x);
    points[k].sety(y);
    points[k].setc(c);
    points[k].setnp(np);
}

double Cloud::getx_p(int k)
{
    return points[k].getx();
}

double Cloud::gety_p(int k)
{
    return points[k].gety();
}

int Cloud::getnp_p(int k)
{
    return points[k].getNP();
}

Cloud::Cloud()
{
    state_work = 0;
    N = 0;
}
int Cloud::size()
{
    return N;
}
bool Cloud::work()
{
    return state_work;
}
void Cloud::create_space(int n, int np) // иницилизация
{
    N = n;
    state_work = 1;
    NP = np;
    points.resize(N);
}

vector <Point> Cloud::get_points()
{
    return points;
}
Cloud::~Cloud(){}                      // деструктор
