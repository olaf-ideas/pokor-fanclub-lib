#include "../template.hpp"

typedef unsigned long long ULL;

ULL mul(ULL a, ULL b, ULL n) {
	return (__uint128_t) a * b % n;
}

ULL power(ULL a, ULL b, ULL n) {
	ULL r = 1;
	for (; b; b >>= 1, a = mul(a, a, n)) {
		if (b & 1) {
			r = mul(r, a, n);
		}
	}
	return r;
}

bool is_prime(ULL n) {
	if (n < 2 || (n > 2 && n % 2 == 0)) {
		return false;
	}
	auto suspect = [&](ULL a, ULL s, ULL d) {
		ULL x = power(a, d, n);
		if (x == 1) {
			return true;
		}
		for (int r = 0; r < s; r++) {
			if (x == n - 1) {
				return true;
			}
			x = mul(x, x, n);
		}
		return false;
	};
	
	ULL d = n - 1, s = 0;
	while (~d & 1) {
		d >>= 1;
		s++;
	}

	for (auto &&p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
		if (p >= n) {
			break;
		}

		if (!suspect(p, s, d)) {
			return false;
		}
	}
	return true;
}

ULL pollard(ULL n) {
	ULL x = 0, y = 0, t = 30, prd = 2, i = 1, q;
	auto rho = [&](ULL x) { return mul(x, x, n) + i; };
	while (t-- % 40 || gcd(prd, n) == 1) {
		if (x == y) {
			x = ++i;
			y = rho(x);
		}
		
		q = mul(prd, max(x, y) - min(x, y), n);
		if (q > 0) {
			prd = q;
		}
		
		x = rho(x);
		y = rho(rho(y));
	}
	return gcd(prd, n);
}

vector<ULL> factor(ULL n) {
	if (n == 1) {
		return {};
	}
	if (is_prime(n)) {
		return {n};
	}
	ULL d = pollard(n);
	auto l = factor(d), r = factor(n / d);
	l.insert(l.end(), r.begin(), r.end());
	return l;
}
