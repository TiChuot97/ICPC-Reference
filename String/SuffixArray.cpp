#include <iostream>
#include <vector>
#define F first
#define S second

using namespace std;

// Suffix Array (nlogn)
// Author: Gary
// Tested on MCPC18-K

// pos [0,n-1]
// rank [0,n-1]

int n;
vector<int> s;  // every element in range [0,n-1]
vector<int> sa;  // rank -> pos
vector<int> rk;  // pos -> rank
vector<int> height;  // height[i] = length of common prefix of suffix(sa[i]) and suffix(sa[i+1])

void suffix_array () {
    rk = s;
    for (int off = 1; off < n; off <<= 1) {
        // radix sort
        int m = max(n, 128);
        vector<pair<int,int>> p(n);
        for (int i = 0; i < n; i++) p[i] = {rk[i]+1, (i+off<n) ? rk[i+off]+1 : 0};

        vector<int> sa(n), cnt(m+1, 0);
        for (int i = 0; i < n; i++) cnt[p[i].S]++;
        for (int i = 0; i < m; i++) cnt[i+1] += cnt[i];
        for (int i = n-1; i >= 0; i--) sa[--cnt[p[i].S]] = i;

        vector<int> sa2(n), cnt2(m+1, 0);
        for (int i = 0; i < n; i++) cnt2[p[i].F]++;
        for (int i = 0; i < m; i++) cnt2[i+1] += cnt2[i];
        for (int i = n-1; i >= 0; i--) sa2[--cnt2[p[sa[i]].F]] = sa[i];

        pair<int,int> prev = {0, 0};
        for (int i = 0, r = -1; i < n; i++) {
            if (p[sa2[i]] == prev) rk[sa2[i]] = r;
            else { rk[sa2[i]] = ++r; prev = p[sa2[i]]; }
        }
    }
    sa.assign(n+1, -1);
    for (int i = 0; i < n; i++) sa[rk[i]] = i;
}

void build_height () {
    height.assign(n-1, -1);
    for (int i = 0, k = 0; i < n; i++) {
        if (rk[i] == n-1) continue;
        if (k) k--;
        for (int j = sa[rk[i]+1]; i+k<n && j+k<n && s[i+k] == s[j+k]; k++);
        height[rk[i]] = k;
    }
}

int main () {
    string str; cin >> str;
    n = str.length(); s.assign(n, 0);
    for (int i = 0; i < n; i++) s[i] = str[i] - 'a';
    suffix_array();
    build_height();
    int ans = 0, index = -1;
    for (int i = 0; i < n-1; i++) {
        if (ans < height[i]) {
            ans = height[i];
            index = sa[i];
        }
    }
    cout << str.substr(index, ans) << endl;

    return 0;
}

