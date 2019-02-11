// FFT

#include <iostream>
#include <cmath>

using namespace std;

const double PI = 2 * acos(0);

struct C {
	double a, b;
	C () : a(0), b(0) {}
	C (double a, double b) : a(a), b(b) {}
	C (double theta) : a(cos(theta)), b(sin(theta)) {}
	C bar () const { return C(a, -b); }
	double modsq () const { return a * a + b * b; }
	C operator+ (const C &c) const { return C(a + c.a, b + c.b); }
	C operator* (const C &c) const { return C(a * c.a - b * c.b, a * c.b + b * c.a); }
	C operator/ (const C &c) const {
		C r = (*this) * c.bar();
		return C(r.a / c.modsq(), r.b / c.modsq());
	}
};

// O(nlogn)
// dir is direction of Fourier transform
void fft (C *in, C *out, int step, int size, int dir) {
	if (size < 1) return;
	if (size == 1) { out[0] = in[0]; return; }
	fft(in, out, step*2, size/2, dir);
	fft(in + step, out + size/2, step*2, size/2, dir);
	for (int i = 0; i < size/2; i++) {
		C even = out[i], odd = out[i + size/2];
		out[i] = even + C(dir * 2*PI * i / size) * odd;
		out[i + size/2] = even + C(dir * 2*PI * (i + size/2) / size) * odd;
	}
}

// c[i] = sum of a[j] * b[i-j]
// n is power of 2; index is cyclic
void convolve (int n, C *a, C *b, C *c) {
	C *fa = new C[n];
	C *fb = new C[n];
	C *fc = new C[n];
	fft(a, fa, 1, n, 1);
	fft(b, fb, 1, n, 1);
	for (int i = 0; i < n; i++) fc[i] = fa[i] * fb[i];
	fft(fc, c, 1, n, -1);
	for (int i = 0; i < n; i++) c[i] = c[i] / C(n,0);
}

int main () {
{	// cyclic convolve
	int n = 8;
	C *a = new C[n];
	for (int i = 0; i < n; i++) a[i].a = i;
	C *b = new C[n];
	for (int i = 0; i < n; i++) b[i].a = n-i-1;
	C *c = new C[n];
	convolve(n, a, b, c);
	for (int i = 0; i < n; i++) cout << c[i].a << ' ' << c[i].b << endl;
}
{	// 0-padding convolve (polymult)
	int n = 8;
	C *a = new C[2*n];
	for (int i = 0; i < n; i++) a[i].a = i;
	C *b = new C[2*n];
	for (int i = 0; i < n; i++) b[i].a = i;
	C *c = new C[2*n];
	convolve(2*n, a, b, c);
	for (int i = 0; i < 2*n; i++) cout << c[i].a << ' ' << c[i].b << endl;
}
	return 0;
}

