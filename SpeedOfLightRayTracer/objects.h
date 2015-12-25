#pragma once
#include <memory>
class object {
public:
    object(material mat) :mat(mat) {}
    virtual intersection intersect(const ray& ray) const = 0;
    const material mat;
};

class triangle : public object {
private:
    const pos a;
    const pos b;
    const pos c;

    auto hit_test(ray r) const {
        auto t = dot(a - r.e, n) / dot(r.dir, n);
        auto x = r.e + r.dir * t;
        if (t >= 0)
            if (dot(cross(b - a, x - a), n) > 0)
                if (dot(cross(c - b, x - b), n) > 0)
                    if (dot(cross(a - c, x - c), n) > 0)
                        return t;
        return -1.f;
    }
public:
    const fvec3 n;
    triangle(pos a, pos b, pos c, material mat) :
        object(mat)
        , a(a)
        , b(b)
        , c(c)
        , n(cross(a - b, b - c).normalized()) {}

    virtual intersection intersect(const ray& r) const override {
        auto dist = hit_test(r);
        if (dist < 0)
            return{};
        else
            return{ dist, n };
    }

};

class plane : public object {
private:
    const triangle a;
    const triangle b;
public:
    const fvec3 n;
    plane(const pos& a, const pos& b, const pos& c, const pos& d, material mat) :
          object(mat)
        , a(triangle(a, b, c, mat))
        , b(triangle(a, c, d, mat))
        , n(this->a.n) {}

    virtual intersection intersect(const ray& r) const override {
        auto h1 = a.intersect(r);
        if (h1.valid)
            return h1;
        return b.intersect(r);
    }

};

class sphere : public object {
private:
    pos c;
    float rsq;
public:
    sphere(const pos& center, float radius, material mat) :
        object(mat)
        , c(center)
        , rsq(radius*radius) {}

    virtual intersection intersect(const ray& r) const override {
        const auto& d = r.dir;
        const auto& e = r.e;
        auto A = dot(d, d);
        auto B = dot(2 * d, e - c);
        auto C = dot(e - c, e - c) - rsq;
        auto disc = B*B - 4 * A*C;

        if (disc < 0) // doesn't hit
        {
            return{};
        }
        else { // hits 
            auto distp = (-B + std::sqrt(disc)) / (2 * A);
            auto dists = (-B - std::sqrt(disc)) / (2 * A);

            if (dists < 0 && distp < 0)
                return{};

            auto dist = (dists > 0) ? dists : distp;
            return{ dist, ((r.e + r.dir * dist) - c).normalized() };
        }
    }
};

auto make_sphere(const pos& p, float r, material mat = materials::red) {
    return std::make_shared<sphere>(p, r, mat);
}

auto make_triangle(const pos& a, const pos& b, const pos& c, material mat = materials::white) {
    return std::make_shared<triangle>(a, b, c, mat);
}

auto make_plane(const pos& a, const pos& b, const pos& c, const pos& d, material mat = materials::white) {
    return std::make_shared<plane>(a, b, c, d, mat);
}
