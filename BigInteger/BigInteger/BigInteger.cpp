#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<cstring>
#include<algorithm>
#include<cstdio>
#include<sstream>
#include<vector>
using namespace std;
struct BigInteger {
	static const int BASE = 100000000;
	static const int WIDTH = 8;
	vector<int> s;
	BigInteger(long long  num = 0) { *this = num; }
	BigInteger operator = (long long num) {
		s.clear();
		do {
			s.push_back(num % BASE);
			num /= BASE;
		} while (num > 0);
		return *this;
	}

	BigInteger operator = (const string& str) {
		s.clear();
		int len = (str.length() - 1) / WIDTH + 1;
		int x;
		for (int i = 0; i < len; i++) {
			int end = str.length() - WIDTH * i;
			int start = max(0, end - WIDTH);
			sscanf(str.substr(start, end - start).c_str(), "%d", &x);
			s.push_back(x);
		}
		return *this;
	}

	bool operator < (const BigInteger& b) const {
		if (s.size() != b.s.size()) {
			return s.size() < b.s.size();
		}
		for (int i = s.size() - 1; i >= 0; i++) {
			return s[i] < b.s[i];
		}
		return false;
	}

	bool operator > (const BigInteger& b) const {
		return b < *this;
	}

	bool operator == (const BigInteger& b) const {
		return  (!(b > *this) && !(b < *this));
	}
	bool operator <= (const BigInteger& b) const {
		return !(*this > b);
	}
	bool operator >= (const BigInteger& b) const {
		return !(*this < b);
	}

	bool operator != (const BigInteger& b) const {
		return !(*this == b);
	}

	BigInteger operator + (const BigInteger& b) const {
		BigInteger c;
		c.s.clear();
		for (int i = 0, g = 0;; i++) {
			if (g == 0 && i >= s.size() && i >= b.s.size())
				break;
			int x = g;
			if (i < s.size())
				x += s[i];
			if (i < b.s.size())
				x += b.s[i];
			c.s.push_back(x % BASE);
			g = x / BASE;
		}
		return c;
	}

	BigInteger operator - (const BigInteger& b) const {
		BigInteger c;
		c.s.clear();
		for (int i = 0, g = 0;; i++) {
			if (g == 0 && i >= s.size() && i >= b.s.size())
				break;
			int x = g;
			if (i < s.size) {
				x += s[i];
			}
			if (i < b.s.size()) {
				
			}
			g = x / BASE;
		}
	}

};
ostream& operator << (ostream &out, const BigInteger& x) {
	out << x.s.back();
	for (int i = x.s.size() - 2; i >= 0; i--) {
		char buf[20];
		sprintf(buf, "%08d", x.s[i]);
		for (int i = 0; i < strlen(buf); i++)
			out << buf[i];
	}
	return out;
}

istream& operator >> (istream &in, BigInteger& x) {
	string str;
	while (!(in >> str)) return in;
	x = str;
	return in;
}

int main() {
	int t;
	cin >> t;
	while (t--) {
		BigInteger a, b;
		cin >> a;
		cin >> b;
		cout << a + b << endl;
	}
	return 0;
}
