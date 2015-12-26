// SpeedOfLightRayTracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "camera.h"
#include "simplePPM.h"
#include "material.h"
#include "light.h"
#include "intersection.h"
#include "objects.h"
#include "randutils.h"

using namespace genvec;
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;
using genvec::dot;
using std::vector;
using std::cout;
using std::cin;
using std::integral_constant;
using std::pair;
using std::make_pair;

using object_ptr = shared_ptr<object>;
using scene = pair<vector<object_ptr>, vector<light>>;

auto load_scene() {
    return make_pair(
        vector<object_ptr>{
        make_plane({ -5,-5,5 }, { -5,5,5 }, { 5,5,5 }, { 5,-5,5 }),
            make_plane({ 5,5,-5 }, { -5,5,-5 }, { -5,-5,-5 }, { 5,-5,-5 }),

            make_plane({ 5,5,5 }, { -5,5,5 }, { -5,5,-5 }, { 5,5,-5 }),

            make_plane({ 5,-5,-5 }, { -5,-5,-5 }, { -5,-5,5 }, { 5,-5,5 }),
            make_plane({ 5,5,5 }, { 5,5,-5 }, { 5,-5,-5 }, { 5,-5,5 }),
            make_plane({ -5,-5,5 }, { -5,-5,-5 }, { -5,5,-5 }, { -5,5,5 }),

            make_sphere({ 2.5f,0,-3 }, 2, materials::red),
            make_sphere({ 2.5f,0,3 }, 2, materials::blue),
    }
    ,
        vector<light> {
        light{ pos{ -3, -3, -3 }, rgb{ 1,1,1 } },
            light{ pos{ 3, 3, 3 }, rgb{ 1,1,1 } },
    }
    );
}

auto get_closest_intersection(const scene& scene, const ray& r) {
    object const* closest_object = nullptr;
    intersection closest_intersection;

    for (const auto& obj : scene.first) {
        auto intersect = obj->intersect(r);

        if (intersect.valid && (!closest_object || closest_intersection.d > intersect.d)) {
            closest_object = obj.get();
            closest_intersection = intersect;
        }
    }

    return make_pair(closest_object, closest_intersection);
}

template<int reflections_left>
rgb intersect_scene(const scene& scene, const ray& r, std::mt19937_64& mt, const int reflection_bounces = 1, const int shadow_bounces = 100, const float reflection_spread = 100, const float shadow_spread = 20) {
    if (reflections_left < 0) return r.dir.abs();
    object const* closest_object;
    intersection closest_intersection;

    std::tie(closest_object, closest_intersection) = get_closest_intersection(scene, r);

    if (closest_object) {
        auto color = rgb{ 0,0,0 };
        auto mat = closest_object->mat;
        auto norm = closest_intersection.n;

        auto pos_of_intersect = r.e + closest_intersection.d * r.dir;


        for (const auto& light : scene.second) {
            auto v = r.dir * (-1.f);
            auto dist_to_light = (light.pos - pos_of_intersect).len();
            auto l = (light.pos - pos_of_intersect).normalized();
            auto h = (v + l).normalized();
            auto n = mat.n;

            for (auto i = 0; i < shadow_bounces; i++) {
                auto wiggle = (shadow_bounces > 1) ? random_point_on_sphere(mt) : fvec3{ 0,0,0 };


                auto ray_to_light = ray{ pos_of_intersect + (norm * .001f), (l * shadow_spread + wiggle).normalized() };
                intersection intersection_towards_light;
                std::tie(std::ignore, intersection_towards_light) = get_closest_intersection(scene, ray_to_light);

                auto visible = !(intersection_towards_light.valid && (intersection_towards_light.d < dist_to_light));
                if (visible) {
                    auto diffuse = mat.diff * light.color * std::max(0.f, dot(norm, l));
                    auto specular = mat.spec * light.color * std::pow(std::max(0.f, dot(norm, h)), n);
                    color += (diffuse.abs() + specular.abs()) / shadow_bounces;
                }
            }

            auto ambient = light.color * mat.ambient;
            color += ambient.abs();
        }

        // reflectance
        if (mat.r > 0) {
            for (auto i = 0; i < reflection_bounces; i++) {
                auto wiggle = (reflection_bounces > 1) ? random_point_on_sphere(mt) : fvec3{ 0,0,0 };

                auto reflect_dir = ((r.dir - 2 * dot(r.dir, norm)*norm).normalized() * reflection_spread + wiggle).normalized();
                auto reflect_ray = ray{ pos_of_intersect + (norm * .001f), reflect_dir };
                auto reflect_color = intersect_scene<reflections_left - 1>(scene, reflect_ray, mt, 1, std::max(shadow_bounces / 2, 1));
                color += (reflect_color * mat.r) / reflection_bounces;
            }
        }

        return color;
    }

    return r.dir.abs();
}

template<>
rgb intersect_scene<0>(const scene&, const ray&, std::mt19937_64&, const int, const int, const float, const float) {
    return{ 0,0,0 };
}

int main()
{
    const auto size = ivec2{ 500, 500 };
    auto c = camera{ size };
    auto img = vector<rgb>(size[0] * size[1]);
    c.reposition({ -4.999f,0.001f,.001 }, { 0.001f,-0.01,-0.001f }, { 0,1,0 }); // jiggled

    auto s = load_scene();

#pragma omp parallel for schedule(dynamic)
    for (auto y = 0; y < size[1]; ++y) {
        std::mt19937_64 mt((uint64_t(randutils::devurand()) << 32) | randutils::devurand());
        
        if (y % 100 == 0)
            cout << y*100.f / size[1] << endl;
        
        for (auto x = 0; x < size[0]; ++x) {
            const auto idx = y*size[0] + x;
            const auto r = c.castRay(x, y);
            img[idx] = intersect_scene<3>(s, r, mt);
        }
    }

    auto max = img[0].len();
    for (auto& col : img) {
        max = std::max(max, col.len());
    }

    auto imgscaled = vector<bvec<3>>(size[0] * size[1]);
    for (int i = 0; i < size[0] * size[1]; ++i) {
        auto red = static_cast<uint8_t>(img[i][0] / max * 0xFF);
        auto gre = static_cast<uint8_t>(img[i][1] / max * 0xFF);
        auto blu = static_cast<uint8_t>(img[i][2] / max * 0xFF);
        auto alp = static_cast<uint8_t>(0xFF);
        imgscaled[i] = { red, gre, blu };
    }

    simplePPM_write_ppm("out.ppm", size[0], size[1], &(imgscaled[0][0]));
}