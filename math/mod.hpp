#include "../template.hpp"

const int mod = 1e9 + 7;

void add(int &a, int b) {
	a += b;
	if (a >= mod) {
		a -= mod;
	}
}

void sub(int &a, int b) {
	a -= b;
	if (a < 0) {
		a += mod;
	}
}

int mul(int a, int b) {
	return (long long) a * b % mod;
}

int power(int a, int b) {
	int r = 1;
	for (; b; b >>= 1, a = mul(a, a)) {
		if (b & 1) {
			r = mul(r, a);
		}
	}
	return r;
}

int inv(int a) {
	a %= mod;
	if (a < 0) {
		a += mod;
	}
	int b = mod, u = 0, v = 1;
	while (a) {
		int t = b / a;
		b -= t * a;
		swap(a, b);
		u -= t * v;
		swap(u, v);
	}
	if (u < 0) {
		u += mod;
	}
	return u;
}

const int N = 1'000'000;

int fac[N], ifac[N];
int inv_fast[N];

void init() {
	fac[0] = 1;
	for (int i = 1; i < N; i++) {
		fac[i] = mul(fac[i - 1], i);
	}
	
	ifac[N - 1] = inv(fac[N - 1]);
	for (int i = N - 2; i >= 1; i--) {
		ifac[i] = mul(ifac[i + 1], i + 1);
	}

	inv_fast[1] = 1;
	for (int i = 2; i < N; i++) {
		sub(inv_fast[i], mul(mod / i, inv_fast[mod % i]));
	}
}

int C(int n, int k) {
	if (0 <= k && k <= n)
		return mul(fac[n], mul(ifac[k], ifac[n - k]));
	return 0;
}
