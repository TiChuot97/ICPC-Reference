// NumberTheory

#include <iostream>
#include <cmath>
#include <vector>
#define LL long long

using namespace std;

LL pow_mod (LL b, LL e, LL MOD) {
	LL ans = 1, base = b;
	while (e) {
		if (e & 1) ans = ans * base % MOD;
		base = base * base % MOD;
		e >>= 1;
	}
	return ans;
}

// 扩展欧几里得算法: x * p + y * q = gcd(p, q)
pair<LL,pair<LL,LL>> extended_euclid (LL p, LL q) {
	if (q == 0) return {p, {1, 0}};
	auto res = extended_euclid(q, p%q);
	LL gcd = res.first;
	LL y = res.second.first;
	LL x = res.second.second;
	y -= p / q * x;
	return {gcd, {x, y}};
}

// 模线性方程: ax = b (mod n)
// returns smallest nonnegative solution; plus n/gcd to get all #(gcd) solutions
LL mod_linear_equation (LL a, LL b, LL n) {
	auto res = extended_euclid(a, n);
	LL gcd = res.first;
	LL x = res.second.first;
	LL y = res.second.second;
	if (b % gcd) return -1;
	x *= b / gcd;
	x %= n / gcd;
	if (x < 0) x += n / gcd;
	return x;
}

// 中国剩余定理: x = bi (mod ni)
LL chinese_remainder_theorm (vector<int> b, vector<int> n) {
	int len = b.size();
	LL prod = 1, ans = 0;
	for (int i = 0; i < len; i++) prod *= n[i];
	for (int i = 0; i < len; i++) {
		LL conj = prod / n[i];
		auto res = extended_euclid(conj, n[i]);
		LL p = res.second.first;
		ans += conj * p * b[i];
	}
	ans %= prod;
	if (ans < 0) ans += prod;
	return ans;
}

// 原根 (p is prime)
LL primitive_root (LL p) {
	LL tmp = p - 1;
	vector<LL> pf;
	for (LL i = 2; i <= tmp/i; i++) {
		if (tmp % i == 0) {
			pf.push_back(i);
			while (tmp % i == 0) tmp /= i;
		}
	}
	if (tmp != 1) pf.push_back(tmp);
	for (LL g = 1; ; g++) {
		bool flag = true;
		for (LL i : pf) {
			if (pow_mod(g, (p-1)/i, p) == 1) {
				flag = false;
				break;
			}
		}
		if (flag) return g;
	}
	return -1;
}

// 离散对数: a^x = b (mod p) (p is prime)
// O(sqrt(p))
int log_remainder (int a, int b, int p) {
	int root = (int)sqrt(p);
	vector<int> is_valid(65536, -1);
	int temp = 1;
	for (int i = 0; i < root; i++) {
		is_valid[temp] = i;
		temp = temp * a % p;
	}
	int curr = 1;
	for (int q = 0; root*q < p; q++) {
		int r = mod_linear_equation(curr, b, p);
		if (is_valid[r] != -1) return root * q + is_valid[r];
		curr = curr * temp % p;
	}
	return -1;
}

// N次剩余: x^a = b (mod p) (p is prime)
// O(sqrt(p))
int exp_remainder (int a, int b, int p) {
	int g = primitive_root(p);
	int s = log_remainder(g, b, p);
	if (b == 0) return 0;
	if (s == -1) return -1;
	auto res = extended_euclid(a, p-1);
	int d = res.first;
	int x = res.second.first;
	int y = res.second.second;
	if (s % d) return -1;
	x = x * s/d % (p-1);
	if (x < 0) x += p-1;
	for (int i = 0; i < d; i++) x = (x + (p-1)/d) % (p-1);
	return pow_mod(g, x, p);
}

// 素数判定Miller-Rabin
// O(log(n)^3)
vector<int> candidates (int n) {
	if (n < 2047) return {2};
	if (n < 1373653) return {2,3};
	if (n < 25326001) return {2,3,5};
	return {2,3,5,7};
//	if (n < 3215031751) return {2,3,5,7};
//	if (n < 2152302898747) return {2,3,5,7,11};
//	if (n < 3474749660383) return {2,3,5,7,11,13};
//	if (n < 341550071728321) return {2,3,5,7,11,13,17};
//	if (n < 3825123056546413051) return {2,3,5,7,11,13,17,19,23};
//	return {2,3,5,7,11,13,17,19,23,29,31,37};
}
bool prime_test (int n, int a, int d) {
	if (n == 2 || n == a) return true;
	if ((n & 1) == 0) return false;
	while (!(d & 1)) d >>= 1;
	int t = pow_mod(a, d, n);
	while ((d != n-1) && (t != 1) && (t != n-1)) {
		t = (long long)t * t % n;
		d <<= 1;
	}
	return (t == n-1 || (d & 1) == 1);
}
bool is_prime (int n) {
	if (n < 2) return false;
	vector<int> as = candidates(n);
	for (int a : as) {
		if (!prime_test(n, a, n-1)) return false;
	}
	return true;
}

// 质因数分解
// O(sqrt(n))
vector<pair<int,int>> prime_decomposition (int n) {
	vector<pair<int,int>> ans;
	for (int p = 2; p*p <= n; p++) {
		int a = 0;
		while (n % p) { n /= p; a++; }
		if (a) ans.push_back({p, a});
	}
	if (n > 1) ans.push_back({n, 1});
	return ans;
}

// Euler函数：单点
// O(sqrt(n))
int phi (int n, int key = 2) {
	if (n == 1) return 1;
	while (n % key && key * key <= n) key++;
	if (key * key > n) return n-1;
	if (n / key % key) return phi(n/key, key+1) * (key-1);
	return phi(n/key, key) * key;
}
// Euler函数：预处理
// O(nlogn)
void phi_gen (int n) {
	int mindiv[n+1], phi[n+1];
	for (int i = 1; i <= n; i++) mindiv[i] = i;
	for (int i = 2; i*i <= n; i++) {
		if (mindiv[i] != i) continue;
		for (int j = i*i; j <= n; j += i) mindiv[j] = i;
	}
	phi[1] = 1;
	for (int i = 2; i <= n; i++) {
		phi[i] = phi[i / mindiv[i]];
		if ((i / mindiv[i]) % mindiv[i] == 0) phi[i] *= mindiv[i];
		else phi[i] *= mindiv[i] - 1;
	}
}

// Mobius函数：单点
// O(sqrt(n))
int mu (int n) {
	vector<pair<int,int>> pd = prime_decomposition(n);
	for (pair<int,int> e : pd) {
		if (e.second > 1) return 0;
	}
	return (pd.size() % 2 == 0) ? 1 : -1;
}
// Mobius函数：预处理
// O(nlogn)
void mu_gen (int n) {
	int mu[n+1];
	for (int i = 1; i <= n; i++) {
		int target = i == 1;
		int delta = target - mu[i];
		mu[i] = delta;
		for (int j = i+i; j <= n; j += i) mu[j] += delta;
	}
}

int main ()
{
	/* Euclid; Extended Euclid
	 * 模线性方程 ax = b (mod n): 解方程 ax + ny = gcd(a,n) (mod n); x *= (b / gcd(a,n)); x += n / gcd(a,n)
	 * 中国剩余定理 x = bi (mod ni): Ni*pi + ni*qi = 1 (mod n); x = sum(Ni*pi*bi)
	 * 原根：p为素数，a为p的原根 <=> a = min {d | d^1,d^2,...,d^(p-1)互不同余}
		* a为p的原根 <=> Ord(a,p) = phi(p)
			* a模p的阶: Ord(a,p) = min {d | a^d = 1 (mod p)}
		* a为p的原根 <=> 对任意(p-1)的素因子pi, a^((p-1)/pi) != 1 (mod p)
	 * N次剩余 x^a = b (mod p) // g为p的原根; g^r = x, g^s = b (mod p); ar = s (mod p-1); x = g^r
	 * 离散对数 a^x = b (mod p) // giant-step baby-step
	 
	 * 素数筛法
	 * 素数判定Miller-Rabin // N-1 = 2^s*d; r=1~s: if a^((2^r)*d) == 1 && a^((2^(r-1))*d) != +-1 then N is composite; if a^(N-1) == 1 then N is prime
	 * 质因数分解
	 * Euler函数 phi(n) = 小于等于n且与n互质的正整数个数 // p is smallest prime factor of N; phi(N) = phi(N/p)*p (p^2|N); phi(N) = phi(N/p)*(p-1) (p^2!|N)
	 * Mobius函数：若有p^2因子则为0，否则质因数个数为偶数时为1，为奇数时为-1
	 */
	
	return 0;
}
