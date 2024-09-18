#include "../math/mod.hpp"

using namespace std;

mt19937 rng(2137);

int randint(int a, int b) {
	return uniform_int_distribution<int>(a, b)(rng);
}

int main() {
	ios::sync_with_stdio(false), cin.tie(nullptr);

	for (int rep = 0; rep < 10000; rep++) {
		int a = randint(1, mod - 1);

		int inv_a = inv(a);
		assert(1 <= inv_a && inv_a < mod);
		assert(mul(a, inv_a) == 1);
	}

	init();

	long long f = 1;
	for (int i = 1; i < N; i++) {
		(f *= i) %= mod;

		assert(fac[i] == f);
		assert(ifac[i] == inv(f));
	}

	for (int i = 1; i < N; i++) {
		assert(inv_fast[i] == inv(i));
	}

	cerr << "SUCCESS\n";

	return 0;
}
