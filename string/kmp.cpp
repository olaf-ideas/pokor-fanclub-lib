#include "../template.hpp"

vector<int> kmp(const string& s) {
	vector<int> p(s.size());
	for (int i = 1, d = 0; i < (int) s.size(); i++) {
		while (d > 0 && s[d] != s[i]) {
			d = p[d - 1];
		}
		if (s[d] == s[i]) {
			d++;
		}
		p[i] = d;
	}
	return p;
}
