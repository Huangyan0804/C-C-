#include<iostream>
#include<algorithm>
using namespace std;
int main() {
	int t;
	cin >> t;
	while (t--) {
		int a; int ans = 0;
		cin >> a;
		for (int i = 0; i <= a; i++) {
			if (a - (a^i) - i == 0)
				ans++;
		}
		cout << ans << endl;
	}
	return 0;
}