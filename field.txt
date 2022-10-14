#include "headers.h"
#include "field.h"

Field::Field()
{
    state_work = 1;
    N = 0;
    NCL = 0;
    clouds.resize(1000);
    points.resize(100000);
}

void Field::state_gen(int n, double x, double y, double gx, double gy, int i)
{
    using namespace std;
    default_random_engine generator;
    normal_distribution<double> distributionx(x,gx);
    normal_distribution<double> distributiony(y,gy);

    clouds[i].create_space(n, i);
    NCL++;

    for (int k = 0; k < n; k++)
        clouds[i].set_par_to_p(distributionx(generator), distributiony(generator), k, N + k, 0);

    for (int k = 0; k < n; k++)
    {
        points[N + k].setx((clouds[i].get_points()[k]).getx());
        points[N + k].sety((clouds[i].get_points()[k]).gety());
        points[N + k].setnp((clouds[i].get_points()[k]).getNP());
    }

    N += n;
    state_work = 1;
}

int Field::file_save()
{
    std::ofstream fout("out.dat");

    for (int i = 0; i < N; i++)
        fout << points[i].getx() << "\t\t" << points[i].gety() << endl;

    fout.close();

    return 0;
}

int Field::size()
{
    return N;
}
int Field::nclouds()
{
    return NCL;
}
bool Field::work()
{
    return state_work;
}
vector <Cloud> *Field::get_cl()
{
    return &clouds;
}
double Field::getx_p(int k)
{
    return points[k].getx();
}
double Field::gety_p(int k)
{
    return points[k].gety();
}
Point Field::p(int k)
{
    return points[k];
}
vector <Point> Field::p()
{
    return points;
}
