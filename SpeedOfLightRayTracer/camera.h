#pragma once
#include "genvec.h"
#include <sstream>

using genvec::pos;
using genvec::fvec3;
using std::endl;
using std::stringstream;

class ray
{
public:
	const pos e;
	const fvec3 dir;
	ray(const pos& p, const fvec3& dir)
		: e(p), dir(dir.normalized())
	{

	}
};

class camera
{

public:
	camera(int width, int height);
	camera(const genvec::ivec2& dim);
	ray castRay(int i, int j);
	void reposition(const pos& center, const pos& lookat, const fvec3& up);
	pos center() const { return e; };

	auto str() {
		stringstream s;
		s << "e: " << e.str() << endl;
		s << "u: " << u.str() << endl;
		s << "v: " << v.str() << endl;
		s << "w: " << w.str() << endl;
		return s.str();
	}

	pos e;
	fvec3 u, v, w;
	const int wid, hei;
	const float r, l, t, b, d;
};