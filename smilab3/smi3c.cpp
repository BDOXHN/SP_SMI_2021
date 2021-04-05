#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
    srand(time(0));
    int i1, j1;
    int mas1[5][5];
    int mas2[4][4];
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            mas1[i][j] = rand() % 9 + 1;
            cout << mas1[i][j] << " ";;
        }
        cout << "\n";
    }

    cout << "\n";
    i1 = rand() % 5; //stroka
    j1 = rand() % 5; //stolbec
    cout << "string[0-4]: " << i1 << "\n";
    cout << "collum[0-4]: " << j1 << "\n\n";

    for (int i = 0; i < 4; i++) {
        if (i >= i1) {
            for (int j = 0; j < 4; j++) {
                if (j >= j1) {
                    mas2[i][j] = mas1[i + 1][j + 1];
                    cout << mas2[i][j] << " ";
                }
                else {
                    mas2[i][j] = mas1[i + 1][j];
                    cout << mas2[i][j] << " ";
                }
            }
        }
        else {

            for (int j = 0; j < 4; j++) {
                if (j >= j1) {
                    mas2[i][j] = mas1[i][j + 1];
                    cout << mas2[i][j] << " ";
                }
                else {
                    mas2[i][j] = mas1[i][j];
                    cout << mas2[i][j] << " ";
                }
            }
        }
        cout << "\n";
    }
    cout << "\n";
    return 0;
}
