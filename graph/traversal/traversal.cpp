
#include "traversal.hpp"
#include <iomanip>        // Functions for operator<<
#include <string>         // Functions for operator<<


// Traversal for BFS

void T_bfs::reset(const int v_num) {
    dist.assign(v_num, vector<int>(v_num, dist0));
    q = queue<int>();
}

void T_bfs::reset(const int v_num, const int v) {
    if (dist.size() != v_num)
        reset(v_num);
    dist[v].assign(v_num, dist0);
    q = queue<int>();
}


// Traversal for Topological Sort

void T_topsort::reset(const int v_num) {
    cycles.clear();
    in.assign(v_num, t0);
    out.assign(v_num, t0);
    parent.assign(v_num, t0);
    topsort = stack<int>();
    timer = 0;
}


// Functions for operator<<

string spaces(const int w) {
    return string(w, ' ');
}

template <typename T>
string align(const T& val, const int w) {
    ostringstream ss;
    if constexpr(is_integral_v<T>)
        ss << setw(w) << to_string(val);
    else if (string{val} == "·")
        ss << string(w - 1, ' ') << val;
    else
        ss << setw(w) << val;
    return ss.str();    
}

template <typename T>
string grey(const T& s) {
    ostringstream ss;
    ss << "\033[48;5;240m" << s << "\033[49m";
    return ss.str();
}

template <typename T>
string cell(const T& t, const int w) {
    return grey(align(t, w));
}


// operator<<

constexpr int LW = 7;

ostream& operator<<(ostream& os, stack<int> stck) {
    while (!stck.empty()) {
        os << stck.top() << ' ';
        stck.pop();
    }
    return os;
}

ostream& operator<<(ostream& os, const T_bfs& t) {
    const int w  = max(2,  (int)to_string(t.dist.size()).size());
    const int lw = max(LW, (int)to_string(t.dist.size()).size());

    os << spaces(lw + 1);
    for (int v = 0; v < t.dist.size(); ++v)
        os << cell(v, w) << ' ';

    for (int v = 0; v < t.dist.size(); ++v) {
        os << '\n' << spaces(lw - w) << cell(v, w) << ' ';
        for (int d : t.dist[v])
            if (d == t.dist0) os << align("·", w) << ' ';
            else              os << align(d, w) << ' ';
    }

    return os << '\n';
}

ostream& operator<<(ostream& os, const T_topsort& t) {
    const int w  = max(2, (int)to_string(2 * t.in.size()).size());

    os << spaces(LW + 1);
    for (int i = 0; i < t.in.size(); ++i)
        os << cell(i, w) << ' ';

    os << '\n' << spaces(LW - 2) << grey("in") << ' ';
    for (int v = 0; v < t.in.size(); ++v)
        os << align(t.in[v], w) << ' ';

    os << '\n' << spaces(LW - 3) << grey("out") << ' ';
    for (int v = 0; v < t.out.size(); ++v)
        os << align(t.out[v], w) << ' ';

    os << "\n\n";

    if (t.cycles.empty())
        os << spaces(LW - 7) << grey("topsort") << ' ' << t.topsort;
    else {
        os << spaces(LW - 6) << grey("cycles") << ' ';
        for (vector<int> cycle : t.cycles) {
            for (int v : cycle)
                os << v << ' ';
            os << '\n' << spaces(LW + 1);
        }
    }

    return os << '\n';
}
