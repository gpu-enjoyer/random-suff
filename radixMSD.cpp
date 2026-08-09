
#include <iostream>
#include "lib.hpp"

using namespace std;

// R = 256
// k = 8


// -1 .. 255
inline int ch(
    const string& str,
    const size_t  ch_n
) {
    if (ch_n >= str.size()) return -1;
    return (uint8_t)str[ch_n];
}

void __radixMSD(
    vector<string>& a,
    vector<string>& tmp,
    const int l,
    const int r,
    const size_t ch_n)
{
    if (l == r || ch_n >= 8)
        return;

    // { -1 0 1 .. 255 }
    int count[257] = {0};

    //  [0..256] -> [count(-1)..count(255)]
    for (int i = l; i < r; ++i)
        ++count[ch(a[i], ch_n) + 1];

    //  Получаем индекс последней строки
    //   в отсортированном порядке
    //    c буквой ch() на позиции ch_n
    for (int i = 1; i < 257; ++i)
        count[i] += count[i - 1];

    //  Запись значений строк в нужном порядке
    for (int i = l; i < r; ++i)
        // После цикла count[i] указывает на конец позиции
        //  предыдущего ключа
        tmp[l + --count[ch(a[i], ch_n) + 1]] = std::move(a[i]);

    for (int i = l; i < r; ++i)
        a[i] = std::move(tmp[i]);

    //  Пропускаем короткие строки i = 0 (ch = -1)
    for (int i = 1; i < 256; ++i)
        // Рекурсия
        __radixMSD(a, tmp, l + count[i], l + count[i + 1], ch_n + 1);
    // Отдельный вызов для i = 256 (ch == 255)
    __radixMSD(a, tmp, l + count[256], r, ch_n + 1);
}

bool radixMSD(
    vector<string>& a)
{
    vector<string> tmp(a.size());
    __radixMSD(a, tmp, 0, a.size(), 0);
    return check(a);
}


int main()
{
    vector<string> a, b;

    gen_strings(a, 10000);
    b.resize(a.size());
    b = a;

    double t_std = get_time(stdSort,   a);
    double t     = get_time(radixMSD, b);

    cout << b << "\n\n";

    cout << "t_std = " << t_std << "\n";
    cout << "t     = " << t     << "\n\n";

    return 0;
}
