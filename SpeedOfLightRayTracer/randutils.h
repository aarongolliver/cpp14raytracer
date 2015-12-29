#pragma once
#include <random>
#include <functional>
#include "genvec.h"

namespace randutils {
    std::random_device devurand;
}

template<typename T = float>
auto random_point_on_sphere(std::mt19937_64& mt) {
    using namespace randutils;
    using genvec::vec;
    using std::uniform_real_distribution;
    
    uniform_real_distribution<T> d(-1,1);
    
    while (true) {
        using fn = std::function<T()>;

        auto v = vec<T, 4>{ static_cast<fn>([d, &mt]() { return d(mt); }) };
        auto vsq = v * v;

        auto lensq = vsq.sum();
        if (lensq >= 1) continue;

        auto invlensq = 1.0f / lensq;

        auto x = 2.f*(v[1]   * v[3]   +   v[0] * v[2]  ) * invlensq;
        auto y = 2.f*(vsq[2] * vsq[3] - vsq[0] * vsq[1]) * invlensq;
        auto z =     (vsq[0] * vsq[3] - vsq[1] * vsq[2]) * invlensq;
        return vec<T, 3> { x, y, z };
    }
}