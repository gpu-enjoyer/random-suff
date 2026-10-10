
#include <optional>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <numeric>  // gcd(), lcm()
#include <cstdlib>  // rand(), srand()
#include <ctime>    // time()

using namespace std;


struct Rat {

    int p = 0;
    int q = 1;

    Rat() = default;

    Rat(long pp, long qq = 1) {
        if (qq == 0)
            throw invalid_argument("qq == 0");
        if (qq < 0) {
            pp = -pp;
            qq = -qq;
        }
        norm(pp, qq);
        p = (int)pp;
        q = (int)qq;
    }

    void norm(long& pp, long& qq) {
        long d = gcd(pp, qq);
        pp /= d;
        qq /= d;
    }

    Rat operator+(const Rat& R) const {
        long m = lcm((long)q, (long)R.q);
        return Rat(p * (m / q) + R.p * (m / R.q), m);
    }
    Rat operator-(const Rat& R) const {
        long m = lcm((long)q, (long)R.q);
        return Rat(p * (m / q) - R.p * (m / R.q), m);
    }
    Rat operator*(const Rat& R) const {
        return Rat((long)p * R.p, (long)q * R.q);
    }
    Rat operator/(const Rat& R) const {
        return Rat((long)p * R.q, (long)q * R.p);
    }

    bool operator==(const Rat& R) const { return p == R.p && q == R.q; }
    bool operator> (const Rat& R) const { return (long)p * R.q > (long)R.p * q; }
    bool operator!=(const Rat& R) const { return !(*this == R); }
    bool operator< (const Rat& R) const { return R > *this; }
    bool operator>=(const Rat& R) const { return !(R > *this); }
    bool operator<=(const Rat& R) const { return !(*this > R); }

    friend ostream& operator<<(ostream&, const Rat&);
};

ostream& operator<<(ostream& os, const Rat& R) {
    if (R.q == 1) return os << R.p;
    return os << R.p << "/" << R.q;
}


struct Point {
    Rat x = rand() % mod;
    Rat y = rand() % mod;
    static constexpr int mod = 10;
    Point(const Rat x, const Rat y) : x(x), y(y) {};
    Point() = default;
};

Point left(const Point& A, const Point& B) {
    if (A.x <= B.x)
        return A;
    return B;
};

Point right(const Point& A, const Point& B) {
    if (A.x >= B.x)
        return A;
    return B;
};


class Segment {

    Point A, B;

public:

    Segment(const Point& A, const Point& B) : A(A), B(B) {};
    Segment() = default;

    Rat dx() const { return B.x - A.x; }
    Rat dy() const { return B.y - A.y; }

    Rat vec_prod(const Segment& S) const {
        return dx() * S.dy() - dy() * S.dx();
    }

    Rat side(const Point& P) const {
        return this->vec_prod(Segment(A, P));
    }

    optional<Point> intersection(const Segment& S) const {

        Rat vp = vec_prod(S);

        if (vp == 0) { // Collinear
            if (side(S.A) == 0) { // On one line
                Point right_start = right(left(A, B), left(S.A, S.B));
                Point    left_end = left(right(A, B), right(S.A, S.B));
                if (right_start.x < left_end.x)
                    throw invalid_argument("Segments overlapping");
                if (right_start.x == left_end.x)
                    return right_start;
            }
            return nullopt;
        }

        Rat r = (dy() * (S.A.x - A.x) - dx() * (S.A.y - A.y)) / vp;

        if (r < 0 || r > 1)
            return nullopt;

        Point P(S.A.x + r * S.dx(), S.A.y + r * S.dy());

        if (P.x < min(A.x, B.x) || P.x > max(A.x, B.x) ||
            P.y < min(A.y, B.y) || P.y > max(A.y, B.y))
            return nullopt;

        return P;
    }

    friend ostream& operator<<(ostream&, const Segment&);
};

ostream& operator<<(ostream& os, const Segment& S) {
    return os << "A(" << S.A.x << "," << S.A.y << ")->B(" << S.B.x << "," << S.B.y << ")";
}


// TODO: Bentley-Ottmann()


int main() {

    // srand(time(nullptr));

    // vector<Segment> vS(3);

    // for (const Segment& s : vS)
    //     for (const Segment& ss : vS)
    //         cout << s << " cross " << ss << ": "
    //             << (s.has_intersection(ss) ? "true" : "false") << '\n';

    // cout << '\n';

    return 0;
}
