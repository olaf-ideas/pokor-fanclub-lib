#include "../math/factor.hpp"

bool is_prime_slow(ULL n) {
	if (n < 2) {
		return false;
	}
	for (ULL i = 2; i * i <= n; i++) {
		if (n % i == 0) {
			return false;
		}
	}
	return true;
}

vector<ULL> factor_slow(ULL n) {
	vector<ULL> f;
	for (ULL i = 2; i * i <= n; i++) {
		while (n % i == 0) {
			f.push_back(i);
			n /= i;
		}
	}
	if (n > 1) {
		f.push_back(n);
	}
	return f;
}

mt19937 rng(2137);

ULL randint(ULL a, ULL b) {
	return uniform_int_distribution<ULL>(a, b)(rng);
}

int main() {

	cerr << "testing is_prime...\n";

	for (int rep = 0; rep < 20'000; rep++) {
		ULL n = rep + 1;
		assert(is_prime(n) == is_prime_slow(n));
	}

	for (int rep = 0; rep < 20'000; rep++) {
		ULL n = randint(1, 1'000'000'000);
		assert(is_prime(n) == is_prime_slow(n));
	}

	cerr << "testing factor...\n";

	for (int rep = 0; rep < 20'000; rep++) {
		ULL n = randint(1, 1'000'000'000);

		vector<ULL> f1 = factor(n);
		vector<ULL> f2 = factor_slow(n);

		sort(f1.begin(), f1.end());
		sort(f2.begin(), f2.end());

		assert(f1 == f2);
	}

	vector<ULL> to_test;
	for (int rep = 0; rep < 20'000; rep++) {
		to_test.push_back(randint(1, 2e18L));
	}

	for (int rep = 0; rep < 20'000; rep++) {
		to_test.push_back(rep + 1);
	}

	for (ULL n : to_test) {
		vector<ULL> f = factor(n);

		ULL m = 1;
		for (ULL x : f) {
			m *= x;
		}

		assert(n == m);
	}

	cerr << "SUCCESS\n";

	return 0;
}
