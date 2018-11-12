#include<bits/stdc++.h>
using namespace std;
void quicksort(int *arr,int low,int high) {
	int i = low;
	int j = high;
	int pivot = (i + j) / 2;
	while (i <= j) {
		while (arr[i] < arr[pivot])
			i++;
		while (arr[j] > arr[pivot])
			j--;
		if (i <= j) {
			swap(arr[i], arr[j]);
			i++;
			j--;
		}

		if (j > low)
			quicksort(arr, low, j);
		if (i < high)
			quicksort(arr, i, high);
	}
}
int main() {
	int n; int arr[10];
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> arr[i];
	}
	quicksort(arr, 0, n-1);
	for (int i = 0; i < n; i++) {
		cout<< arr[i]<<' ';
	}
	system("pause");
	return 0;
}