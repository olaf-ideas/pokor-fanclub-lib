/*
Copyright (c) 2023 Zhao Yuyang. All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Authors: Zhao Yuyang "FR"
*/
#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <numeric>
#include <cstring>

struct IO_Tp
{
	const static int _O_Buffer_Size = 10 << 20;
	char _O_Buffer[_O_Buffer_Size], *_O_pos = _O_Buffer;
	
	char m[10000][4];
	
	uint16_t pack2(char *p)
	{
		uint16_t res;
		std::copy(p, p + 2, reinterpret_cast<char *>(&res));
		return res;
	}
	
	IO_Tp()
	{
		int x = 0;
		for (char i = '0'; i <= '9'; ++i)
			for (char j = '0'; j <= '9'; ++j)
				for (char k = '0'; k <= '9'; ++k)
					for (char l = '0'; l <= '9'; ++l)
						m[x][0] = i, m[x][1] = j, m[x][2] = k, m[x][3] = l, ++x;
	}
	~IO_Tp() { fwrite(_O_Buffer, 1, _O_pos - _O_Buffer, stdout); }
	
	IO_Tp &operator<<(char ch)
	{
		*_O_pos++ = ch;
		return *this;
	}
	
	template<int n = 4>
	void print4(uint32_t x)
	{
		_O_pos = std::copy(m[x], m[x] + n, _O_pos);
	}
	
	IO_Tp &operator<<(uint32_t x)
	{
		switch (x)
		{
			default:
				print4(x / 1000000), x %= 1000000;
				[[fallthrough]];
			case 100000 ... 999999:
				print4(x / 100), x %= 100;
				[[fallthrough]];
			case 10 ... 99:
				print4<2>(x * 100);
				break;
			case 100000000 ... 999999999:
				print4(x / 100000), x %= 100000;
				[[fallthrough]];
			case 10000 ... 99999:
				print4(x / 10), x %= 10;
				[[fallthrough]];
			case 0 ... 9:
				*_O_pos++ = '0' + x;
				break;
			case 10000000 ... 99999999:
				print4(x / 10000), x %= 10000;
				[[fallthrough]];
			case 1000 ... 9999:
				print4(x);
				break;
			case 1000000 ... 9999999:
				print4(x / 1000), x %= 1000;
				[[fallthrough]];
			case 100 ... 999:
				print4<3>(x * 10);
				break;
		}
		return *this;
	}
} IO;

const int Max_N = 500005;

namespace SA_IS
{
	int *sa;
	
	template<typename _Char>
	void sais_core(const int n, const int m, const _Char s[], char type[], int lms[], int cnt[])
	{
		int n1 = 0, ch = -1;
		
		type[n - 1] = 1;
		for (int i = n - 2; i >= 0; --i)
			type[i] = s[i] == s[i + 1] ? type[i + 1] : s[i] < s[i + 1];
		
		std::fill(cnt, cnt + m, 0);
		for (int i = 0; i < n; ++i)
			++cnt[static_cast<int>(s[i])];
		std::partial_sum(cnt, cnt + m, cnt);
		
		auto induced_sort = [&](const int v[])
		{
			std::fill(sa, sa + n, 0);
			
			int *cur = cnt + m;
			auto push_S = [&](const int x) { sa[--cur[static_cast<int>(s[x])]] = x; };
			auto push_L = [&](const int x) { sa[cur[static_cast<int>(s[x])]++] = x; };
			
			std::copy(cnt, cnt + m, cur);
			for (int i = n1 - 1; i >= 0; --i)
				push_S(v[i]);
			
			std::copy(cnt, cnt + m - 1, cur + 1);
			for (int i = 0; i < n; ++i)
				if (sa[i] > 0 && type[sa[i] - 1] == 0)
					push_L(sa[i] - 1);
			
			std::copy(cnt, cnt + m, cur);
			for (int i = n - 1; i >= 0; --i)
				if (sa[i] > 0 && type[sa[i] - 1])
					push_S(sa[i] - 1);
		};
		
		for (int i = 1; i < n; ++i)
			if (type[i - 1] == 0 && type[i] == 1)
				type[i] = 2, lms[n1++] = i;
		induced_sort(lms);
		
		auto lms_equal = [&](int x, int y)
		{
			if (s[x] == s[y])
				while (s[++x] == s[++y] && type[x] == type[y])
					if (type[x] == 2 || type[y] == 2)
						return true;
			return false;
		};
		
		int *s1 = std::remove_if(sa, sa + n, [&](const int x) { return type[x] != 2; });
		for (int i = 0; i < n1; ++i)
			s1[sa[i] >> 1] = ch += ch <= 0 || !lms_equal(sa[i], sa[i - 1]);
		for (int i = 0; i < n1; ++i)
			s1[i] = s1[lms[i] >> 1];
		
		if (ch + 1 < n1)
			sais_core(n1, ch + 1, s1, type + n, lms + n1, cnt + m);
		else
			for (int i = 0; i < n1; ++i)
				sa[s1[i]] = i;
		
		for (int i = 0; i < n1; ++i)
			lms[n1 + i] = lms[sa[i]];
		induced_sort(lms + n1);
	}
	
	template<typename _Char>
	void main(const _Char s[], const int n, const int m)
	{
		static int _lms[Max_N], _cnt[Max_N << 1];
		static char _type[Max_N << 1];
		sais_core(n + 1, m, s, _type, _lms, _cnt);
	}
}

char s[Max_N];
int N;
int sa[Max_N];

int main(int argc, char **argv)
{
	scanf("%s", s);
	N = strlen(s);
	
	SA_IS::sa = sa;
	SA_IS::main(s, N, 128);
	
	for (int i = 1; i <= N; ++i)
		IO << uint32_t(sa[i]) << " \n"[i == N];
	
	return 0;
}
