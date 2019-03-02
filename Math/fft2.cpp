// Tested with https://www.spoj.com/problems/MUL/
// To multiply a, b and put result in c:
// PolyMul::polynomial_multiply(a, b, c);
struct FFT {
    typedef vector < int64_t > ivector;
    typedef complex < double > ctype;
    typedef vector < ctype > cvector;

    static ctype generate_root(int len, bool invert) {
        double alpha = 2.0 * PI / len * (invert ? -1 : 1);
        return ctype(cos(alpha), sin(alpha));
    }

    static void prepare_output(const cvector& vin, ivector& vout, uint32_t out_size) {
        vout.resize(out_size);
        for (int i = 0; i < out_size; ++i) vout[i] = llround(vin[i].real());
    }

    static void invert(cvector& a) {
        for (auto& x : a) x /= a.size();
    }

    static double PI;
};

using PolyMul = DFT < FFT, FFT::ctype >;

template<> int32_t PolyMul::NBIT = 18;
template<> int32_t PolyMul::N = 1 << PolyMul::NBIT;

double FFT::PI = acos(-1.0);

