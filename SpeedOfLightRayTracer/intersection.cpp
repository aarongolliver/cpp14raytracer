#include "stdafx.h"
#include "intersection.h"


intersection::intersection(float d, genvec::fvec3 n) : d(d), n(n), valid(true)
{
}

intersection::intersection() : d(), n(), valid(false)
{
}


intersection::~intersection()
{
}
