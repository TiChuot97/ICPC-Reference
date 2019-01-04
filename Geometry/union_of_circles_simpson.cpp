// Input N circles, output area of the union of circles
// Simpson integral
// Tested with: https://www.spoj.com/problems/CIRU/

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;
#define sqr(x) ((x) * (x))
#define eps 1e-8
#define Maxn 1005
struct dot
{
    double x, y;
    double dis(dot &o)
    {
        return sqrt(sqr(x - o.x) + sqr(y - o.y));
    }
};
int lx = 1000, rx = -1000;
struct circle
{
    dot o; int r;
    void init()
    {
        int x, y;
        scanf("%d%d%d", &x, &y, &r);
        lx = min(lx, x - r); rx = max(rx, x + r);
        o.x = x; o.y = y;
    }
    bool in(circle &b)
    {
        return (b.r - r - o.dis(b.o) >= -eps);
    }
    bool operator==(const circle &b)
    {
        return r == b.r && fabs(o.x - b.o.x) <= eps && fabs(o.y - b.o.y) <= eps;
    }
}tmp[Maxn], c[Maxn];
struct seg
{
    double v; int s;
    bool operator<(const seg &o)
        const{return v < o.v - eps;}
}l[Maxn * 2];
int n, m;

void Init()
{
    scanf("%d", &m);
    for (int i = 1; i <= m; ++i)
    {
        tmp[++n].init();
        for (int j = 1; j <= n - 1; ++j)
            if (tmp[j] == tmp[n])
                {--n; break;}
    }
    m = n; n = 0;
    for (int i = 1; i <= m; ++i)
    {
        bool f = 0;
        for (int j = 1; j <= m; ++j) if (j != i)
            if (tmp[i].in(tmp[j]))
            {
                f = 1;
                break;
            }
        if (!f) c[++n] = tmp[i];
    }
}

inline double get(double x)
{
    int t = 0, now = 0;
    double d, last, s = 0;
    for (int i = 1; i <= n; ++i)
    {
        if (fabs(x - c[i].o.x) - c[i].r >= -eps) continue;
        d = sqrt(sqr(c[i].r) - sqr(x - c[i].o.x));
        l[++t].v = c[i].o.y - d; l[t].s = 1;
        l[++t].v = c[i].o.y + d; l[t].s = -1;
    }
    sort(l + 1, l + 1 + t);
    for (int i = 1; i <= t; ++i)
    {
        now += l[i].s;
        if (now == 1 && l[i].s == 1) last = l[i].v;
        if (now == 0) s += l[i].v - last;
    }
    return s;
}

double simpson(double l, double r, double lx, double mx, double rx)
{
    double m = (l + r) * 0.5, lp, rp, s, ls, rs;
    lp = get((l + m) * 0.5);
    rp = get((m + r) * 0.5);
    s = (lx + rx + 4 * mx) * (r - l) / 6;
    ls = (lx + mx + 4 * lp) * (m - l) / 6;
    rs = (mx + rx + 4 * rp) * (r - m) / 6;
    if (fabs(ls + rs - s) <= 1e-6)
        return s;
    return simpson(l, m, lx, lp, mx) + simpson(m, r, mx, rp, rx);
}

void Work()
{
    double s = 0, last = get(lx), now;
    for (int i = lx; i <= rx - 1; ++i)
    {
        now = get(i + 1);
        if (fabs(last) > eps || fabs(now) > eps)
            s += simpson(i, i + 1, last, get(i + 0.5), now);
        last = now;
    }
    printf("%.3lf\n", s);
}

int main()
{
    Init();
    Work();
    return 0;
}
