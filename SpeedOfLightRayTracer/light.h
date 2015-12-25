#pragma once

#include "genvec.h"

class light
{
public:
	light(genvec::pos pos, genvec::rgb color);
	~light();
	const genvec::pos pos;
	const genvec::rgb color;
};

