#include "../template.hpp"

typedef long double LD;

struct Line {
	mutable LD a, b, p;
	LD eval(LD x) const {
		return a * x + b;
	}
	bool operator< (const Line& other) const {
		return a < other.a;
	}
	bool operator< (LD x) const {
		return p < x;
	}
};

struct LineContainer : multiset<Line, less<>> {
	const LD inf = 1 / LD(0);
	LD div(LD a, LD b) {
		return a / b;
	}
	bool intersect(iterator x, iterator y) {
		if (y == end()) {
			x->p = inf;
			return false;
		}
		if (x->a == y->a) {
			x->p = x->b > y->b ? inf : -inf;
		}
		else {
			x->p = div(y->b - x->b, x->a - y->a);
		}
		return x->p >= y->p;
	}
	void add(LD a, LD b) {
		auto z = insert({a, b, 0}), x = z++, y = x;
		while (intersect(y, z)) {
			z = erase(z);
		}
		if (x != begin() && intersect(--x, y)) {
			intersect(x, erase(y));
		}
		while ((y = x) != begin() && (--x)->p >= y->p) {
			intersect(x, erase(y));
		}
	}
	LD query(LD x) {
		assert(!empty());
		return lower_bound(x)->eval(x);
	}
};
