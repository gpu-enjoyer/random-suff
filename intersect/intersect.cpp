
#include <vector>
#include <iostream>
#include <numeric>  // struct Rat: gcd(), lcm()
#include <optional> // optional<Point> intersection()
#include <set>      // find_intersections()

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
    if (R.q == 1)
        return os << R.p;
    int n = R.p / R.q;
    return os << n << "+" << R.p - n * R.q << "/" << R.q;
}


struct Point {
    Rat x = 0;
    Rat y = 0;
    Point(const Rat x, const Rat y) : x(x), y(y) {};
    Point() = default;
    bool operator==(const Point& P) const { return x == P.x && y == P.y; }
};

ostream& operator<<(ostream& os, const Point& P) {
    return os << "(" << P.x << "," << P.y << ")";
}


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

    bool operator==(const Segment& S) const {
        return A == S.A && B == S.B || A == S.B && B == S.A;
    }

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
                        // Bentley-Ottmann' invariant
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
                        // Bentley-Ottmann' invariant
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

    Point upper() const {
        if (A.y != B.y)
            return (A.y < B.y ? A : B);
        return (A.x <= B.x ? A : B);
    }

    Point lower() const {
        if (A.y != B.y)
            return (A.y > B.y ? A : B);
        return (A.x >= B.x ? A : B);
    }

    friend ostream& operator<<(ostream&, const Segment&);
};

ostream& operator<<(ostream& os, const Segment& S) {
    return os << S.A << "->" << S.B;
}

vector<Segment> demo() {
    vector<Segment> vS;
    vS.reserve(11);
    vS.push_back(Segment(Point( 3,  0), Point(21, 27))); // 0
    vS.push_back(Segment(Point(18,  0), Point( 3, 30))); // 1
    vS.push_back(Segment(Point( 3,  3), Point(23, 15))); // 2
    vS.push_back(Segment(Point(15,  3), Point(15, 21))); // 3
    vS.push_back(Segment(Point( 3,  8), Point( 9, 13))); // 4
    vS.push_back(Segment(Point( 6,  8), Point( 6,  8))); // 5
    vS.push_back(Segment(Point(11, 19), Point(13, 21))); // 6
    vS.push_back(Segment(Point(13, 19), Point(11, 21))); // 7
    vS.push_back(Segment(Point( 4, 23), Point(22, 23))); // 8
    vS.push_back(Segment(Point(22, 23), Point(17, 32))); // 9
    vS.push_back(Segment(Point( 5, 26), Point(17, 32))); // 10
    return vS;
}


void find_intersections_bruteforce(const vector<Segment>& vS) {

    int count = 0;

    cout << "find_intersections_bruteforce() \n\n";

    for (int i = 0; i < vS.size(); ++i)
        cout << i << ". " << vS[i] << '\n';
    cout << '\n';

    for (int i = 0; i < vS.size(); ++i)
        for (int j = i + 1; j < vS.size(); ++j) {
            optional<Point> p = vS[i].intersection(vS[j]);
            if (p) {
                cout << i << " x " << j << " = " << p.value() << '\n';
                ++count;
            }
        }
    cout << '\n';

    cout << "Found " << count << " intersections \n\n";
}


enum class Kind { upper, containing, lower };

struct Event {
    Point  P;
    Kind   kind;
    size_t i, j;
    Event(Point P, Kind kind, size_t i, size_t j = 0)
        : P(P), kind(kind), i(i), j(j) {};
};

struct EventCmp {
    bool operator()(const Event& e1, const Event& e2) const {
        if (e1.P.y != e2.P.y)
            return e1.P.y < e2.P.y;
        if (e1.P.x != e2.P.x)
            return e1.P.x < e2.P.x;
        if (e1.kind != e2.kind)
            return (int)e1.kind < (int)e2.kind;
        return e1.i < e2.i;
    }
};

// TODO
struct SegmentCmp {
    bool operator()(const Segment& s1, const Segment& s2) const;
};

// Bentley-Ottmann
void find_intersections(const vector<Segment>& vS) {

    set<Event,   EventCmp>   events;
    // set<Segment, SegmentCmp> status;

    for (int i = 0; i < vS.size(); ++i) {
        events.insert(Event(vS[i].upper(), Kind::upper, i));
        events.insert(Event(vS[i].lower(), Kind::lower, i));
    }

}


int main() {

    const vector<Segment> vS = demo();

    find_intersections_bruteforce(vS);

    // find_intersections(vS);

    return 0;
}
