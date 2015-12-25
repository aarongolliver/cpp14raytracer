#include "stdafx.h"
#include "camera.h"

camera::camera(int width, int height)
	:
	wid(width),
	hei(height),
	r(static_cast<float>(width)),
	l(static_cast<float>(-width)),
	t(static_cast<float>(height)),
	b(static_cast<float>(-height)),
	d(static_cast<float>(height))
{};

camera::camera(const genvec::ivec2& dim)
	: camera(dim[0], dim[1])
{};

void camera::reposition(const pos& center, const pos& lookat, const fvec3& up)
{
	auto lookdir = (lookat - center).normalized();
	this->w = lookdir * (-1.f);
	this->u = cross(up, w).normalized();
	this->v = cross(w, u).normalized();
	this->e = center;
}

ray camera::castRay(int i, int j)
{
	auto  U = l + (r - l) * (i + 0.5f) / wid;
	auto  V = b + (t - b) * (j + 0.5f) / hei;
	auto dir = ((-d)*w) + (u*U) + (v*V);

	return ray{ this->e, dir };
}
