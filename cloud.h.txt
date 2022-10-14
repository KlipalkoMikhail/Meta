#ifndef CLOUD_H
#define CLOUD_H
using namespace std;
class Cloud
{
    private:
    bool state_work;                // состояние работы облака
    int N;                          // количество точек
    int NP;                         // номер облака
    vector <Point> points;          // вектор точек
    public:
    Cloud();
    int size();
    bool work();
    void set_par_to_p(double x, double y, int k, int np, int c);
    void create_space(int n, int np); // иницилизация
    double getx_p(int k);
    double gety_p(int k);
    int getnp_p(int k);
    vector <Point> get_points();
    ~Cloud();                    // деструктор
};

#endif
