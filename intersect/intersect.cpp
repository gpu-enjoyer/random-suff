
#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;


struct Point {
    int x = rand() % mod;
    int y = rand() % mod;
    static constexpr int mod = 100;
    Point(const int x, const int y) : x(x), y(y) {};
    Point() = default;
};

class Segment {

private:

    Point A, B;

public:

    Segment(const Point& A, const Point& B) : A(A), B(B) {};
    Segment() = default;

    inline int x() const { return B.x - A.x; }
    inline int y() const { return B.y - A.y; }

    inline long vec_prod(const Segment& S) const {
        return x() * S.y() - y() * S.x();
    }

    inline long side(const Point& P) const {
        return this->vec_prod(Segment(A, P));
    }

    bool intersect(const Segment& S) const {
        const long side_S_A = side(S.A);
        const long S_side_A = S.side(A);
        return
            ((side_S_A > 0) != (side(S.B) > 0))
                && ((S_side_A > 0) != (S.side(B) > 0))
            || side_S_A == 0
                && min(A.x, B.x) <= S.A.x && S.A.x <= max(A.x, B.x)
                && min(A.y, B.y) <= S.A.y && S.A.y <= max(A.y, B.y)
            || S_side_A == 0
                && min(S.A.x, S.B.x) <= A.x && A.x <= max(S.A.x, S.B.x)
                && min(S.A.y, S.B.y) <= A.y && A.y <= max(S.A.y, S.B.y);
    }

    friend ostream& operator<<(ostream&, const Segment&);
};

ostream& operator<<(ostream& os, const Segment& S) {
    return os
        << "A(" << S.A.x << ", " << S.A.y << ") "
        << "B(" << S.B.x << ", " << S.B.y << ") "
        << "vec(" << S.x() << ", " << S.y() << ")\n";
}


enum class Event {
    begin,
    intersection,
    end
};

// TODO: Bentley-Ottmann()


int main() {

    vector<Segment> vS(10);

    for (Segment s : vS)
        cout << s;
    cout << '\n';

    return 0;
}
