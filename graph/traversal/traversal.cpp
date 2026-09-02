
#include "traversal.hpp"
#include <iomanip>        // Functions for operator<<
#include <string>


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
    in.assign(v_num, t0);
    out.assign(v_num, t0);
    topsort = stack<int>();
    timer = 0;
    cycle = false;
}


// Functions for operator<<

ostream& operator<<(ostream& os, stack<int> a) {
    while (a.empty() == false) {
        os << a.top() << ' ';
        a.pop();
    }
    return os;
}

string grey(const string& s) {
    return "\033[48;5;240m" + s + "\033[49m";
}

string grey(int digit) {
    return grey(to_string(digit));
}

string cell(int x, int w) {
    ostringstream ss;
    ss << setw(w) << x;
    return grey(ss.str());
}

string cell(const string& s, int w) {
    ostringstream ss;
    ss << left << setw(w) << s;
    return grey(ss.str());
}

ostream& print_vec(ostream& os, const vector<int>& a, int w) {
    for (int x : a)
        os << setw(w) << x << ' ';
    return os;
}


// operator<<

// Backlog:
//  Такая же красота, как у T_topsort
ostream& operator<<(ostream& os, const T_bfs& t) {
    os << "  ";
    for (int i = 0; i < t.dist.size(); ++i)
        os << grey(i) << ' ';
    for (int i = 0; i < t.dist.size(); ++i) {
        os << '\n' << grey(i) << ' ';
        for (int d : t.dist[i])
            os << (d == t.dist0 ? "·" : to_string(d)) << ' ';
    }
    return os << '\n';
}


ostream& operator<<(ostream& os, const T_topsort& t) {
    const int n = t.in.size();

    // Ширина колонки = длина самого длинного числа
    //  Индексы: 0 .. n-1
    //   Значения in/out: -1 .. 2n
    int w = 1;
    auto upd = [&w](int x) { w = max(w, (int)to_string(x).size()); };
    for (int i = 0; i < n; ++i) upd(i);
    for (int x : t.in) upd(x);
    for (int x : t.out) upd(x);
    w = max(w, 2);

    // Ширина колонки заголовков in, out, topsort
    const int lw = 8;

    os << string(lw, ' ') << ' ';
    for (int i = 0; i < n; ++i)
        os << cell(i, w) << ' ';

    os << '\n' << cell("in",  lw) << ' ';
    print_vec(os, t.in,  w);

    os << '\n' << cell("out", lw) << ' ';
    print_vec(os, t.out, w);

    os << "\n\n" << cell("topsort", lw) << ' ';
    if (t.cycle) {
        os << "cycles detected: \n";
        os << cell("", lw) << " ... \n";
        // TODO: Печатать циклы из вектора t.cycle
    }
    else
        os << t.topsort;

    return os << '\n';
}
