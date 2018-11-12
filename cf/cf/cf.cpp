#include<iostream>
#include<algorithm>
using namespace std;
int main() {
	int a[3];
	for (int i = 0; i < 3; i++)
		cin >> a[i];
	sort(a, a + 3);
	if (a[0] + a[1] <= a[2])
		cout << a[2] - a[1] - a[0] + 1 << endl;
	else
		cout << "0" << endl;
	return 0;
}