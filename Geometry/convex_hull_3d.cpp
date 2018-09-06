// Tested with: http://poj.org/problem?id=3528

struct dot
{
	double x, y, z;
	dot(){}
	dot(double _x, double _y, double _z) {x = _x; y = _y; z = _z;}
	void read() {scanf("%lf%lf%lf", &x, &y, &z);}
	dot operator%(const dot &o)
	{
		return dot(y * o. z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x);
	}
	dot operator-(const dot &o)
	{
		return dot(x - o.x, y - o.y, z - o.z);
	}
	double operator*(const dot &o)
	{
		return x * o.x + y * o.y + z * o.z;
	}
	double dis() {return sqrt(x * x + y * y + z * z);}
}p[Maxn];

struct face
{
	int a, b, c, del;
	dot n;
	face(){}
	face(int _a, int _b, int _c)
	{
		a = _a; b = _b; c = _c;
		n = (p[b] - p[a]) % (p[c] - p[b]);
		del = 0;
	}
}f[Maxn * 10];

bool out(int a, int b) // check if the point_a is out of the face_b
{
	double s = f[b].n * (p[a] - p[f[b].a]);
	//printf("out : %d %d %.3lf\n", a, b, s);
	return s < -1e-9;
}

void add(int a, int b, int c)
{
	++m; f[m] = face(a, b, c);
	//printf("add : %d %d %d\n", a, b, c);
	be[a][b] = m; be[b][c] = m; be[c][a] = m;
}

int main()
{
	scanf("%d", &n);
	rep(i, n) p[i].read();
	f[0] = face(1, 2, 3); if (out(4, 0)) swap(f[0].a, f[0].b);
	add(f[0].a, f[0].b, f[0].c);
	f[0] = face(1, 2, 4); if (out(3, 0)) swap(f[0].a, f[0].b);
	add(f[0].a, f[0].b, f[0].c);
	f[0] = face(1, 3, 4); if (out(2, 0)) swap(f[0].a, f[0].b);
	add(f[0].a, f[0].b, f[0].c);
	f[0] = face(2, 3, 4); if (out(1, 0)) swap(f[0].a, f[0].b);
	add(f[0].a, f[0].b, f[0].c);
	FOR(i, 5, n)
	{
		rep(j, m) if (!f[j].del && out(i, j))
			f[j].del = 1;
		//printf("%d\n", i);
		rep(j, m) if (f[j].del == 1)
		{
			int a = f[j].a, b = f[j].b, c = f[j].c;
			//printf("%d %d %d\n", a, b, c);
			if (!f[be[b][a]].del) add(a, b, i);
			if (!f[be[c][b]].del) add(b, c, i);
			if (!f[be[a][c]].del) add(c, a, i);
			f[j].del = -1;
		}
	}
	double ans = 0;
	rep(i, m) if (!f[i].del)
		ans += f[i].n.dis();
	printf("%.3lf\n", ans * 0.5);
	return 0;
}
