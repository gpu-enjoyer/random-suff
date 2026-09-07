
#include "graph/graph.hpp"

int main()
{
    Graph     g, gg;
    T_bfs     t_bfs;
    T_topsort t_top;

    g.demo();
    cout << g << "\n";
    g.bfs(t_bfs);
    cout << t_bfs << "\n";
    g.topsort(t_top);
    cout << t_top << "\n\n";

    gg.demo_cyclic();
    cout << gg << "\n";
    gg.bfs(t_bfs);
    cout << t_bfs << "\n";
    gg.topsort(t_top);
    cout << t_top << "\n\n";

    return 0;
}
