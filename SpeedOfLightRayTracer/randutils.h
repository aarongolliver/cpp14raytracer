#pragma once
#include <random>
#include "genvec.h"

std::random_device devurand;
std::mt19937_64 mt((uint64_t(devurand()) << 32) | devurand());

auto random_point_on_sphere() {
    using genvec::vec;
    std::uniform_real_distribution<float> d(-1,1);
    while (true) {
        auto v = vec<float, 4> { d(mt), d(mt), d(mt), d(mt) };
        auto vsq = v * v;
        auto lensq = vsq.sum();
        if (lensq >= 1) continue;

        auto invlensq = 1.0f / lensq;
        auto x = 2.f*(v[1] * v[3] + v[0] * v[2]) * invlensq;
        auto y = 2.f*(vsq[2] * vsq[3] - vsq[0] * vsq[1]) * invlensq;
        auto z =     (vsq[0] * vsq[3] - vsq[1] * vsq[2]) * invlensq;
        return vec<float, 3>{x, y, z};
    }
}