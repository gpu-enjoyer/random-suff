
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <chrono>

using namespace std;
using namespace chrono;


template <typename T>
bool check(
    const vector<T>& a)
{
    for (size_t i = 0; i < a.size() - 1; ++i)
        if (a[i] > a[i + 1]) return false;
    return true;
}


template <typename T>
void gen_rand(
    vector<T>& a,
    size_t size,
    size_t range)
{
    static_assert(
        std::is_same_v<T, int> || std::is_same_v<T, size_t>,
        "T must be int or size_t"
    );

    static std::mt19937 mt(time(nullptr));
    a.resize(size);

    if constexpr (std::is_same_v<T, size_t>)
        for (size_t i = 0; i < a.size(); ++i)
            a[i] = mt() % range;

    else if constexpr (std::is_same_v<T, int>)
        for (size_t i = 0; i < a.size(); ++i)
            a[i] = 2 * int(mt() % range) - int(range);
}

template <typename T>
void gen_rand(
    vector<T>& a,
    size_t size)
{
    mt19937 mt(time(nullptr));
    a.resize(size);
    for (size_t i = 0; i < a.size(); ++i)
		a[i] = mt();
}


void gen_rand_unique(
    std::vector<unsigned long>& a,
    size_t size,
    size_t range)
{
    if (range <= size)
        throw std::runtime_error("Range is too small");
    static std::mt19937 mt(time(nullptr));
    std::vector<unsigned long> pool;
    pool.reserve(range);
    for (int elem = 0; elem <= range; ++elem)
        pool.push_back(elem);
    std::shuffle(pool.begin(), pool.end(), mt);
    a.assign(pool.begin(), pool.begin() + size);
}



template <typename T>
ostream& operator<<(
    ostream& os,
    const vector<T>& a)
{
    for (size_t i = 0; i < a.size(); ++i)
        os << a[i] << ' ';
    return os;
}


template <
    typename  SortFunc,
    typename  T,
    typename... Args
>
double get_time(
    SortFunc   sort_func,
    vector<T>  a,
    Args...    args)
{
	auto time0 = steady_clock::now();
	if (!sort_func(a, args...)) 
        throw std::runtime_error("Wrong sort");
	auto time1 = steady_clock::now();
	duration<double> time = time1 - time0;
	return time.count();
}
