#include <Windows.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <time.h>
#include <vector>
using namespace std;

/*
  Ð§¹ûÑÝÊ¾

      * *   * *
    *     *     *
    *           *
      *       *
        *   *
          *

    W i s h   Y o u   H a v e   A   N i c e   D a y !

      {_}
    {_}o{_}
      {_}
       |
      ~|
       |~

*/
int temp1[20] = { 2, 4, 8, 10, 14, 20, 26, 28, 40, 44, 52, 60, 64, 76 };
int temp2[20] = { 13, 27, 41, 55, 69, 77 };
int temp3[20] = { 7, 15, 23, 31, 39, 46 };
vector<int> heart(temp1, temp1 + 14);
vector<int> blank_line(temp2, temp2 + 6);
vector<int> flower(temp3, temp3 + 6);
void nextLine()
{
    Sleep(300);
    cout << endl;
}
void print_space(int x)
{
    for (int i = 0; i < x; i++)
        cout << " ";
}
void draw_heart()
{
    int space = (int)rand() % 90 + 20;
    print_space(space);
    for (int i = 0; i < 78; i++) {
        if (find(blank_line.begin(), blank_line.end(), i) != blank_line.end()) {
            nextLine();
            print_space(space);
        } else if (find(heart.begin(), heart.end(), i) != heart.end()) {
            printf("\033[40;31m*\033[0m"); // *
        } else {
            cout << " ";
        }
    }
    cout << endl;
}

void show_string()
{
    print_space(rand() % 45 + 5);
    printf("\033[40;34mW i s h   Y o u   H a v e   A   N i c e   D a y !\033[0m");
}

void draw_flower()
{
    int space = (int)rand() % 90 + 20;
    print_space(space);
    for (int i = 0; i < 47; i++) {
        if (find(flower.begin(), flower.end(), i) != flower.end()) {
            cout << endl;
            print_space(space);
        } else if (i == 2 || i == 8 || i == 12 || i == 18) {
            printf("\033[40;35m{\033[0m"); // purple {
        } else if (i == 3 || i == 9 || i == 13 || i == 19) {
            printf("\033[40;35m_\033[0m"); // purple _
        } else if (i == 4 || i == 10 || i == 14 || i == 20) {
            printf("\033[40;35m}\033[0m"); // purple }
        } else if (i == 27 || i == 35 || i == 43) {
            printf("\033[40;32m|\033[0m"); // green |
        } else if (i == 34 || i == 44) {
            printf("\033[40;32m~\033[0m"); // green ~
        } else if (i == 11) {
            printf("\033[40;33mo\033[0m"); // yellow o
        } else {
            cout << " ";
        }
    }
    cout << endl;
}

int main()
{
    srand((unsigned int)time(0));
    system("mode con cols=150 lines=60");
    system("cls");
    while (true) {
        nextLine();
        nextLine();
        draw_heart();
        nextLine();
        nextLine();
        show_string();
        nextLine();
        nextLine();
        draw_flower();
    }

    return 0;
}