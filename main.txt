// 212 Клипалко Михаил Михайлович
// Генерация точек по Гауссу

#include "headers.h"
#include "interface.h"

int main()
{
    using namespace std;

    // создаем интерфейс
    Interface s;
    int a;
    int k;

    cout << "Start! Enter HELP to see the list of available commands\n";

    while (true)
    {
        // контроллер
        Controller controller;
        switch(a = s.Starts())
        {
            case 1: cout << "Enter the number of points, x-mean, y-mean, x-sigma, y-sigma: ";
                    s.controller.Create_Cloud(s.N, s.x, s.y, s.gx, s.gy);
                    s.controller.raise();
                    continue;
            case 2: cout << "What's cloud you wanted to see?" << endl;
                    cout << "Enter the number:   ";
                    cin >> k;
                    s.controller.Print_Cloud(k - 1);
                    continue;
            case 3: cout << "Let's print the field..." << endl;
                    s.controller.Print_Field();
                    continue;
            case 4: cout << "It is Wave algorithm. Let's start\n";
                    s.controller.Wave();
                    continue;
            case 5: cout << "It is DBscan algorithm. Enter the k-neighbors of the cores: ";
                    cin >> k;
                    s.controller.dbscan(k);
                    continue;
            case 6: cout << "It is K means algorithm. Enter the number of clusters: ";
                    cin >> k;
                    s.controller.k_means(k);
                    continue;
            case 7: cout << "Enter the number of algorithm (index starts with 1): ";
                    cin >> k;
                    s.controller.find_cluster(k - 1);
                    continue;
            case 8: continue;
            case 9: return 0;
            default:cout << "Unknown command. Try again!\n";
                    continue;
        }
    }
}
