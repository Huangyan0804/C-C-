#include<iostream>
#include<algorithm>
using namespace std;
int i(0);
void quicksort(int a[], int start, int end) {
	if (start >= end)
		return;
	int left = start, right = end;
	int key = a[right];
	while (left < right) {
		while (left < right&&a[left] <= key) {
			left++;
		}
		a[right] = a[left];
		while (left < right&&a[right] >= key) {
			right--;
		}
		a[left] = a[right];
	}
	a[right] = key;
	quicksort(a, start, left - 1);
	quicksort(a, left + 1, end);
}
int main() {
	int a[14] = { 3,54,43,2,6,2,7,12,5,7,4,6,4,7 };
	quicksort(a, 0, 13);
	for (int i = 0; i <= 13; i++)
		cout << a[i] << ' ';
	return 0;
}
