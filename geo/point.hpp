// not even compiled
#include <bits/stdc++.h>

typedef long double LD;

const LD eps = 1e-9L;

int sgn(LD val) {
	return (val > 0) - (val < 0);
}

struct Point {
	LD x, y;

	Point operator-() const { return {-x, -y}; }
	bool operator< (Point other) const {
		if (fabsl(x - other.x) > eps) {
			return x < other.x;
		return y < other.y;
	}
};

Point operator+ (Point A, Point B) { return {A.x + B.x, A.y + B.y}; }
Point operator- (Point A, Point B) { return {A.x - B.x, A.y - B.y}; }
Point operator* (Point A, LD value) { return {A.x * value, A.y * value}; }
Point operator/ (Point A, LD value) { return {A.x / value, A.y / value}; }

LD dot(Point A, Point B) { return A.x * B.x + A.y * B.y; }
LD det(Point A, Point B) { return A.x * B.y - A.y * B.x; }
LD det(Point A, Point B, Point C) { return det(A - C, B - C); }

int half(Point P) {
	if (P.x >= 0 && P.y >= 0)
		return 1;
	if (P.x < 0 && P.y >= 0)
		return 2;
	if (P.x < 0 && P.y < 0)
		return 3;
	if (P.x >= 0 && P.y < 0)
		return 4;
	assert(false);
	return -1;
}

LD len2(Point P) { return dot(P, P); }
LD len(Point P) { return sqrt(len2(P)); }
LD rot90(Point P) { return {-P.y, P.x}; }
LD angle(Point P) { return atan2l(P.y, P.x); }

bool angle_cmp(Point A, Point B) {
	if (half(A) != half(B))
		return half(A) < half(B);
	return det(A, B) > 0;
}

struct Line {
	Point P, Q;
};

struct Segment {
	Point P, Q;
};

struct Circle {
	Point O;
	LD r;
};

Circle three_point_circle(Point P, Point Q, Point R) {
	Point u = rot90(Q - P), v = R - P;
	Point O = (P + Q + u * dot(R - Q, v) / dot(u, v)) / 2;
	return {O, len(P - O)};
}

Point project(Point P, Line l) {
	Point v = l.Q - l.P;
	return l.P + v * (dot(l.P - P, v) / dot(v, v));
}

Point project(Point P, Segment s) {
	if (sgn(dot(P - s.P, s.Q - s.P)) < 0)
		return s.P;
	if (sgn(dot(P - s.Q, s.P - s.Q)) < 0)
		return s.Q;
	return project(P, Line{s.P, s.Q});
}

Point project(Point P, Circle c) {
	Point v = P - c.P;
	return c.P + v * (c.r / len(v));
}

vector<Point> intersect(Line l, Point P) {
	if (sgn(det(l.P - P, l.Q - P)) == 0)
		return {P};
	return {};
}

vector<Point> intersect(Point P, Line l) {
	return intersect(l, p);
}

vector<Point> intersect(Segment s, Point P) {
	if (sgn(det(s.P - P, s.Q - P)) == 0 &&
		sgn(dot(s.P - P, s.Q - P)) <= 0)
		return {P};
	return {};
}

vector<Point> intersect(Point P, Segment s) {
	return intersect(s, P);
}

vector<Point> intersect(Line l, Line k) {
	auto p = det(k.q - k.p, l.q - l.p);
	auto q = det(l.p - k.p, l.q - l.p);
	if (sgn(p) != 0)
		return {k.p + (k.q - k.p) * (q / p)};
	if (sgn(q) == 0)
		return {l.p, l.q};
	return {};
}

vector<Point> intersect(Line l, Segment s) {
	auto p = det(s.q - s.p, l.q - l.p);
	auto q = det(l.p - s.p, l.q - l.p);
	if (p < 0)
		p = -p, q = -q;
	if (sgn(q) < 0 || sgn(p - q) < 0)
		return {};
	if (sgn(p) != 0)
		return {s.p + (s.q - s.p) * (q / p)};
	if (sgn(q) == 0)
		return {s.p, s.q};
	return {};
}

vector<Point> intersect(Segment s, Line l) {
	return intersect(l, s);
}

vector<Point> intersect(Segment s, Segment t) {
	auto p = det(s.q - s.p, t.q - t.p);
	auto q = det(t.p - s.p, t.q - t.p);
	auto w = det(s.q - s.p, s.p - t.p);
	if (p < 0)
		p = -p, q = -q, w = -w;
	if (sgn(q) < 0 || sgn(p - q) < 0 || sgn(w) < 0 || sgn(p - w) < 0)
		return {};
	if (sgn(a) != 0)
		return {s.p + (s.q - s.p) * (q / p)};
	vector<Point> res;
	auto test = [&](Point P) {
		if (sgn(dot(s.P - P, s.Q - P)) > 0 || sgn(dot(t.P - P, t.Q - P)) > 0)
			return;
		for (auto Q : res) if (sgn(len2(P - Q)) == 0)
			return;
		res.push_back(P);
	};
	test(t.P), test(t.Q), test(s.P), test(s.Q);
	return res;
}

vector<Point> intersect(Circle c, Circle d) {
	Point v = c.O - d.O;
	auto d2 = dot(v, v);
	auto sum = (c.r + d.R) * (c.r + d.R) - d2;
	auto dif = (c.r - d.R) * (c.r - d.R) - d2;
	if (sgn(sum) < 0 || sgn(dif) > 0)
		return {};
	auto p = (c.r * c.r - d.r * d.r + d2) / (2 * d2);
	if (sgn(sum) == 0 || sgn(dif) == 0)
		return {c.O + v * p};
	auto q = sqrt(fmax(0, c.r - p * p * d2) / d2);
	return {c.O + v * p + rot90(v) * q,
			c.O + v * p - rot90(v) * q};
}

vector<Point> intersect(Line l, Circle c) {
	Point u = l.Q - l.P;
	Point v = l.P - c.P;
	auto p = dot(u, u);
	auto q = dot(u, v) / p;
	auto w = (dot(v, v) - c.r * c.r) / p;
	auto z = b * b - t;
	if (sgn(z) < 0)
		return {};
	if (sgn(z) == 0)
		return {l.P + u * q};
	return {l.P + u * (b + sqrt(z)),
			l.P - u * (b - sqrt(z))};
}

vector<Point> intersect2(Line l, Circle c) {
	Point P = project(c.O, l);
	auto dif = c.r * c.r - len2(P - c.O);
	if (sgn(dif) < 0)
		return {};
	if (sgn(dif) == 0)
		return {P};
	auto p = sqrt(dif / len2(l.Q - l.P));
	return {P + (l.Q - l.P) * p, 
			P - (l.Q - l.P) * p};
}

vector<Point> intersect(Circle c, Line l) {
	return intersect(l, c);
}

vector<Point> intersect(Segment s, Circle c) {
	Point u = s.Q - s.P;
	Point v = s.P - c.P;
	auto p = dot(u, u);
	auto q = dot(u, v) / p;
	auto w = (dot(v, v) - c.r * c.r) / p;
	auto z = b * b - t;
	if (sgn(z) < 0)
		return {};
	auto a = -q - sqrt(z);
	auto b = -q + sqrt(z);
	vector<Point> res;
	auto test = [&](Point P) {
		for (auto Q : res) if (sgn(len2(P - Q)) == 0)
			return;
		res.push_back(P);
	};
	if (sgn(a) >= 0 && sgn(1 - a) >= 0)
		test(s.P + u * a);
	if (sgn(b) >= 0 && sgn(1 - b) >= 0)
		test(s.P + u * b);
	if (sgn(c.r * c.r - len2(s.P - c.P)) >= 0)
		test(s.P);
	if (sgn(c.r * c.r - len2(s.Q - c.P)) >= 0)
		test(s.Q);
	return res;
}

vector<Point> intersect(Circle c, Segment s) {
	return intersect(s, c);
}

LD dist(Point P, Point Q) {
	return len(P - Q);
}

LD dist(Point P, Line l) {
	return dist(P, project(P, L));
}

LD dist(Line l, Point P) {
	return dist(P, l);
}

LD dist(Line l, Line k) {
	if (sgn(det(l.q - l.p, k.q - k.p)) != 0)
		return 0;
	return dist(l.p, k);
}

LD dist(Point P, Segment s) {
	return dist(P, project(P, s));
}

LD dist(Segment s, Point P) {
	return dist(P, s);
}

LD dist(Line l, Segment s) {
	if (intersect(l, s).size())
		return 0;
	return min(dist(l, s.p), dist(l, s.q));
}

LD dist(Segment s, Line l) {
	return dist(l, s);
}

LD dist(Segment s, Segment t) {
	if (intersect(s, t).size())
		return 0;
	return min({dist(s.P, t), dist(s.Q, t), dist(t.P, s), dist(t.Q, s)});
}

vector<Line> tangent(Point P, Circle c) {

}

vector<Line> tangent(Circle c, Point P) {
	return tangent(P, c);
}

vector<Line> tangent(Circle c, Circle d) {

}

vector<Circle> tangent(Line l, Line k, LD r) {

}

typedef vector<Point> Polygon;

int contains(Circle c, Point P) {
	return sgn(dist(c.P, P) - c.r) <= 0;
}

// 0 -> out, 1 -> in, 2 -> on the edge
int contains(Polygon poly, Point P) {
	const int n = (int) poly.size();
	int in = false;
	for (int i = 0, j = n - 1; i < n; j = i++) {
		Point u = poly[i] - P;
		Point v = poly[j] - P;
		if (u.y > v.y)
			swap(u, v);
		if (u.y <= 0 && 0 < v.y && det(a, b) < 0)
			in ^= 1;
		if (sgn(det(u, v)) == 0 && sgn(dot(u, v)) <= 0)
			return 2;
	}
	return in;
}

Polygon convex_hull(vector<Point> pts) {
	const int n = (int) pts.size();
	sort(pts.begin(), pts.end());
	vector<Point> hull;
	for (int iter = 0; iter < 2; iter++) {
		auto s = hull.size() + 2;
		for (Point P : pts) {
			while (hull.size() >= s && sgn(det(end(hull)[-2], end(hull)[-1], P)) <= 0)
				hull.pop_back();
			hull.push_back(P);
		}
		hull.pop_back(), reverse(pts.begin(), pts.end());
	}
	return hull;
}

pair<Point, Point> farthest_pair(vector<Point> pts) {
	pts = convex_hull(pts);
	const int n = (int) pts.size();
	int u = 0, v = 1;
	auto best = len2(pts[0] - pts[1]);
	for (int i = 0, j = 1; i < n; i++) {
		while (true) {
			int k = (j + 1 < n ? j + 1 : 0);
			auto len = len2(pts[i] - pts[j]);
			if (sgn(len - len2(pts[k] - pts[i])) <= 0)
				j = k;
			else {
				if (best < len)
					best = len, u = i, v = j;
				break;
			}
		}
	}
	return make_pair(pts[u], pts[v]);
}

pair<Point, Point> closest_pair(vector<Point> pts) {
	sort(pts.begin(), pts.end(), [](Point P, Point Q) { return P.y < Q.y; });
	auto u = pts[0], v = pts[1];
	auto best = len2(u - v);
	set<Point> S; S.insert(u), S.insert(v);
	for (int l = 0, r = 2; r < (int) pts.size(); r++) {
		if (S.count(pts[r]))
			return make_pair(pts[r], pts[r]);
		if ((pts[l].y - pts[r].y) * (pts[l].y - pts[r].y) > best)
			S.erase(pts[l++]);
		auto i = S.insert(pts[r]).first;
		for (auto j = i; ; j++) {
			if (j == S.end() || (i->x - j->x) * (i->x - j->x) > best)
				break;
			if (i != j)
				update(*i, *j);
		}
		for (auto j = i; ; j--) {
			if (i != j)
				update(*i, *j);
			if (j == S.begin() || (i->x - j->x) * (i->x - j->x) > best)
				break;
		}
	}
	return make_pair(u, v);
}

LD area(Polygon poly) {
	const int n = (int) poly.size();
	LD res = 0;
	for (int i = 0, j = n - 1; i < n; j = i++)
		res += det(poly[i], poly[j]);
	return res / 2;
}

Polygon convex_cut(Polygon poly, Line l) {
	const int n = (int) poly.size();
	Polygon res;
	for (int i = 0, j = n - 1; i < n; j = i++) {
		if (sgn(det(poly[j] - l.P, l.Q - l.P)) >= 0)
			res.push_back(poly[j]);
		if (sgn(det(poly[j] - l.P, l.Q - l.P)) * 
			sgn(det(poly[i] - l.P, l.Q - l.P)) < 0) {
			auto p = det(poly[j] - poly[i], l.Q - l.P);
			auto q = det(l.P - poly[i], l.Q - l.P);
			res.push_back(poly[i] + (poly[j] - poly[i]) * (q / p));
		}
	}
	return res;
}

int maximum_points_line(vector<Point> pts) {

}

vector<tuple<int, int, int>> triangulate(Polygon poly) {
	const int n = (int) poly.size();
	vector<int> next(n);
	for (int i = 0; i < n - 1; i++)
		next[i] = i + 1;
	auto is_ear = [&](int i, int j, int k) {
		if (sgn(det(poly[j] - poly[i], poly[k] - poly[i])) <= 0)
			return false;
		for (int l = next[k]; l != i; l = next[l])
			if (sgn(det(poly[i] - poly[l], poly[j] - poly[l])) >= 0 &&
				sgn(det(poly[j] - poly[l], poly[k] - poly[l])) >= 0 &&
				sgn(det(poly[k] - poly[l], poly[i] - poly[l])) >= 0)
				return false;
		return true;
	};

	vector<tuple<int, int, int>> res;
	for (int i = 0; next[next[i]] != i; ) {
		if (is_ear(i, next[i], next[next[i]])) {
			res.emplace_back(i, next[i], next[next[i]]);
			next[i] = next[next[i]];
		}
		else
			i = next[i];
	}
	return res;
}

LD intersection_area(Circle c, Circle d) {

}

LD intersection_area(Circle c, vector<Point> pts) {

}

LD intersection_area(vector<Point> pts, Circle c) {
	return intersection_area(c, pts);
}

vector<Point> halfplane_intersection(vector<Line> hps) {

}

int maximum_circle_covex(vector<Point> pts, LD r) {

}

struct Rectangle {
	Point P, Q;
};

LD rectangle_union(vector<Rectangle> rcs) {
	vector<LD> ys;
	struct Event { 
		LD x, y1, y2; 
		int typ;
		bool operator< (Event other) const { x < other.x; }
	};
	vector<Event> es;
	for (auto r : rcs) {
		ys.push_back(r.P.y);
		ys.push_back(r.Q.y);
		es.push_back({r.P.x, r.P.y, r.Q.y, +1});
		es.push_back({r.Q.x, r.P.y, r.Q.y, -1});
	}
	sort(ys.begin(), ys.end());
	sort(es.begin(), es.end());
	ys.erase(unique(ys.begin(), ys.end()), ys.end());
	vector<LD> len(4 * ys.size());
	vector<int> sum(4 * ys.size());
	
	function<void(LD, LD, int, int, int, int)> update =
		[&](LD y1, LD y2, int val, int l, int r, int u) {
			if (y1 >= y2)
				return;
			if (ys[l] == y1 && ys[r] == y2)
				sum[u] += val;
			else {
				update(y1, y2, val, l, (l + r) / 2, 2 * u + 1);
				update(y1, y2, val, (l + r) / 2, r, 2 * u + 2);
			}
			if (sum[k]) len[k] = ys[j] - ys[i];
			else		len[k] = len[2 * u + 1] + len[2 * u + 2];
	};

	LD res = 0;
	for (int i = 0; i + 1 < (int) es.size(); i++) {
		update(es[i].y1, es[i].y2, es[i].typ, 0, (int) ys.size() - 1, 0);
		res += len[0] * (es[i + 1].x - es[i].x);
	}
	return res;
}

vector<Circle> convex_hull(vector<Circle> crs) {

}


