// https://sotanishy.github.io/cp-library-cpp/graph/manhattan_mst.hpp.html
// https://judge.yosupo.jp/submission/167321
#include <bits/stdc++.h>
using namespace std;
#define all(x) x.begin(), x.end()
typedef long long ll;
typedef pair<int, int> pi;
#define x first
#define y second
typedef tuple<int, int, int> ti3;
struct UF {
	int n;
	vector<int> t, r;
	UF(int _n) : n{ _n } {
		t.resize(n); iota(all(t), 0);
		r.resize(n);
	}
	int f(int x) { return t[x] == x ? x : (t[x] = f(t[x])); }
	bool merge(int a, int b) {
		a = f(a), b = f(b);
		if (a == b) return false;
		if (r[a] < r[b]) swap(a, b);
		t[b] = a; 
		r[b] += r[a] == r[b];
		return true;
	}
};
void manhattan_mst(vector<pi>& pts) {
	int n = pts.size();
	vector<int> idx(n); iota(all(idx), 0);
	vector<ti3> edges; edges.reserve(4 * n);

	for (int i = 0; i < 2; i++) { 
		for(auto&& [x,y]:pts) x=-x;
		for (int j = 0; j < 2; j++) { 
			for(auto&& [x, y] : pts) swap(x,y);
			sort(all(idx), [&](int a, int b)->bool {
				return pts[a].x + pts[a].y < pts[b].x + pts[b].y; 
				});

			map<int, int> mp;
			for (int id : idx) {
				auto&& [x, y] = pts[id];
				for (auto itr = mp.lower_bound(-y); itr != mp.end(); itr = mp.erase(itr)) {
					int to = itr->second;
					auto&& [x0, y0] = pts[to];
					int dx = x - x0, dy = y - y0;
					if (dy > dx) break;
					edges.push_back({ dx + dy, id, to });
				}
				mp[-y] = id;
			}
		}
	}
	sort(all(edges));
	ll ans = 0;
	UF uf(n);
	vector<pi> ret;
	for (auto& [c, u, v] : edges) {
		if (uf.merge(u, v)) {
			ans += c;
			ret.push_back(pi(u, v));
		}
	}
	cout << ans << "\n";
	for (auto& [u, v] : ret)
		cout << u << " " << v << "\n";
}
int main() {
	ios::sync_with_stdio(0);
	cin.tie(0), cout.tie(0);
	int n; cin >> n;
	vector<pi> arr(n);
	for (auto& [x, y] : arr) cin >> x >> y;
	manhattan_mst(arr);
	return 0;
}
