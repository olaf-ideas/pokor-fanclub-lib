

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <tuple>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <cmath>
#include <functional>
#include <cassert>
#include <climits>
#include <iomanip>
#include <numeric>
#include <memory>
#include <random>
#include <thread>
#include <chrono>
#define allof(obj) (obj).begin(), (obj).end()
#define range(i, l, r) for(int i=l;i<r;i++)
#define unique_elem(obj) obj.erase(std::unique(allof(obj)), obj.end())
#define bit_subset(i, S) for(int i=S, zero_cnt=0;(zero_cnt+=i==S)<2;i=(i-1)&S)
#define bit_kpop(i, n, k) for(int i=(1<<k)-1,x_bit,y_bit;i<(1<<n);x_bit=(i&-i),y_bit=i+x_bit,i=(!i?(1<<n):((i&~y_bit)/x_bit>>1)|y_bit))
#define bit_kth(i, k) ((i >> k)&1)
#define bit_highest(i) (i?63-__builtin_clzll(i):-1)
#define bit_lowest(i) (i?__builtin_ctzll(i):-1)
#define sleepms(t) std::this_thread::sleep_for(std::chrono::milliseconds(t))
using ll = long long;
using ld = long double;
using ul = uint64_t;
using pi = std::pair<int, int>;
using pl = std::pair<ll, ll>;
using namespace std;

template<typename F, typename S>
std::ostream &operator<<(std::ostream &dest, const std::pair<F, S> &p){
  dest << p.first << ' ' << p.second;
  return dest;
}
template<typename T>
std::ostream &operator<<(std::ostream &dest, const std::vector<std::vector<T>> &v){
  int sz = v.size();
  if(sz==0) return dest;
  for(int i=0;i<sz;i++){
    int m = v[i].size();
    for(int j=0;j<m;j++) dest << v[i][j] << (i!=sz-1&&j==m-1?'\n':' ');
  }
  return dest;
}
template<typename T>
std::ostream &operator<<(std::ostream &dest, const std::vector<T> &v){
  int sz = v.size();
  if(sz==0) return dest;
  for(int i=0;i<sz-1;i++) dest << v[i] << ' ';
  dest << v[sz-1];
  return dest;
}
template<typename T, size_t sz>
std::ostream &operator<<(std::ostream &dest, const std::array<T, sz> &v){
  if(sz==0) return dest;
  for(int i=0;i<sz-1;i++) dest << v[i] << ' ';
  dest << v[sz-1];
  return dest;
}
template<typename T>
std::ostream &operator<<(std::ostream &dest, const std::set<T> &v){
  for(auto itr=v.begin();itr!=v.end();){
    dest << *itr;
    itr++;
    if(itr!=v.end()) dest << ' ';
  }
  return dest;
}
template<typename T, typename E>
std::ostream &operator<<(std::ostream &dest, const std::map<T, E> &v){
  for(auto itr=v.begin();itr!=v.end();){
    dest << '(' << itr->first << ", " << itr->second << ')';
    itr++;
    if(itr!=v.end()) dest << '\n';
  }
  return dest;
}
std::ostream &operator<<(std::ostream &dest, __int128_t value) {
  std::ostream::sentry s(dest);
  if (s) {
    __uint128_t tmp = value < 0 ? -value : value;
    char buffer[128];
    char *d = std::end(buffer);
    do {
      --d;
      *d = "0123456789"[tmp % 10];
      tmp /= 10;
    } while (tmp != 0);
    if (value < 0) {
      --d;
      *d = '-';
    }
    int len = std::end(buffer) - d;
    if (dest.rdbuf()->sputn(d, len) != len) {
      dest.setstate(std::ios_base::badbit);
    }
  }
  return dest;
}
template<typename T>
vector<T> make_vec(size_t sz, T val){return std::vector<T>(sz, val);}
template<typename T, typename... Tail>
auto make_vec(size_t sz, Tail ...tail){
  return std::vector<decltype(make_vec<T>(tail...))>(sz, make_vec<T>(tail...));
}
template<typename T>
vector<T> read_vec(size_t sz){
  std::vector<T> v(sz);
  for(int i=0;i<(int)sz;i++) std::cin >> v[i];
  return v;
}
template<typename T, typename... Tail>
auto read_vec(size_t sz, Tail ...tail){
  auto v = std::vector<decltype(read_vec<T>(tail...))>(sz);
  for(int i=0;i<(int)sz;i++) v[i] = read_vec<T>(tail...);
  return v;
}
long long max(long long a, int b){return std::max(a, (long long)b);}
long long max(int a, long long b){return std::max((long long)a, b);}
long long min(long long a, int b){return std::min(a, (long long)b);}
long long min(int a, long long b){return std::min((long long)a, b);}
long long modulo(long long a, long long m){a %= m; return a < 0 ? a + m : a;}

void io_init(){
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);
}


struct Point{
  int64_t x,y;
  Point operator-(Point p)const{
    return {x-p.x,y-p.y};
  }
  int64_t cross(Point p)const{
    return x*p.y-y*p.x;
  }
  int64_t dot(Point p)const{
    return x*p.x+y*p.y;
  }
  bool operator<(Point p)const{
    if(y!=p.y) return y<p.y;
    return x<p.x;
  }
  bool operator==(Point p)const{
    return x==p.x&&y==p.y;
  }
  Point operator-()const{
    return {-x,-y};
  }
};

int64_t cross(Point a,Point b,Point c){
  return (b-a).cross(c-a);
}


class LeftHull{
  std::vector<Point> ps;
  struct Node{
    int bl,br;
    int L,R;
    int lchd,rchd;
  };
  std::vector<Node> nodes;
  int root;
  bool isleaf(int w){
    return nodes[w].lchd==-1&&nodes[w].rchd==-1;
  }
  void pull(int w){
    assert(!isleaf(w));
    int l=nodes[w].lchd,r=nodes[w].rchd;
    int64_t split_y=ps[nodes[r].L].y;
    while(!isleaf(l)||!isleaf(r)){
      int a=nodes[l].bl,b=nodes[l].br, c=nodes[r].bl,d=nodes[r].br;
      if(a!=b && cross(ps[a],ps[b],ps[c])>0){
        l=nodes[l].lchd;
      }else if(c!=d && cross(ps[b],ps[c],ps[d])>0){
        r=nodes[r].rchd;
      }else if(a==b){
        r=nodes[r].lchd;
      }else if(c==d){
        l=nodes[l].rchd;
      }else{
        int64_t s1=cross(ps[a],ps[b],ps[c]);
        int64_t s2=cross(ps[b],ps[a],ps[d]);
        assert(s1+s2>=0);
        if(s1+s2==0||s1*ps[d].y+s2*ps[c].y<split_y*(s1+s2)){
          l=nodes[l].rchd;
        }else{
          r=nodes[r].lchd;
        }
      }
    }
    nodes[w].bl=nodes[l].L;
    nodes[w].br=nodes[r].L;
  }
  void build(int w,int L,int R){
    nodes[w].L=L;
    nodes[w].R=R;
    if(R-L==1){
      nodes[w].lchd=nodes[w].rchd=-1;
      nodes[w].bl=nodes[w].br=L;
    }else{
      int M=(L+R)/2;
      nodes[w].lchd=w+1;
      nodes[w].rchd=w+2*(M-L);
      build(nodes[w].lchd,L,M);
      build(nodes[w].rchd,M,R);
      pull(w);
    }
  }
  int erase(int w,int L,int R){
    if(R<=nodes[w].L||L>=nodes[w].R) return w;
    if(L<=nodes[w].L&&R>=nodes[w].R) return -1;
    nodes[w].lchd=erase(nodes[w].lchd,L,R);
    nodes[w].rchd=erase(nodes[w].rchd,L,R);
    if(nodes[w].lchd==-1) return nodes[w].rchd;
    if(nodes[w].rchd==-1) return nodes[w].lchd;
    pull(w);
    return w;
  }
  //only works for whole hull
  void get_hull(int w,int l,int r,std::vector<int>& res){
    if(isleaf(w)){
      res.push_back(nodes[w].L);
    }else if(r<=nodes[w].bl){
      get_hull(nodes[w].lchd,l,r,res);
    }else if(l>=nodes[w].br){
      get_hull(nodes[w].rchd,l,r,res);
    }else{
      assert(l<=nodes[w].bl&&nodes[w].br<=r);
      get_hull(nodes[w].lchd,l,nodes[w].bl,res);
      get_hull(nodes[w].rchd,nodes[w].br,r,res);
    }
  }
public:
  LeftHull(const std::vector<Point>& ps):ps(ps),nodes(ps.size()*2),root(0){
    build(0,0,ps.size());
  }
  std::vector<int> get_hull(){
    if(root==-1) return {};
    std::vector<int> res;
    get_hull(root,0,ps.size()-1,res);
    return res;
  }
  void erase(int L){
    root=erase(root,L,L+1);
  }
};

std::vector<Point> ps;
std::map<Point,int> id;
int layer[1000005];
int ans[1000005];

int main(){
  io_init();
  int n;
  std::cin >> n;
  range(i, 0, n){
    int x, y;
    std::cin >> x >> y;
    ps.push_back({x, y});
    id[{x, y}]=i;
  }
  std::sort(allof(ps));
  LeftHull left(ps);
  std::reverse(allof(ps));
  for(auto& p: ps) p = -p;
  LeftHull right(ps);
  for(auto& p:ps) p = -p;
  std::reverse(allof(ps));
  for(int l=1,cnt=0;cnt<n;l++){
    std::set<int> hull;
    for(int i:left.get_hull()){
      hull.insert(i);
    }
    for(int i:right.get_hull()){
      hull.insert(n-1-i);
    }
    for(int i:hull){
      assert(!layer[i]);
      cnt++;
      layer[i]=l;
      left.erase(i);
      right.erase(n-1-i);
    }
  }
  range(i, 0, n) ans[id[ps[i]]]=layer[i];
  range(i, 0, n) std::cout << ans[i] << '\n';
}

