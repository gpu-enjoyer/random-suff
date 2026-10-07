
#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;


struct Point {
    int x = rand() % mod;
    int y = rand() % mod;
    static constexpr int mod = 10;
    Point(const int x, const int y) : x(x), y(y) {};
    Point() = default;
};

class Segment {

    Point A, B;

public:

    Segment(const Point& A, const Point& B) : A(A), B(B) {};
    Segment() = default;

    inline int dx() const { return B.x - A.x; }
    inline int dy() const { return B.y - A.y; }

    inline long vec_prod(const Segment& S) const {
        return dx() * S.dy() - dy() * S.dx();
    }

    inline long side(const Point& P) const {
        return this->vec_prod(Segment(A, P));
    }

    bool intersect(const Segment& S) const {

        long side_SA, side_SB, side_A, side_B;
    
        side_SA = side(S.A);
        side_SB = side(S.B);

        //  Collinear
        if (side_SA == 0 && side_SB == 0)
            //  [  [  ]  ]
            return max(min(A.x, B.x), min(S.A.x, S.B.x)) <= min(max(A.x, B.x), max(S.A.x, S.B.x))
                && max(min(A.y, B.y), min(S.A.y, S.B.y)) <= min(max(A.y, B.y), max(S.A.y, S.B.y));

        side_A = S.side(A);
        side_B = S.side(B);

        //  S_x_line_AB  &&  AB_x_line_S
        return ((side_SA < 0) == (side_SB > 0) || side_SA == 0 || side_SB == 0)
            && ((side_A < 0)  == (side_B > 0)  || side_A == 0  || side_B == 0);
    }

    friend ostream& operator<<(ostream&, const Segment&);
};

ostream& operator<<(ostream& os, const Segment& S) {
    return os << "A(" << S.A.x << "," << S.A.y << ")->B(" << S.B.x << "," << S.B.y << ")";
}


// TODO: Bentley-Ottmann()


int main()
{
    srand(time(nullptr));

    vector<Segment> vS(3);

    for (const Segment& s : vS)
        for (const Segment& ss : vS)
            cout << s << " cross " << ss << ": "
                << (s.intersect(ss) ? "true" : "false") << '\n';

    cout << '\n';

    return 0;
}
