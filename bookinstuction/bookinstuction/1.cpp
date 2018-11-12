#include<iostream>
#include<math.h>

using namespace std;
int ans = 1;
void jc(int b) {
	
	if (b == 1) return;
	ans *= b;
	b--;
	jc(b);
}
int main() {
	jc(5);
	cout << ans;
	system("pause");
	return 0;
}