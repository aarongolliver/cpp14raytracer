#pragma once
class intersection
{
public:
    intersection(float d, genvec::fvec3 n);
    intersection();
    ~intersection();
    float d;
    genvec::fvec3 n;
    bool valid;
};

