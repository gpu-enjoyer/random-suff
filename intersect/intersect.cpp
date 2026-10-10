
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
    bool operator==(const Point& P) const { return x == P.x && y == P.y; }
};


class Segment {

    Point A, B;

    Point l_(const Point& P1, const Point& P2) const {
        if (P1.x != P2.x)
            return P1.x < P2.x ? P1 : P2;
        return P1.y <= P2.y ? P1 : P2;
    }

    Point r_(const Point& P1, const Point& P2) const {
        if (P1.x != P2.x)
            return P1.x > P2.x ? P1 : P2;
        return P1.y >= P2.y ? P1 : P2;
    }

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

        // AB x S
        Rat vp = vec_prod(S);

        // (1) At least one Segment is Point or
        //  (2) Collinear
        if (vp == 0)
        {
            // 2 Segments is 2 Points
            if (A == B && S.A == S.B)
            {
                // Same Points
                if (A == S.A)
                    return A;

                // Different Points
                return nullopt;
            }

            // Only AB is Point (S.A != S.B)
            if (A == B)
            {
                // Point and Segment are not on one line
                if (S.side(A) != 0)
                    return nullopt;
                
                // else: (2) Collinear
            }

            // Only S is Point (A != B)
            if (S.A == S.B)
            {
                // Point and Segment are not on one line
                if (side(S.A) != 0)
                    return nullopt;
                
                // else: (2) Collinear
            }

            // (2) Collinear

            // On same line: AB x AP
            if (side(S.A) == 0)
            {
                //  [-[-]-]  vs  [-] [-]
                Point max_start = r_(l_(A, B), l_(S.A, S.B));
                Point   min_end = l_(r_(A, B), r_(S.A, S.B));

                // Vertical segments
                if (A.x == B.x && S.A.x == S.B.x)
                {
                    // Y: [-[-]-]
                    if (max_start.y < min_end.y)
                        throw invalid_argument("Segments overlapping");

                    // Y: [-][-]
                    if (max_start.y == min_end.y)
                        return max_start;
                }

                // Not vertical segments
                else
                {
                    // X_proj: [-[-]-]
                    if (max_start.x < min_end.x)
                        throw invalid_argument("Segments overlapping");

                    // X_proj: [-][-]
                    if (max_start.x == min_end.x)
                        return max_start;
                }
            }

            // Not on same line
            return nullopt;
        }

        // Not collinear and no Points

        // r == Segment(S.A, P) / S
        Rat r = (dy() * (S.A.x - A.x) - dx() * (S.A.y - A.y)) / vp;

        // P not belongs to S
        if (r < 0 || r > 1)
            return nullopt;

        Point P(S.A.x + r * S.dx(), S.A.y + r * S.dy());

        // P not belongs to AB
        if (P.x < min(A.x, B.x) || P.x > max(A.x, B.x) ||
            P.y < min(A.y, B.y) || P.y > max(A.y, B.y))
            return nullopt;

        // P belongs to S and AB
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
