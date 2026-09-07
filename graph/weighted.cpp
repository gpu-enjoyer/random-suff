
#include <vector>
using namespace std;

struct Graph_W
{
    // {vertex, cost}
    vector<vector<pair<int, int>>> adj;

    void demo();

    vector<int> dijkstra();
    vector<int> prim();
    vector<int> kruskal();
};


int main() {
    return 0;
}
