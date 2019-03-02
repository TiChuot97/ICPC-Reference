// Base class for FFT and DFT
template < class Transform, class DType >
struct DFT {
    typedef vector < int64_t > ivector;
    typedef vector < DType > dvector;

    static void fft(dvector& a, bool invert = false) {
        int n = a.size();
        for (int i = 1, j = 0; i < n; ++i) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (j > i) swap(a[i], a[j]);
        }
        for (int iter = 0, len = 1; len < n; ++iter, len *= 2) {
            DType wlen = Transform::generate_root(2 * len, invert);
            for (int i = 0; i < n; i += 2 * len) {
                DType w(1);
                for (int j = 0; j < len; ++j) {
                    auto x = a[i + j], y = a[i + j + len];
                    a[i + j] = x + w * y;
                    a[i + j + len] = x - w * y;
                    w *= wlen;
                }
            }
        }
        if (invert) Transform::invert(a);
    }

    static void polynomial_multiply(const ivector& a, const ivector& b, ivector& out) {
        int new_size = a.size() + b.size() - 1;
        for (NBIT = 0, N = 1; N < new_size; N *= 2, ++NBIT) {}
        dvector fa(a.begin(), a.end()), fb(b.begin(), b.end());
        fa.resize(N); fft(fa);
        fb.resize(N); fft(fb);
        for (int i = 0; i < fa.size(); ++i) fa[i] *= fb[i];
        fft(fa, true);
        Transform::prepare_output(fa, out, new_size);
    }

    static int32_t NBIT, N;
};
