#include "../geo/line_container.hpp"

typedef long long LL;

int main() {

	mt19937 rng(2137);

	auto randint = [&](LL a, LL b) {
		return uniform_int_distribution<LL>(a, b)(rng);
	};

	rep(t, 10) {
		vector<pair<int, int>> lines;
		LineContainer lc;

		rep(i, 10000) {
			int a = randint(-1000, +1000);
			int b = randint(-1000, +1000);

			lines.emplace_back(a, b);

			lc.add(a, b);
		}

		rep(i, 10000) {
			int x = randint(-1000, +1000);
			int y = -1e9;

			for (auto [a, b] : lines) {
				y = max(y, a * x + b);
			}

			LD yy = lc.query(x);
			cerr << "x: " << x << '\n';
			cerr << y << ' ' << yy << '\n';
			assert(y == lc.query(x));
		}
	}

	rep(t, 100) {
		vector<pair<int, int>> lines;
		LineContainer lc;
		rep(i, 10000) {
			int a = randint(-1e9, +1e9);
			int b = randint(-1e9, +1e9);

			lines.emplace_back(a, b);
			lc.add(a, b);
		}

		rep(i, 10000) {
			LL x = randint(-1e9, +1e9);
			LL y = -2e18L;

			for (auto [a, b] : lines) {
				y = max(y, a * x + b);
			}

			LL yy = lc.query(x);
			cerr << "x: " << x << '\n';
			cerr << y << ' ' << yy << '\n';
			assert(y == yy);
		}
	}

	return 0;
}
