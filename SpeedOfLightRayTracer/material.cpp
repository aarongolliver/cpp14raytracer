#include "stdafx.h"
#include "material.h"


material::material()
	: alpha(1), ambient(), diff({}), spec(), n(), r(0)
{
}

material::material(float alpha, genvec::rgb ambient, genvec::rgb diff, genvec::rgb spec, float n, float r)
	: alpha(alpha), ambient(ambient), diff(diff), spec(spec), n(n), r(r)
{
}


material::~material()
{
}
