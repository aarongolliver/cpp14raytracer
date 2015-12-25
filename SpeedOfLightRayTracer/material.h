#pragma once
#include "genvec.h"

class material
{
public:
	material(float alpha, genvec::rgb ambient, genvec::rgb diff, genvec::rgb spec, float n, float r);
	material();
	~material();

	const float alpha; // 0 = transparent, 1 = opaque
	const genvec::rgb ambient;
	const genvec::rgb diff;
	const genvec::rgb spec;
	const float n;
    const float r;
};

namespace materials {
    const auto red =   material{ 1, { .1,0,0 },{ 1,0,0 },{ .1,.1,.1 }, 20, 0.5f };
    const auto green = material{ 1, { .1,0,0 },{ 0,1,0 },{ .1,.1,.1 }, 20, 0.5f };
    const auto blue =  material{ 1, {  0,0,0 },{ 0,0,1 },{ .1,.1,.1 }, 20, 0.5f };
    
    const auto white = material{ 1, { .1f,.1f,.1f },{ 1,1,1 },{ .1,.1,.1 }, 20, .1f };
}

