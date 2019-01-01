#include <algorithm>
#include <iostream>
using namespace std;
int qmi(int m, int k, int p)
{
    int res = 1, t = m;
    while (k) {
        if (k & 1) {
            res = res * t % p;
        }
        k >>= 1;
        t = t * t % p;
    }
    return res;
}
int main()
{
    int m, k, p;
    cin >> m >> k >> p;
    cout << qmi(m, k, p);
    return 0;
}