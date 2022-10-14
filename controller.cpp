#include "headers.h"
#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>
#include "controller.h"
#include <iterator>
#include <list>
#include <cmath>

double distance(Point x, Point y)
{
    return sqrt((x.getx() - y.getx())*(x.getx()- y.getx()) + (x.gety() - y.gety())*(x.gety() - y.gety()));
}

void Controller::raise()
{
    NCL++;
}
Controller::Controller()
{
    Status_Work = 1;
    NCL = 0;
}

Controller::~Controller(){}

int Controller::Print_Field()
{

    using namespace std;
    if (field.work() != 1)
    {
        cout << "Field is not created\n" << endl;
        return -1;
    }
    else
    {
        cout << " X " << "\t\t\t" << " Y " << endl;
        for (int i = 0; i < field.size(); i++)
        {
            cout << field.getx_p(i) << "\t\t";
            cout << field.gety_p(i) << endl;
        }

    }
    field.file_save();
    return 0;
}

void Controller::Create_Cloud(int N, double x, double y, double gx, double gy)
{
    cin >> N >> x >> y >> gx >> gy;
    field.state_gen(N, x, y, gx, gy, NCL);
    std::cout << "Great!" << endl;
}

int Controller::Print_Cloud(int i)
{
    using namespace std;
    if (((*field.get_cl())[i]).work() != 1)
    {
        cout << "Cloud is not created\n" << endl;
        return -1;
    }
    else
    {
        cout << " X " << "\t\t\t" << " Y " << endl;

        for (int k = 0; k < (*field.get_cl())[i].size(); k++)
        {
            cout << fixed << setprecision(6) << (*field.get_cl())[i].getx_p(k) << "\t\t";
            cout << fixed << setprecision(6) << (*field.get_cl())[i].gety_p(k) << "\t\t" << "\t Col " << (*field.get_cl())[i].getnp_p(k) << endl;
        }
    }
    return 0;
}

// поиск результата по номеру выполненного алгоритма
void Controller::find_cluster(int k)
{
    // создаем вектор точек
    vector <Point> points = field.p();

    // печатаем в файл
    sv.cluster_save(k, field.size(), points);

    cout << "Check the folder. The result are saved." << endl;
}

// Волновой алгоритм, входных параметров не принимает
void Controller::Wave()
{
    // создаем кластеры, матрицу расстояний, матрицу графа, векторы a, b, nul, переменную N - число точек, T - поджог
    vector <Cluster> clusters;

    int N = field.size();

    vector <vector <double>> D;
    vector <vector <int>> B;

    vector <int> a(N);
    vector <int> b(N);
    vector <int> nul(N);

    int T = 1;

    // выделяем память
    D.resize(N);
    B.resize(N);
    double step = 0;

    // выделяем память
    for (int i = 0; i < N; i++)
    {
        D[i].resize(N);
        B[i].resize(N);
    }

    // создаем матрицу расстояний
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            D[i][j] = distance(field.p(i), field.p(j));
            step += D[i][j]/2;
        }
    }

    // считаем порог
    step = step/(2*N*(N - 1));

    // создаем матрицу графа
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if(D[i][j] < step)
                B[i][j] = 1;
            else B[i][j] = 0;
        }
    }

    // освобождаем память
    D.clear();

    // создаем листы: index - точки, заженные в момент T, indexnew - все соседи этих заженных точек
    list <int> index;
    list <int> indexnew;
    // создаем векторы: temp - просто для обмена значений, blong - вектор заженных и не заженных точек
    vector <int> temp(N);
    vector <int> blong(N);

    // зажигаем точку
    for (int k = 0; k < N; k++)
    {
        if (!blong[k])
        {
            // создаем кластер и добавляем в него точку
            Cluster cl;
            cl.reserve(N);
            blong[k] = 1;
            a[k] = 1;
            cl.add_p(k);

            // создаем лист, хранящий точки компоненты графа
            b = a;
            index.push_back(k);

            // зажигаем по циклу остальные точки
            while(1)
            {
                for(list <int> :: iterator it = index.begin(); it != index.end(); it++)
                {
                    for(int i = 0 ; i < N; i++)
                    {
                        // проверяем соединены ли точки ребром и загорелись ли они
                        if(((B[*it][i]) == 1) && (a[i] == 0) && (blong[i] == 0))
                        {
                            // зажигаем точку
                            b[i] = T + 1;
                            // добавляем в кластер
                            cl.add_p(i);
                            // считаем, что по ней уже прошли
                            blong[i] = 1;
                            // отправляем точку в компоненту графа (это временный список)
                            indexnew.push_back(i);
                        }
                    }
                }

                // здесь точки связной компоненты сохраняются в список index
                index.clear();
                index.merge(indexnew);
                indexnew.clear();

                if (a == b) break;
                temp = a;
                a = b;
                T = T + 1;
            }
            clusters.push_back(cl);
        }
        // обновляем параметры T, a, b
        T = 1;
        a = b = nul;
    }
    // сохраняем результат
    sv.save(clusters, 2);
    cout << "The wave algorithm has done its work\n";
}

// DBscan, m - число соседей, чтобы точка являлась ядром
void Controller::dbscan(int m)
{
    // создаем кластеры, матрицу расстояний, матрицу графа, векторы a, b, nul, переменную N - число точек, T - поджог
    vector <Cluster> clusters;

    int N = field.size();

    vector <vector <double>> D;
    vector <vector <int>> B;

    vector <int> a(N);
    vector <int> b(N);
    vector <int> nul(N);

    int T = 1;

    // выделяем память
    D.resize(N);
    B.resize(N);
    double step = 0;

    // выделяем память
    for (int i = 0; i < N; i++)
    {
        D[i].resize(N);
        B[i].resize(N);
    }

    // создаем матрицу расстояний
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            D[i][j] = distance(field.p(i), field.p(j));
            step += D[i][j]/2;
        }
    }

    // считаем порог
    step = step/(2*N*(N - 1));

    // создаем матрицу графа
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if(D[i][j] < step)
                B[i][j] = 1;
            else B[i][j] = 0;
        }
    }

    // освобождаем память
    D.clear();

    // создаем листы: index - точки, заженные в момент T, indexnew - все соседи этих заженных точек
    list <int> index;
    list <int> indexnew;

    // создаем векторы: temp - просто для обмена значений, blong - вектор заженных и не заженных точек
    vector <int> temp(N);
    vector <int> blong(N);

    // зажигаем точку
    for (int k = 0; k < N; k++)
    {
        if (!blong[k])
        {
            // создаем кластер и добавляем в него точку, если это не ядро, то пропускаем
            if (!iscore(B, k, m)) continue;
            Cluster cl;
            cl.reserve(N);
            blong[k] = 1;
            a[k] = 1;
            cl.add_p(k);

            // создаем лист, хранящий точки компоненты графа
            b = a;
            index.push_back(k);

            // зажигаем по циклу остальные точки
            while(1)
            {
                for(list <int> :: iterator it = index.begin(); it != index.end(); it++)
                {
                    // проверяем, является ли точка ядром, если да, то в список обхода вписываем ее соседей
                    if(!iscore(B, *it, m)) continue;

                    for(int i = 0 ; i < N; i++)
                    {
                        // проверяем соединены ли точки ребром и загорелись ли они
                        if(((B[*it][i]) == 1) && (a[i] == 0) && (blong[i] == 0))
                        {
                            // зажигаем точку
                            b[i] = T + 1;
                            // добавляем в кластер
                            cl.add_p(i);
                            // считаем, что по ней уже прошли
                            blong[i] = 1;
                            // отправляем точку в компоненту графа (это временный список)
                            indexnew.push_back(i);
                        }
                    }
                }

                // здесь точки связной компоненты сохраняются в список index
                index.clear();
                index.merge(indexnew);
                indexnew.clear();

                if (a == b) break;
                temp = a;
                a = b;
                T = T + 1;
            }
            clusters.push_back(cl);
        }
        // обновляем параметры T, a, b
        T = 1;
        a = nul;
        b = nul;
    }
    // создаем кластер для точек, не вошедших в другие кластеры
    Cluster cl;
    cl.reserve(N);

    // записываем в кластер
    for (int i = 0; i < N; i++)
    {
        if (!blong[i]) cl.add_p(i);
    }

    // кластер записываем в результат работы алгоритма
    clusters.push_back(cl);
    sv.save(clusters, 3);

    cout << "DBscan has perfectly done\n" << endl;
}

// Алгоритм k_means, входные параметры - k - количество кластеров
void Controller::k_means(int k)
{
    // тут кластеры
    // берем точку, затем сравниваем с каждым центром
    // кидаем ее в группу центра, где расстояние минимальное
    // делаем так со всеми точками
    // пересчитываем центры
    // идем заново, сравниваем предыдущее состояние с нынешним

    vector <Point> points = field.p();
    int N = field.size();
    vector <vector <short int>> vec(k);
    vector <Cluster> clusters(k);

    // создаем кластеры и добавляем в них по одной точке
    for (int i = 0; i < k; i++)
    {
        // создаем вектор на N элементов
        vec[i].resize(N);
        // создаем вектор в кластере на N элементов
        clusters[i].reserve(N);
        // добавляем точку
        clusters[i].add_p(i);
        // считаем центр
        clusters[i].ccenter(points, N);
    }

    while(1)
    {
        // устанавливаем флажок в 0

        bool flag = 0;
        for (int i = 0; i < N; i++)
        {
            // ищем минимальное расстояние
            int indexmin = 0;
            double min = distance(points[i], *(clusters[0].getcen()));

            // фиксируем нынешнее состояние
            vec[0] = clusters[0].p();

            for (int j = 1; j < k; j++)
            {
                // добавялем точки
                vec[j] = clusters[j].p();
                if (min > distance(points[i], *(clusters[j].getcen())))
                {
                    min = distance(points[i], *(clusters[j].getcen()));
                    indexmin = j;
                }
            }

            // убираем точки, которые в кластер не попадают
            for (int j = 0; j < k; j++)
            {
                clusters[j].rem_p(i);
            }

            // добавляем только в тот кластер, которому эта точка принадлежит
            clusters[indexmin].add_p(i);

            // если изменение есть, то фиксируем флажок в 1
            for (int m = 0; m < k; m++)
                if (vec[m] != clusters[m].p()) flag = 1;
        }
        // пересчитываем центры
        for (int i = 0; i < k; i++)
            clusters[i].ccenter(points, N);

        // проверяем флажок
        if (flag == 0) break;
    }
    cout << "K means algorithm has perfectly done" << endl;

    // сохраняем результат
    sv.save(clusters, 1);
}
