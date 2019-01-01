#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
int main()
{
	int x;
	cin >> x;
	for (int t = 1; t <= x; t++) {
		int n;
		cin >> n;
		int sha[2] = { 0 }, shan[2] = { 0 }, tao[2] = { 0 }, sm[2] = { 1, 1 };
		string name[2] = { "ZSGG", "EAGLE" };
		string s;
		for (int i = 0; i < n; i++) {
			cin >> s;
			if (s[0] == 'K')
				sha[0]++;
			else if (s[0] == 'F')
				shan[0]++;
			else if (s[0] == 'P')
				tao[0]++;
		}
		for (int i = 0; i < n; i++) {
			cin >> s;
			if (s[0] == 'K')
				sha[1]++;
			else if (s[0] == 'F')
				shan[1]++;
			else if (s[0] == 'P')
				tao[1]++;
		}
		string round = to_string(t);
		if (round.back() == '1' && round != "11" && round != "111") {
			round += "st";
		}
		else if (round.back() == '2'&& round != "12"&& round != "112") {
			round += "nd";
		}
		else if (round.back() == '3'&& round != "13"&& round != "113") {
			round += "rd";
		}
		else {
			round += "th";
		}
		int xs = 0, hs = 1;
		if (t % 2 == 0) {
			swap(xs, hs);
		}
		string winner;
		for (int i = 0; i < n; i++) {
			if (sha[xs] > 0) {
				sha[xs]--;
				if (shan[hs] > 0) {
					shan[hs]--;
				}
				else if (tao[hs] > 0) {
					tao[hs]--;
				}
				else {
					sm[hs]--;
				}
			}
			else {
				if (tao[xs] > 0) {
					tao[xs]--;
					sm[xs]++;
				}
				else if (shan[xs] > 0) {
					shan[xs]--;
				}
				if (sha[hs] > 0) {
					sha[hs]--;
					if (shan[xs] > 0) {
						shan[xs]--;
					}
					else if (tao[xs] > 0) {
						tao[xs]--;
					}
					else {
						sm[xs]--;
					}
				}
				else {
					if (tao[hs] > 0) {
						tao[hs]--;
						sm[hs]++;
					}
					else if (shan[hs] > 0) {
						shan[hs]--;
					}
				}
			}
			if (sm[hs] == 0) {
				winner = name[xs];
				break;
			}
			else if (sm[xs] == 0) {
				winner = name[hs];
				break;
			}
		}
		if (winner.empty()) {
			sm[xs] += tao[xs];
			sm[hs] += tao[hs];
			if (sm[xs] > sm[hs]) {
				winner = name[xs];
			}
			else if (sm[xs] < sm[hs]) {
				winner = name[hs];
			}
			else {
				winner = "Both";
			}
		}
		cout << "The " << round << " battle's winner is " << winner << "." << endl;
	}
	return 0;
}